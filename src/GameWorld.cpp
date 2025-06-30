#include "GameWorld.h"

GameWorld::GameWorld(size_t w, size_t h)
    : width_(w), height_(h), currGrid_(w * h), oldGrid_(w * h) {}

void GameWorld::initialize() {
  oldGrid_[idx(width_ / 2, height_ / 2)].emplace(Cell());
}

// place a copy of c into the slot
void GameWorld::spawn(int x, int y, const Cell &c) {
  currGrid_[idx(x, y)].emplace(c);
}

void GameWorld::kill(int x, int y) { currGrid_[idx(x, y)].reset(); }

void GameWorld::move(int x1, int y1, int x2, int y2) {
  auto &from = oldGrid_[idx(x1, y1)];
  currGrid_[idx(x2, y2)] = std::move(from);
  from.reset();
}

void GameWorld::update(int dt) {
  // Clear current grid
  for (auto cell : currGrid_) {
    cell.reset();
  }

  for (int x = 0; x < width_; x++) {
    for (int y = 0; y < height_; y++) {
      auto &cell = oldGrid_[idx(x, y)];
      if (cell) {
        int nx = x, ny = y;
        ActionType a = cell->update(dt);
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
        }

        if (nx < 0) {
          nx += width_;
        } else if (nx >= width_) {
          nx -= width_;
        }

        if (ny < 0) {
          ny += height_;
        } else if (ny >= height_) {
          ny -= height_;
        }

        this->move(x, y, nx, ny);
      }
    }
  }

  std::swap(currGrid_, oldGrid_);
}

int GameWorld::getWidth() { return width_; }

int GameWorld::getHeight() { return height_; }

bool GameWorld::hasValue(int x, int y) {
  return oldGrid_[idx(x, y)].has_value();
}