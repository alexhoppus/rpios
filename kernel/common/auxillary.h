#ifndef AUXILLARY_H
#define AUXILLARY_H
#ifdef RPI2
#include "../common/arch/BCM2836.h" /* Raspberriy Pi 2 */
#else
#include "../common/arch/BCM2835.h" /* Original B,A,A+,B+ */
#endif
#define AUX_ENABLES     (PBASE + 0x00215004)
#endif
