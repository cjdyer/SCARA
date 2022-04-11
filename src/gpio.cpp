#include "gpio.h"

void GPIO::initialise_gpio()
{
    int fd;
    unsigned int rev = 0;

    FILE *filp;
    char buf[512];

    if (filp = fopen("/proc/device-tree/soc/ranges" , "rb"))
    {
        if (fread(buf, 1, sizeof(buf), filp) >= 8)
        {
            piPeriphBase = buf[4] << 24 | buf[5] << 16 | buf[6] << 8 | buf[7];
            if (!piPeriphBase) 
            {
                piPeriphBase = buf[8] << 24 | buf[9] << 16 | buf[10] << 8 | buf[11];
            }
            if (piPeriphBase == 0xFE00000) pi_is_2711 = 1;
        }

        fclose(filp);
    }

    fd = open("/dev/mem", O_RDWR | O_SYNC) ;

    //! TODO: Check for error here, (fd < 0) probably forgot sudo

    m_gpio_register = mmap(0, GPIO_LEN, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_SHARED|MAP_LOCKED, fd, GPIO_BASE);

    close(fd);

    return 0;
}

void GPIO::set_mode_gpio(uint8_t _pin, uint8_t _mode)
{
   int reg   =  _pin / 10;
   int shift = (_pin % 10) * 3;
   m_gpio_register[reg] = (m_gpio_register[reg] & ~(7 << shift)) | (_mode << shift);
}

void GPIO::write_gpio(uint8_t _pin, bool _logic)
{
   if (_level == 0) *(_pin + GPCLR0 + PI_BANK) = PI_BIT;
   else             *(_pin + GPSET0 + PI_BANK) = PI_BIT;
}
