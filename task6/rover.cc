#include "rover.h"

void Rover::land(Position position, Direction direction) {
    rover_state.set_state(direction, position);
}

void Rover::execute(const std::string& command_string) {
    rover_state.run();

    if (!rover_state.landed()) {
        throw NotLandedYet();
    }
    
    for (char symbol : command_string) {
        auto command_it = commands.find(symbol);
        if (command_it == commands.end() || !command_it->second->execute(rover_state, sensors)) {
            rover_state.stop();
            break;
        }
    }
}

void Rover::add_sensor(std::unique_ptr<Sensor>&& sensor) {
    sensors.emplace_back(std::move(sensor));
}

void Rover::program_command(char symbol, std::shared_ptr<Command>& command_ptr) {
    commands[symbol] = command_ptr;
}

std::ostream& operator<<(std::ostream& os, const Rover& rover) {
    os << rover.rover_state.get_state_string();
    return os;
}

RoverBuilder& RoverBuilder::program_command(char symbol, std::shared_ptr<Command> command_ptr) {
    rover.program_command(symbol, command_ptr);
    return *this;
}

RoverBuilder& RoverBuilder::add_sensor(std::unique_ptr<Sensor>&& sensor_ptr) {
    rover.add_sensor(std::move(sensor_ptr));
    return *this;
}

Rover RoverBuilder::build() {
    return std::move(rover);
}
