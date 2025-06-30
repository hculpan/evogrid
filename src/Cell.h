#pragma once

#include <optional>
#include <vector>

enum class ActionType : uint8_t {
  MOVE_STRAIGHT,
  MOVE_STRAIGHT_RIGHT,
  MOVE_RIGHT,
  MOVE_BACK_RIGHT,
  MOVE_BACK,
  MOVE_BACK_LEFT,
  MOVE_LEFT,
  MOVE_STRAIGHT_LEFT
};

class Cell {
public:
  Cell() = default;
  ~Cell() = default;

  ActionType update(int dt);

private:
  size_t age = 0, health = 100;
};