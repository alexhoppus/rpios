#ifndef GPIO_H
#define GPIO_H
#ifdef RPI2
#include "../common/arch/BCM2836.h" /* Raspberriy Pi 2 */
#else
#include "../common/arch/BCM2835.h" /* Original B,A,A+,B+ */
#endif
#define GPFSEL1         (PBASE + 0x00200004)
#define GPSET0          (PBASE + 0x0020001C)
#define GPCLR0          (PBASE + 0x00200028)
#define GPPUD           (PBASE + 0x00200094)
#define GPPUDCLK0       (PBASE + 0x00200098)
#endif
