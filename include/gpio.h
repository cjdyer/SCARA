// #include <stdio.h>
// #include <unistd.h>
// #include <stdint.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <sys/types.h>

// namespace GPIO
// {
//     static volatile uint32_t piPeriphBase = 0x20000000;

//     static volatile int pi_is_2711 = 0;

//     #define SYST_BASE (piPeriphBase + 0x003000)
//     #define DMA_BASE (piPeriphBase + 0x007000)
//     #define CLK_BASE (piPeriphBase + 0x101000)
//     #define GPIO_BASE (piPeriphBase + 0x200000)
//     #define UART0_BASE (piPeriphBase + 0x201000)
//     #define PCM_BASE (piPeriphBase + 0x203000)
//     #define SPI0_BASE (piPeriphBase + 0x204000)
//     #define I2C0_BASE (piPeriphBase + 0x205000)
//     #define PWM_BASE (piPeriphBase + 0x20C000)
//     #define BSCS_BASE (piPeriphBase + 0x214000)
//     #define UART1_BASE (piPeriphBase + 0x215000)
//     #define I2C1_BASE (piPeriphBase + 0x804000)
//     #define I2C2_BASE (piPeriphBase + 0x805000)
//     #define DMA15_BASE (piPeriphBase + 0xE05000)

//     #define DMA_LEN 0x1000 /* allow access to all channels */
//     #define CLK_LEN 0xA8
//     #define GPIO_LEN 0xF4
//     #define SYST_LEN 0x1C
//     #define PCM_LEN 0x24
//     #define PWM_LEN 0x28
//     #define I2C_LEN 0x1C
//     #define BSCS_LEN 0x40

//     #define GPSET0 7
//     #define GPSET1 8

//     #define GPCLR0 10
//     #define GPCLR1 11

//     #define GPLEV0 13
//     #define GPLEV1 14

//     #define GPPUD 37
//     #define GPPUDCLK0 38
//     #define GPPUDCLK1 39

//     #define GPPUPPDN0 57
//     #define GPPUPPDN1 58
//     #define GPPUPPDN2 59
//     #define GPPUPPDN3 60

//     #define SYST_CS 0
//     #define SYST_CLO 1
//     #define SYST_CHI 2

//     static volatile uint32_t *gpioReg = static_cast<uint32_t *> MAP_FAILED;
//     static volatile uint32_t *systReg = static_cast<uint32_t *> MAP_FAILED;
//     static volatile uint32_t *bscsReg = static_cast<uint32_t *> MAP_FAILED;

//     #define PI_BANK (gpio >> 5)
//     #define PI_BIT (1 << (gpio & 0x1F)) // Change to _pin

//     #define PI_INPUT 0
//     #define PI_OUTPUT 1
//     #define PI_ALT0 4
//     #define PI_ALT1 5
//     #define PI_ALT2 6
//     #define PI_ALT3 7
//     #define PI_ALT4 3
//     #define PI_ALT5 2

//     #define PI_PUD_OFF 0
//     #define PI_PUD_DOWN 1
//     #define PI_PUD_UP 2

//     void gpioSetMode(unsigned gpio, unsigned mode);
//     int gpioGetMode(unsigned gpio);
//     void gpioSetPullUpDown(unsigned gpio, unsigned pud);
//     int gpioRead(unsigned gpio);
//     void gpioWrite(unsigned gpio, unsigned level);
//     void gpioTrigger(unsigned gpio, unsigned pulseLen, unsigned level);
//     uint32_t gpioReadBank1();
//     uint32_t gpioReadBank2();
//     void gpioClearBank1(uint32_t bits);
//     void gpioClearBank2(uint32_t bits);
//     void gpioSetBank1(uint32_t bits);
//     void gpioSetBank2(uint32_t bits);
//     unsigned gpioHardwareRevision();
//     uint32_t gpioTick();
//     static uint32_t *initMapMem(int fd, uint32_t addr, uint32_t len);
//     int gpioInitialise();

// } // namespace GPIO