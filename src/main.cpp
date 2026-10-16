#include <Arduino.h>
#include "config.h"
#include "motor.h"
#include "encoder.h"
#include "sensor.h"

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

config_t& GetConfig(void) {
    return config;
}

static Motor      Motor_L(config.motor_L);
static Motor      Motor_R(config.motor_R);
static Encoder    Enc_L(config.motor_L); 
static Encoder    Enc_R(config.motor_R);
static LineSensor Sensor_L(config.sensor_L);
static LineSensor Sensor_R(config.sensor_R);

static constexpr int   DRIVE_SPEED = 60;
static constexpr float TARGET_MM   = 200.0f;

static bool  moving = false;
static unsigned long last_print_ms = 0;

void init_cfg(void) {
    // --- Левый мотор ---
    config.motor_L.pin_in1   = PIN_IN1_L;
    config.motor_L.pin_in2   = PIN_IN2_L;
    config.motor_L.pin_en    = PIN_EN_L;
    config.motor_L.pin_enc_a = PIN_ENC_L_A;
    config.motor_L.pin_enc_b = PIN_ENC_L_B;
    config.motor_L.invert    = false;
    config.motor_L.max_speed = 255;
    config.motor_L.dead_zone = 0;

    // --- Правый мотор ---
    config.motor_R.pin_in1   = PIN_IN1_R;
    config.motor_R.pin_in2   = PIN_IN2_R;
    config.motor_R.pin_en    = PIN_EN_R;
    config.motor_R.pin_enc_a = PIN_ENC_R_A;
    config.motor_R.pin_enc_b = PIN_ENC_R_B;
    config.motor_R.invert    = true;
    config.motor_R.max_speed = 255;
    config.motor_R.dead_zone = 0;

    // --- Датчики линии ---
    config.sensor_L.pin = PIN_L_SENSOR;
    config.sensor_R.pin = PIN_R_SENSOR;
}

void isrEncoderL() { Enc_L.tick(); }
void isrEncoderR() { Enc_R.tick(); }

static float avgDistanceMM() {
    float dL = Enc_L.distanceMM();
    float dR = Enc_R.distanceMM();
    return (abs(dL) + abs(dR)) * 0.5f;
}

static void startMove() {
    Enc_L.reset();
    Enc_R.reset();
    moving = true;
    Motor_L.setSpeed(DRIVE_SPEED);
    Motor_R.setSpeed(DRIVE_SPEED);
}

static void stopMove() {
    moving = false;
    Motor_L.setSpeed(0);
    Motor_R.setSpeed(0);
}

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

    Serial.println(F("start"));

    Enc_L.reset();
    Enc_R.reset();

    startMove();
}

void loop() {
    if (moving && avgDistanceMM() >= TARGET_MM) {
        stopMove();
        Serial.print(F("done, L=")); Serial.print(Enc_L.distanceMM(), 1);
        Serial.print(F(" mm, R="));  Serial.print(Enc_R.distanceMM(), 1);
        Serial.println(F(" mm"));
    }

    unsigned long now = millis();
    if (now - last_print_ms >= 100) {
        last_print_ms = now;
        Serial.print(F("L="));  Serial.print(Enc_L.distanceMM(), 1);
        Serial.print(F(" R=")); Serial.print(Enc_R.distanceMM(), 1);
        Serial.print(F(" avg=")); Serial.print(avgDistanceMM(), 1);
        Serial.print(F(" moving=")); Serial.println(moving);
    }
}