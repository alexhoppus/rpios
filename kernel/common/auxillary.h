#ifndef AUXILLARY_H
#define AUXILLARY_H
#ifdef RPI2
#include "../common/arch/BCM2836.h" /* Raspberriy Pi 2 */
#else
#include "../common/arch/BCM2835.h" /* Original B,A,A+,B+ */
#endif

#define AUX_ENABLES     (VPBASE + 0x00215004)
/* Mini UART 1 base ports information*/
#define AUX_ENABLES     (VPBASE + 0x00215004)
/* Mini Uart I/O Data */
#define AUX_MU_IO_REG   (VPBASE + 0x00215040)
/* Mini Uart Interrupt Enable */
#define AUX_MU_IER_REG  (VPBASE + 0x00215044)
/* Mini Uart Interrupt Identifity */
#define AUX_MU_IIR_REG  (VPBASE + 0x00215048)
/* Mini Uart Line Control */
#define AUX_MU_LCR_REG  (VPBASE + 0x0021504C)
/* Mini Uart Modem Control */
#define AUX_MU_MCR_REG  (VPBASE + 0x00215050)
/* Mini Uart Line Status */
#define AUX_MU_LSR_REG  (VPBASE + 0x00215054)
/* Mini Uart Modem Status */
#define AUX_MU_MSR_REG  (VPBASE + 0x00215058)
/* Mini Uart Scratch */
#define AUX_MU_SCRATCH  (VPBASE + 0x0021505C)
/* Mini Uart Extra Control */
#define AUX_MU_CNTL_REG (VPBASE + 0x00215060)
/* Mini Uart Extra Status */
#define AUX_MU_STAT_REG (VPBASE + 0x00215064)
/* Mini Uart Baudrate */
#define AUX_MU_BAUD_REG (VPBASE + 0x00215068)

/* AUX_MU_LCR_REG Register (0x0021 504C) */
/*
 * bit 0: 0 - 7bit mode 1 - 8bit mode
 * */
#define AUX_MU_LCR_REG_8BITMODE (1 << 0) | (1 << 1)

/* AUX_MU_IIR_REG Register (0x0021 5048) */
/*
 * bit 0: 1 - Clear pending interrupts
 * bit 1: 1 - Clear recv FIFO
 * bit 2: 1 - Clear trans FIFO
*/
#define AUX_MU_IIR_REG_CLEAR_PENDINT (1 << 0)
#define AUX_MU_IIR_REG_CLEAR_RFIFO (1 << 1)
#define AUX_MU_IIR_REG_CLEAR_WFIFO (1 << 2)

/* AUX_MU_CNTL_REG Register (0x0021 5060) */
/*
 * bit 0: 1 - Enable reciever
 * bit 1: 1 - Enable transmitter
 */

#define AUX_MU_CNTL_REG_RE (1 << 0)
#define AUX_MU_CNTL_REG_TE (1 << 1)

/* AUX_MU_LSR_REG Register (0x0021 5054) */
/*
 * bit 0: 1 - reciever FIFO held at least 1 byte
 * bit 5: 1 - transmitter FIFO can accept at least
 *		pne byte
 */

#define AUX_MU_LST_REG_RRDY (1 << 0)
#define AUX_MU_LST_REG_TRDY (1 << 5)
#endif
