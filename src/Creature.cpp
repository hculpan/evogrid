#include "Creature.h"

#include "raylib.h"

#include <random>

Creature::Creature() : health_(800), age_(0) {}

void Creature::draw(int x, int y) {
  DrawRectangle(x * 4, y * 4 + 50, 4, 4, WHITE);
}

ActionType Creature::update(int dt) {
  static std::random_device rd;  // non-deterministic seed
  static std::mt19937 gen(rd()); // Mersenne Twister RNG
  static std::uniform_int_distribution<> dist(0, 7);

  health_--;
  age_++;

  if (health_ <= 0) {
    return ActionType::DIE;
  } else if (health_ > 800 && age_ > 400) {
    health_ = health_ / 2;
    age_ = 0;
    return ActionType::SPAWN;
  }

  return static_cast<ActionType>(dist(gen));
}

void Creature::addHealth(int v) { health_ += v; }

int Creature::getHealth() { return health_; }