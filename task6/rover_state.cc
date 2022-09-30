#include "rover_state.h"

void RoverState::set_state(Direction _direction, Position _position) {
    direction = _direction;
    position = _position;
    has_landed = true;
    run();
}

std::string RoverState::get_state_string() const {
    if (!has_landed) {
        return "unknown";
    }

    std::string result = "(" + std::to_string(position.x) + ", " +
                         std::to_string(position.y) + ") ";

    result += direction_to_string(direction);

    if (has_stopped) {
        result += " stopped";
    }

    return result;
}

void RoverState::stop() {
    has_stopped = true;
}

void RoverState::run() {
    has_stopped = false;
}

bool RoverState::landed() const {
    return has_landed;
}

Position RoverState::get_position() const {
    return position;
}

Position RoverState::get_forward() const {
    return next_position(direction, position);
}

Position RoverState::get_backward() const {
    return prev_position(direction, position);
}

void RoverState::move_forward() {
    position = get_forward();
}

void RoverState::move_backward() {
    position = get_backward();
}

void RoverState::rotate_right() {
    direction = direction_on_the_right(direction);
}

void RoverState::rotate_left() {
    direction = direction_on_the_left(direction);
}
