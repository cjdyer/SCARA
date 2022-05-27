#pragma once 

// #include "gpio.h"
#include <wiringPi.h>

class Servo
{
public:
    Servo(int pin);
    ~Servo();

    void write_servo(int period_us);

private:
    const int m_pin;
};