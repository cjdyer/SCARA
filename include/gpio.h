#pragma once

#include "log.h"
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

class GPIO
{
public:
    static void initialise_gpio();
    static void set_mode_gpio(uint8_t _pin, uint8_t _mode);
    static void write_gpio(uint8_t _pin, bool _level);

private:
    static volatile uint32_t  *m_gpio_register;
    static volatile uint32_t m_peripheral_base;
    static bool pi_is_2711;

    #define SYST_BASE  (m_peripheral_base + 0x003000)
    #define DMA_BASE   (m_peripheral_base + 0x007000)
    #define CLK_BASE   (m_peripheral_base + 0x101000)
    #define GPIO_BASE  (m_peripheral_base + 0x200000)
    #define UART0_BASE (m_peripheral_base + 0x201000)
    #define PCM_BASE   (m_peripheral_base + 0x203000)
    #define SPI0_BASE  (m_peripheral_base + 0x204000)
    #define I2C0_BASE  (m_peripheral_base + 0x205000)
    #define PWM_BASE   (m_peripheral_base + 0x20C000)
    #define BSCS_BASE  (m_peripheral_base + 0x214000)
    #define UART1_BASE (m_peripheral_base + 0x215000)
    #define I2C1_BASE  (m_peripheral_base + 0x804000)
    #define I2C2_BASE  (m_peripheral_base + 0x805000)
    #define DMA15_BASE (m_peripheral_base + 0xE05000)

    #define DMA_LEN   0x1000 /* allow access to all channels */
    #define CLK_LEN   0xA8
    #define GPIO_LEN  0xF4
    #define SYST_LEN  0x1C
    #define PCM_LEN   0x24
    #define PWM_LEN   0x28
    #define I2C_LEN   0x1C
    #define BSCS_LEN  0x40

    #define GPSET0 7
    #define GPCLR0 10
};
