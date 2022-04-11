#pragma once

class GPIO
{
public:
    static void initialise_gpio();
    static void set_mode_gpio(uint8_t _pin, uint8_t _mode);
    static void write_gpio(uint8_t _pin, uint8_t _level)

private:
    static volatile uint32_t  *m_gpio_register;
};
