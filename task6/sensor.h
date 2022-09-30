#ifndef SENSOR_H
#define SENSOR_H

class Sensor {

public:

    virtual bool is_safe(coordinate_t x, coordinate_t y) = 0;

    virtual ~Sensor() = default;

};

#endif // SENSOR_H
