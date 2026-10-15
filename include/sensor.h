#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include <Arduino.h>
#include "config.h"

class LineSensor {
public:
    LineSensor(const sensor_cfg_t& cfg);
    void begin();
    int  read();

private:
    const sensor_cfg_t& _cfg;
};

#endif