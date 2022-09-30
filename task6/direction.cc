#include "direction.h"

std::string direction_to_string(const Direction& direction) {
    switch (direction) {
        case Direction::NORTH:
            return "NORTH";
        case Direction::EAST:
            return "EAST";
        case Direction::SOUTH:
            return "SOUTH";
        default:
            return "WEST";
    }
}

Position next_position(const Direction& direction, const Position& position) {
    switch (direction) {
        case Direction::NORTH:
            return { position.x, position.y + 1 };
        case Direction::EAST:
            return { position.x + 1, position.y };
        case Direction::SOUTH:
            return { position.x, position.y - 1 };
        default:
            return { position.x - 1, position.y };
    }
}

Position prev_position(const Direction& direction, const Position& position) {
    switch (direction) {
        case Direction::NORTH:
            return { position.x, position.y - 1 };
        case Direction::EAST:
            return { position.x - 1, position.y };
        case Direction::SOUTH:
            return { position.x, position.y + 1 };
        default:
            return { position.x + 1, position.y };
    }
}

Direction direction_on_the_right(const Direction& direction) {
    switch (direction) {
        case Direction::NORTH:
            return Direction::EAST;
        case Direction::EAST:
            return Direction::SOUTH;
        case Direction::SOUTH:
            return Direction::WEST;
        default:
            return Direction::NORTH;
    }
}

Direction direction_on_the_left(const Direction& direction) {
    switch (direction) {
        case Direction::NORTH:
            return Direction::WEST;
        case Direction::EAST:
            return Direction::NORTH;
        case Direction::SOUTH:
            return Direction::EAST;
        default:
            return Direction::SOUTH;
    }
}
