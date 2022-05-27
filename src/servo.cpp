#include "servo.h"

// 400  min
// 900 nuetral
// 1500 max

Servo::Servo(int pin) : m_pin(pin) 
{ 
    pinMode(1, PWM_OUTPUT);
    // GPIO::gpio_set_function(m_pin, GPIO::PI_FUNCTION::OUTPUT);
    //  set the PWM mode to milliseconds stype
    pwmSetMode(PWM_MODE_MS);

    //  divide down clock
    pwmSetClock(192);
    pwmSetRange(2000);
    // GPIO::pwm_start(m_pin);
}

Servo::~Servo() 
{ 
    // GPIO::pwm_stop();
}

void Servo::write_servo(int pulse)
{
    pwmWrite(1, pulse);
    // GPIO::pwm_write(20000, period_us);
}