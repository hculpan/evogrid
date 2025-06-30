#pragma once

#include <optional>
#include <vector>

#include "Cell.h"

class GameWorld {
public:
  GameWorld(size_t width, size_t height);
  ~GameWorld() = default;

  void initialize();

  void spawn(int x, int y, const Cell &c);

  void kill(int x, int y);

  void move(int x1, int y1, int x2, int y2);

  void update(int dt);

  bool hasValue(int x, int y);

  int getWidth();

  int getHeight();

private:
  size_t width_, height_;
  std::vector<std::optional<Cell>> currGrid_;
  std::vector<std::optional<Cell>> oldGrid_;

  // helper: convert (x,y) → flat index
  size_t idx(int x, int y) const { return y * width_ + x; }
};