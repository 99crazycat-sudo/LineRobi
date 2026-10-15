#include <Arduino.h>
#include "config.h"
#include "motor.h"
#include "sensor.h"
#include "encoder.h"

static const config_t cfg = {
    {   8,   7,  6,  false,       255,         0 },
    {  9,   10,  5,  true,        255,         0 },
    {  A2 },
    {  A3 },
    {  2,   4,  false },
    {  3,  11,  false },
};

Motor      motorL(cfg.motor_L);
Motor      motorR(cfg.motor_R);
LineSensor sensorL(cfg.sensor_L);
LineSensor sensorR(cfg.sensor_R);
Encoder    encoderL(cfg.encoder_L);
Encoder    encoderR(cfg.encoder_R);

Encoder* pEncoderL = &encoderL;
Encoder* pEncoderR = &encoderR;

void isrEncoderL() { pEncoderL->tick(); }
void isrEncoderR() { pEncoderR->tick(); }

constexpr int DRIVE_SPEED = 130;

void driveMM(float target_mm) {
    encoderL.reset();
    encoderR.reset();

    while (true) {
        float dL = encoderL.distanceMM();
        float dR = encoderR.distanceMM();
        float avg = (abs(dL) + abs(dR)) * 0.5f;

        if (avg >= target_mm - (float)STOP_THRESHOLD_IMP * MM_PER_IMP) {
            break;
        }

        motorL.setSpeed(DRIVE_SPEED);
        motorR.setSpeed(DRIVE_SPEED);
    }

    motorL.setSpeed(0);
    motorR.setSpeed(0);
}

void setup() {
    motorL.begin();
    motorR.begin();
    sensorL.begin();
    sensorR.begin();
    encoderL.begin();
    encoderR.begin();
    motorL.reverse();
    attachInterrupt(digitalPinToInterrupt(cfg.encoder_L.pin_a), isrEncoderL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(cfg.encoder_R.pin_a), isrEncoderR, CHANGE);

    driveMM(500.0f);   // проехать 500 мм
}

void loop() {
}