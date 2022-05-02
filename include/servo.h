#pragma once 

#include "gpio.h"

class Servo
{
public:
    Servo(uint8_t pin);
    ~Servo();

    void write_servo(uint32_t period_us);

private:
    const uint8_t m_pin;
};