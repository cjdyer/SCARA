#include "servo.h"

// 600  min
// 1500 nuetral
// 2400 max

Servo::Servo(uint8_t pin) : m_pin(pin) 
{ 
    GPIO::gpio_set_function(m_pin, GPIO::PI_FUNCTION::OUTPUT);
    GPIO::pwm_start(m_pin);
}

Servo::~Servo() 
{ 
    GPIO::pwm_stop();
}

void Servo::write_servo(uint32_t period_us)
{
    GPIO::pwm_write(20000, period_us);
}