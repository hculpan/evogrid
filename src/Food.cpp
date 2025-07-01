#include "Food.h"

#include "raylib.h"

void Food::draw(int x, int y) { DrawRectangle(x * 4, y * 4 + 50, 4, 4, GREEN); }

ActionType Food::update(int dt) { return ActionType::NO_ACTION; }

int Food::getNutrition() { return 40; }