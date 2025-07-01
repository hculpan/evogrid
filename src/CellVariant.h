#pragma once

#include "Creature.h"
#include "Food.h"
#include <variant>

using CellVariant = std::variant<Food, Creature>;