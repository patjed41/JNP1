#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>

#include "position.h"

#define DIRECTIONS 4

enum class Direction {
    NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3
};

std::string direction_to_string(const Direction& direction);

Position next_position(const Direction& direction, const Position& position);

Position prev_position(const Direction& direction, const Position& position);

Direction direction_on_the_right(const Direction& direction);

Direction direction_on_the_left(const Direction& direction);

#endif // DIRECTION_H
