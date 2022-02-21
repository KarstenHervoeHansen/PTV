/* comdrv.h - device specification file */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1998.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#define COM1 /* valid choices are COM1, COM2, COM3, or COM4 */

#ifdef COM1
#define BASE    0x3f8 /* std COM1 */
#define COM_IRQ 4
#endif

#ifdef COM2
#define BASE    0x2f8 /* std COM2 */
#define COM_IRQ 3
#endif

#ifdef COM3
#define BASE    0x3e8 /* std COM3 */
#define COM_IRQ 5     /* 4 or 5 */
#endif

#ifdef COM4
#define BASE    0x2e8 /* std COM4 */
#define COM_IRQ 3     /* 3 or 7 */
#endif

#define DLAB	 	0x80		/* devisor latch access bit  */

#define COMINT  8+COM_IRQ            /* interrupt # */
#define COM_INT_DISABLE (1<<COM_IRQ) /* PIC mask */
#define COM_INT_ENABLE  (~COM_INT_DISABLE)

#define UART_TBR        BASE+0 /* transmit buffer register */
#define UART_RBR        BASE+0 /* receive buffer register */
#define UART_INT_EN_REG BASE+1 /* interrupt enable register */
#define UART_INT_ID_REG BASE+2 /* interrupt id register */
#define UART_LCR        BASE+3 /* line control register */
#define UART_MCR        BASE+4 /* modem control register */
#define UART_LSR        BASE+5 /* line status register */
#define UART_MSR        BASE+6 /* modem status register */

#define TX_BUFF_EMPTY 0x02
#define RX_DATA_READY 0x04

#define RX_INT_ENABLE    0x01
#define TX_INT_ENABLE    0x02
#define ERR_INT_ENABLE   0x04
#define MSR_INT_ENABLE   0x08

#define INT_DISABLE      0x00

#define COM_MCR 0x0b /* DTR, RTS */

#define EOI           0x20
#define PIC_EOI_ADDR  0x20
#define PIC_MASK_ADDR 0x21

/* End of file - comdrv.h */
