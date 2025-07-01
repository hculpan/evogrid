#include "raylib.h"

#include "GameWorld.h"

#include <iostream>
#include <random>
#include <string>
#include <variant>

GameWorld::GameWorld(size_t w, size_t h)
    : width_(w), height_(h), currGrid_(w * h), oldGrid_(w * h) {
  std::uniform_int_distribution<> ydist(0, height_);
}

int GameWorld::randomWidth() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> xdist(0, width_);

  return xdist(gen);
}

int GameWorld::randomHeight() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> xdist(0, height_);

  return xdist(gen);
}

void GameWorld::initialize() {
  loop_ = 0;

  for (int i = 0; i < 10000; i++) {
    oldGrid_[idx(randomWidth(), randomHeight())].emplace(Food());
  }

  for (int i = 0; i < 10; i++) {
    int x = randomWidth(), y = randomHeight();
    while (true) {
      if (!oldGrid_[idx(x, y)])
        break;
      x = randomWidth(), y = randomHeight();
    }
    oldGrid_[idx(x, y)].emplace(Creature());
  }
}

// place a copy of c into the slot
void GameWorld::spawn(int x, int y, const CellVariant &c) {
  currGrid_[idx(x, y)].emplace(c);
}

void GameWorld::kill(int x, int y) { currGrid_[idx(x, y)].reset(); }

void GameWorld::move(int x1, int y1, int x2, int y2) {
  auto &from = oldGrid_[idx(x1, y1)];
  currGrid_[idx(x2, y2)] = std::move(from);
  from.reset();
}

void GameWorld::update() {
  if (!running_)
    return;

  // 1) Clear the “next” grid
  for (auto &cell : currGrid_) {
    cell.reset();
  }
  bugCount_ = 0;

  // 2) Single generic pass over every oldGrid_ cell
  for (int x = 0; x < width_; ++x) {
    for (int y = 0; y < height_; ++y) {
      auto &optCell = oldGrid_[idx(x, y)];
      if (!optCell)
        continue;

      auto &variantCell = *optCell;

      // a) Run its update() and get the ActionType
      int currLoop = loop_;
      ActionType a = std::visit(
          [currLoop](auto &c) { return c.update(currLoop); }, variantCell);

      // b) Compute wrapped new coords
      int nx = x, ny = y;
      switch (a) {
      case ActionType::MOVE_STRAIGHT:
        ny--;
        break;
      case ActionType::MOVE_STRAIGHT_RIGHT:
        ny--, nx++;
        break;
      case ActionType::MOVE_RIGHT:
        nx++;
        break;
      case ActionType::MOVE_BACK_RIGHT:
        ny++, nx++;
        break;
      case ActionType::MOVE_BACK:
        ny++;
        break;
      case ActionType::MOVE_BACK_LEFT:
        ny++, nx--;
        break;
      case ActionType::MOVE_LEFT:
        nx--;
        break;
      case ActionType::MOVE_STRAIGHT_LEFT:
        ny--, nx--;
        break;
      case ActionType::SPAWN: {
        auto c = optCell.value();
        ny--;
        if (ny < 0) {
          ny += 2;
        }
        spawn(x, y, c);
        std::cout << "New birth" << std::endl;
        break;
      }
      case ActionType::DIE:
        optCell.reset();
        break;
      case ActionType::NO_ACTION:
      default:
        break;
      }
      if (nx < 0)
        nx += width_;
      else if (nx >= width_)
        nx -= width_;
      if (ny < 0)
        ny += height_;
      else if (ny >= height_)
        ny -= height_;

      size_t targetIdx = idx(nx, ny);
      auto &targetOpt = oldGrid_[targetIdx];

      // c) If it’s a Creature, consume any Food at the destination
      std::visit(
          [&](auto &c) {
            using T = std::decay_t<decltype(c)>;
            if constexpr (std::is_same_v<T, Creature>) {
              bugCount_++;
              if (targetOpt && std::holds_alternative<Food>(*targetOpt)) {
                auto nutrition = std::get<Food>(*targetOpt).getNutrition();
                targetOpt.reset(); // eat
                c.addHealth(nutrition);
              }
            }
          },
          variantCell);

      // d) Move every cell (Creature, Food, or future types) into currGrid_
      move(x, y, nx, ny);
    }

    // Add new food
    seedValue_ += seedRate_;
    while (seedValue_ >= 1.0) {
      int x = randomWidth(), y = randomHeight();
      while (true) {
        if (!currGrid_[idx(x, y)])
          break;
        x = randomWidth(), y = randomHeight();
      }
      currGrid_[idx(x, y)].emplace(Food());
      seedValue_ -= 1.0;
    }
  }

  // 3) Swap buffers for the next frame
  std::swap(currGrid_, oldGrid_);
  loop_++;

  if (bugCount_ == 0) {
    running_ = false;
  }
}

void GameWorld::draw() {
  BeginDrawing();
  ClearBackground(BLACK);
  int idx = 0;
  for (int x = 0; x < width_; x++) {
    for (int y = 0; y < height_; y++) {
      auto cell = oldGrid_[idx];
      if (cell) {
        std::visit([x, y](auto &cell) { cell.draw(x, y); }, *cell);
      }
      idx++;
    }
  }

  std::string txt1 = "Cycle: " + std::to_string(loop_);
  DrawText(txt1.c_str(), 20, 12, 24, WHITE);

  std::string txt2 = "Bugs: " + std::to_string(bugCount_);
  DrawText(txt2.c_str(), width_ * 4 - 120, 12, 24, WHITE);

  if (bugCount_ == 0) {
    int textWidth = MeasureText("All bugs are dead!", 48);
    DrawRectangle(width_ * 2 - (textWidth / 2) - 20, height_ * 2 - 10,
                  textWidth + 40, 68, BLACK);
    DrawText("All bugs are dead!", width_ * 2 - (textWidth / 2), height_ * 2,
             48, RED);
    seedRate_ = 0;
  }

  DrawRectangle(width_ * 4 - 110, height_ * 4 + 20, 110, 35, BLACK);
  DrawFPS(width_ * 4 - 100, height_ * 4 + 25);
  EndDrawing();
}

int GameWorld::getWidth() { return width_; }

int GameWorld::getHeight() { return height_; }

bool GameWorld::hasValue(int x, int y) {
  return oldGrid_[idx(x, y)].has_value();
}