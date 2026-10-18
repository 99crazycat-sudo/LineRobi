#include <Arduino.h>
#include "config.h"
#include "motor.h"
#include "encoder.h"
#include "sensor.h"
#include "motion.h"

#define PIN_R_SENSOR   A3
#define PIN_L_SENSOR   A2

#define PIN_IN1_L       7
#define PIN_IN2_L       8
#define PIN_EN_L        6
#define PIN_ENC_L_A     2
#define PIN_ENC_L_B     4

#define PIN_IN1_R       9
#define PIN_IN2_R      10
#define PIN_EN_R        5
#define PIN_ENC_R_A     3
#define PIN_ENC_R_B    11

config_t config;

static Encoder    Enc_L(config.motor_L);
static Encoder    Enc_R(config.motor_R);
static Motor      Motor_L(config.motor_L, Enc_L);
static Motor      Motor_R(config.motor_R, Enc_R);
static LineSensor Sensor_L(config.sensor_L);
static LineSensor Sensor_R(config.sensor_R);

// Создаем объект Motion
static Motion     RobotMotion(Motor_L, Motor_R, Enc_L, Enc_R);

void init_cfg(void) {
    // --- Левый мотор ---
    config.motor_L.pin_in1   = PIN_IN1_L;
    config.motor_L.pin_in2   = PIN_IN2_L;
    config.motor_L.pin_en    = PIN_EN_L;
    config.motor_L.pin_enc_a = PIN_ENC_L_A;
    config.motor_L.pin_enc_b = PIN_ENC_L_B;
    config.motor_L.invert    = false;
    config.motor_L.max_speed = 255;
    config.motor_L.dead_zone = 60;
    config.motor_L.pid.kp      = 1.5f;
    config.motor_L.pid.ki      = 3.0f;
    config.motor_L.pid.kd      = 0.0f;
    config.motor_L.pid.min_out = -255.0f;
    config.motor_L.pid.max_out =  255.0f;

    // --- Правый мотор ---
    config.motor_R.pin_in1   = PIN_IN1_R;
    config.motor_R.pin_in2   = PIN_IN2_R;
    config.motor_R.pin_en    = PIN_EN_R;
    config.motor_R.pin_enc_a = PIN_ENC_R_A;
    config.motor_R.pin_enc_b = PIN_ENC_R_B;
    config.motor_R.invert    = true;
    config.motor_R.max_speed = 255;
    config.motor_R.dead_zone = 60;
    config.motor_R.pid.kp      = 1.5f;
    config.motor_R.pid.ki      = 3.0f;
    config.motor_R.pid.kd      = 0.0f;
    config.motor_R.pid.min_out = -255.0f;
    config.motor_R.pid.max_out =  255.0f;

    // --- Датчики линии ---
    config.sensor_L.pin = PIN_L_SENSOR;
    config.sensor_R.pin = PIN_R_SENSOR;
}

void isrEncoderL() { Enc_L.tick(); }
void isrEncoderR() { Enc_R.tick(); }

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    init_cfg();

    Enc_L.begin();
    Enc_R.begin();
    Motor_L.begin();
    Motor_R.begin();
    Sensor_L.begin();
    Sensor_R.begin();

    attachInterrupt(digitalPinToInterrupt(PIN_ENC_L_A), isrEncoderL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_R_A), isrEncoderR, CHANGE);

    Serial.println(F("Start Sequence"));
}

void loop() {
    if (RobotMotion.isDone()) {
        static int step = 0;
        if (step == 0) {
            RobotMotion.drive(200.0f, 150.0f);
            step++;
        } 
        else if (step == 1) {
            RobotMotion.stop(1500);
            step++;
        }
        else if (step == 2) {
            RobotMotion.turn(90.0f, 0.0f, 100.0f);
            step++;
        }
        else if (step == 3) {
            RobotMotion.turn(90.0f, 250.0f, 120.0f);
            step++;
        }
    }

    RobotMotion.update();
}