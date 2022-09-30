#ifndef ROVER_H
#define ROVER_H

#include <memory>
#include <vector>
#include <map>
#include <cassert>

#include "command.h"

struct NotLandedYet : public std::exception {

    const char* what() const noexcept override {
        return "The rover has not landed yet.";
    }

};

class Rover {

private:

    std::vector<std::unique_ptr<Sensor>> sensors;

    std::map<char, std::shared_ptr<Command>> commands;

    RoverState rover_state;

public:

    Rover() = default;

    Rover(Rover&& other) = default;

    Rover(const Rover& other) = delete;

    void land(Position position, Direction direction);

    void execute(const std::string& command_string);

    void add_sensor(std::unique_ptr<Sensor>&& sensor);

    void program_command(char symbol, std::shared_ptr<Command>& command_ptr);

    friend std::ostream& operator<<(std::ostream& os, const Rover& rover);

};

class RoverBuilder {

private:

    Rover rover;

public:

    RoverBuilder& program_command(char symbol, std::shared_ptr<Command> command_ptr);

    RoverBuilder& add_sensor(std::unique_ptr<Sensor>&& sensor_ptr);

    Rover build();
    
};

#endif // ROVER_H
