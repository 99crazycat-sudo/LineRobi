#include "encoder.h"

Encoder::Encoder(const motor_cfg_t& cfg)
    : _cfg(cfg), _count(0) {}

void Encoder::begin() {
    pinMode(_cfg.pin_enc_a, INPUT_PULLUP);
    pinMode(_cfg.pin_enc_b, INPUT_PULLUP);
}

long Encoder::count() {
    noInterrupts();
    long c = _count;
    interrupts();
    return c;
}

void Encoder::reset() {
    noInterrupts();
    _count = 0;
    interrupts();
}

float Encoder::distanceMM() {
    return count() * MM_PER_IMP;
}

void Encoder::tick() {
    bool a = digitalRead(_cfg.pin_enc_a);
    bool b = digitalRead(_cfg.pin_enc_b);

    int8_t dir = (a == b) ? +1 : -1;
    if (_cfg.invert) dir = -dir; 

    _count += dir;
}