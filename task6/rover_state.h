#ifndef ROVER_STATE_H
#define ROVER_STATE_H

#include "direction.h"

class RoverState {

private:

    Direction direction = Direction::NORTH;
    Position position = {0, 0};

    bool has_landed = false;
    bool has_stopped = false;

public:

    void set_state(Direction _direction, Position _position);

    std::string get_state_string() const;

    void stop();

    void run();

    bool landed() const;

    Position get_position() const;

    Position get_forward() const;

    Position get_backward() const;

    void move_forward();

    void move_backward();

    void rotate_right();

    void rotate_left();

};

#endif // ROVER_STATE_H
