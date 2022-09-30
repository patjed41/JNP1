#include <vector>
#include <memory>

#include "command.h"

static bool check_next_position(
    const Position& next_position,
    std::vector<std::unique_ptr<Sensor>>& sensors
) {
    for (auto& sensor : sensors) {
        if (!sensor->is_safe(next_position.x, next_position.y)) {
            return false;
        }
    }

    return true;
}

bool ForwardCommand::execute(
    RoverState& rover_state,
    std::vector<std::unique_ptr<Sensor>>& sensors
) const {
    Position position_after_execution = rover_state.get_forward();

    if (!check_next_position(position_after_execution, sensors)) {
        return false;
    }

    rover_state.move_forward();
    return true;
}

bool BackwardCommand::execute(
    RoverState& rover_state,
    std::vector<std::unique_ptr<Sensor>>& sensors
) const {
    Position position_after_execution = rover_state.get_backward();
    
    if (!check_next_position(position_after_execution, sensors)) {
        return false;
    }

    rover_state.move_backward();
    return true;
}

bool RotateRightCommand::execute(
    RoverState& rover_state,
    [[maybe_unused]] std::vector<std::unique_ptr<Sensor>>& sensors
) const {
    rover_state.rotate_right();
    return true;
}

bool RotateLeftCommand::execute(
    RoverState& rover_state,
    [[maybe_unused]] std::vector<std::unique_ptr<Sensor>>& sensors
) const {
    rover_state.rotate_left();
    return true;
}

bool ComposeCommand::execute(
    RoverState& rover_state,
    std::vector<std::unique_ptr<Sensor>>& sensors
) const {
    for (const auto& command_ptr : commands) {
        if (!command_ptr->execute(rover_state, sensors)) {
            return false;
        }
    }
    
    return true;
}

std::shared_ptr<Command> move_forward() {
    return std::static_pointer_cast<Command>(std::make_shared<ForwardCommand>());
}

std::shared_ptr<Command> move_backward() {
    return std::static_pointer_cast<Command>(std::make_shared<BackwardCommand>());
}

std::shared_ptr<Command> rotate_right() {
    return std::static_pointer_cast<Command>(std::make_shared<RotateRightCommand>());
}

std::shared_ptr<Command> rotate_left() {
    return std::static_pointer_cast<Command>(std::make_shared<RotateLeftCommand>());
}

std::shared_ptr<Command> compose(std::initializer_list<std::shared_ptr<Command>> init_list) {
    return std::static_pointer_cast<Command>(std::make_shared<ComposeCommand>(init_list));
}
