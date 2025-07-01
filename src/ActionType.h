#pragma once

#include <cstdint>

enum class ActionType : uint8_t {
  MOVE_STRAIGHT,
  MOVE_STRAIGHT_RIGHT,
  MOVE_RIGHT,
  MOVE_BACK_RIGHT,
  MOVE_BACK,
  MOVE_BACK_LEFT,
  MOVE_LEFT,
  MOVE_STRAIGHT_LEFT,
  NO_ACTION,
  DIE,
  SPAWN
};
