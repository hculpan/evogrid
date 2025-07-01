#include "raylib.h"

#include <random>

#include "GameWorld.h"

int main() {
  const int screenWidth = 1400;
  const int screenHeight = 1400;

  InitWindow(screenWidth, screenHeight, "EvoGrid");

  auto gw = GameWorld(screenWidth / 4, screenHeight / 4);
  gw.initialize();

  while (!WindowShouldClose()) {
    gw.update();

    gw.draw();
  }

  CloseWindow();
  return 0;
}