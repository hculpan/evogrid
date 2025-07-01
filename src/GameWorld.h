#pragma once

#include <optional>
#include <vector>

#include "CellVariant.h"

class GameWorld {
public:
  GameWorld(size_t width, size_t height);
  ~GameWorld() = default;

  void initialize();

  void spawn(int x, int y, const CellVariant &c);

  void kill(int x, int y);

  void move(int x1, int y1, int x2, int y2);

  void update();

  void draw();

  bool hasValue(int x, int y);

  int getWidth();

  int getHeight();

private:
  size_t width_, height_, loop_;
  double seedRate_ = 0.01;
  double seedValue_ = 0;
  int bugCount_ = 0;
  bool running_ = true;
  std::vector<std::optional<CellVariant>> currGrid_;
  std::vector<std::optional<CellVariant>> oldGrid_;

  int randomWidth();
  int randomHeight();

  size_t idx(int x, int y) const { return y * width_ + x; }
};