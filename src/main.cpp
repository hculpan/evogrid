#include "raylib.h"

#include <random>

#include "GameWorld.h"

int main() {
  const int screenWidth = 1400;
  const int screenHeight = 1400;

  InitWindow(screenWidth, screenHeight, "EvoGrid");

  auto gw = GameWorld(screenWidth / 4, screenHeight / 4);
  gw.initialize();

  int loop = 0;
  while (!WindowShouldClose()) {
    loop++;

    // draw
    BeginDrawing();
    ClearBackground(BLACK);
    for (int x = 0; x < gw.getWidth(); x++) {
      for (int y = 0; y < gw.getHeight(); y++) {
        if (gw.hasValue(x, y)) {
          DrawRectangle(x * 4, y * 4, 4, 4, WHITE);
        }
      }
    }
    DrawFPS(screenWidth - 110, screenHeight - 40);
    EndDrawing();

    // update
    gw.update(loop);
  }

  CloseWindow();
  return 0;
}