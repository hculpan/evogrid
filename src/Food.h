#pragma once

#include "ActionType.h"

class Food {
public:
  Food() = default;
  ~Food() = default;

  void draw(int x, int y);

  ActionType update(int dt);

  int getNutrition();

  int getHealth() { return 1; };

  void setAge(int v) {};
};