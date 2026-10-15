#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

constexpr float WHEEL_DIAMETER = 60.0f;
constexpr int   IMP_PER_ROTATE = 491;

constexpr float WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER;
constexpr float MM_PER_IMP          = WHEEL_CIRCUMFERENCE / IMP_PER_ROTATE;

constexpr float STOP_THRESHOLD_MM  = 3.0f;
constexpr long  STOP_THRESHOLD_IMP = (long)(STOP_THRESHOLD_MM / MM_PER_IMP);

struct motor_cfg_t {
    uint8_t pin_in1;
    uint8_t pin_in2;
    uint8_t pin_en;
    bool    invert;
    int     max_speed;
    uint8_t dead_zone;
};

struct sensor_cfg_t {
    uint8_t pin;
};

struct encoder_cfg_t {
    uint8_t pin_a;
    uint8_t pin_b;
    bool    invert;
};

struct config_t {
    motor_cfg_t   motor_L;
    motor_cfg_t   motor_R;
    sensor_cfg_t  sensor_L;
    sensor_cfg_t  sensor_R;
    encoder_cfg_t encoder_L;
    encoder_cfg_t encoder_R;
};

#endif