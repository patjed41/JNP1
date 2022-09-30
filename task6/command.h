#ifndef COMMAND_H
#define COMMAND_H

#include "rover_state.h"
#include "sensor.h"

class Command {

public:

    virtual bool execute(RoverState& rover_state, std::vector<std::unique_ptr<Sensor>>& sensors) const = 0;

    virtual ~Command() = default;

};

class ForwardCommand : public Command {

public:

    bool execute(
        RoverState& rover_state,
        std::vector<std::unique_ptr<Sensor>>& sensors
    ) const override;
    
};

class BackwardCommand : public Command {

public:

    bool execute(
        RoverState& rover_state,
        std::vector<std::unique_ptr<Sensor>>& sensors
    ) const override;

};

class RotateRightCommand : public Command {

public:

    bool execute(
        RoverState& rover_state,
        [[maybe_unused]] std::vector<std::unique_ptr<Sensor>>& sensors
    ) const override;

};

class RotateLeftCommand : public Command {

public:

    bool execute(
        RoverState& rover_state,
        [[maybe_unused]] std::vector<std::unique_ptr<Sensor>>& sensors
    ) const override;

};

class ComposeCommand : public Command {

private:

    std::vector<std::shared_ptr<Command>> commands;

public:

    ComposeCommand() = default;

    ComposeCommand(std::initializer_list<std::shared_ptr<Command>> init_list) : commands(init_list) {}

    ~ComposeCommand() override = default;

    bool execute(
        RoverState& rover_state,
        std::vector<std::unique_ptr<Sensor>>& sensors
    ) const override;

};

std::shared_ptr<Command> move_forward();

std::shared_ptr<Command> move_backward();

std::shared_ptr<Command> rotate_right();

std::shared_ptr<Command> rotate_left();

std::shared_ptr<Command> compose(std::initializer_list<std::shared_ptr<Command>> init_list);

#endif // COMMAND_H
