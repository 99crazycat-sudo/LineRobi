#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

constexpr float WHEEL_DIAMETER = 60.0f;
constexpr int   IMP_PER_ROTATE = 982;
constexpr float WHEEL_WIDTH    = 156.0f; // Колея (расстояние между центрами колес), мм

constexpr float WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER;
constexpr float MM_PER_IMP          = WHEEL_CIRCUMFERENCE / IMP_PER_ROTATE;

constexpr unsigned long VEL_UPDATE_MS = 20;

struct pid_cfg_t {
    float kp;
    float ki;
    float kd;
    float min_out;
    float max_out;
};

struct motor_cfg_t {
    uint8_t pin_in1;
    uint8_t pin_in2;
    uint8_t pin_en;
    uint8_t pin_enc_a;
    uint8_t pin_enc_b;
    bool    invert;
    int     max_speed;
    uint8_t dead_zone;
    pid_cfg_t pid;
};

struct sensor_cfg_t {
    uint8_t pin;
};

struct config_t {
    motor_cfg_t  motor_L;
    motor_cfg_t  motor_R;
    sensor_cfg_t sensor_L;
    sensor_cfg_t sensor_R;
};

#endif