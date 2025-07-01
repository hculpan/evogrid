#pragma once

#include "ActionType.h"

class Creature {
public:
  Creature();
  ~Creature() = default;

  void draw(int x, int y);

  ActionType update(int dt);

  void addHealth(int v);

  int getHealth();

  void setAge(int v) { age_ = v; }

private:
  int health_, age_;
};