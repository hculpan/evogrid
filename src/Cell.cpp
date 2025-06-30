#include "Cell.h"

#include <random>

ActionType Cell::update(int dt) {
  static std::random_device rd;  // non-deterministic seed
  static std::mt19937 gen(rd()); // Mersenne Twister RNG
  static std::uniform_int_distribution<> dist(0, 10);

  return static_cast<ActionType>(dist(gen));
}