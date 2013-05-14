/**
 * $Id: de2-70.h 471 2011-02-22 10:38:08Z klugeflo $
 */

/******************************************************************************

File: de2-70.h

Project: Roomba Embedded Systems Training

Description: ALTERA DE2-70 on-board devices using OR32
             The addresses in this file can also be used for assembly code.
             If you include this file into your assemly file, make
             sure that you have __ASSEMBLY__ defined.

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS training
26.04.2011 (MG) Modified for use in embedded practical training

*/

#ifndef BOARD_INCLUDE
#error Do not directly include this file, use <board.h> instead!
#endif

#ifndef _DE2_70_H
#define _DE2_70_H 1


/****************************************************************** Includes */

#ifndef __ASSEMBLY__
#include <stdint.h>
#endif /* __ASSEMBLY__ */

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************* Defines */

//////////////
// LED outputs
//////////////

#define HAVE_LEDS_DE2_70 1

// other defines are in led.h

#define A_SEVEN_SEG   0xf0000120 /*!< 8 digit seven-segment display */
#define A_SSEG_POINTS 0xf0000130 /*!< the points of the seven-segment display */

#ifndef __ASSEMBLY__
  /*!
    Points of seven segment displays
    0 = on
    1 = off
  */
#define SSEG_POINTS (*((uint8_t volatile *) A_SSEG_POINTS))
#define SEVEN_SEG (*((uint32_t volatile *) A_SEVEN_SEG))
#endif /* __ASSEMBLY__ */


  //////
  // LCD
  //////

#define HAVE_LCD 0

  // addresses
#define A_LCD_BASE    0xf0000080 /*!< LCD base address */
#define A_LCD_CMD     (A_LCD_BASE + 0x00) /*!< LCD command (wo) */
#define A_LCD_ST      (A_LCD_BASE + 0x04) /*!< LCD status (ro) */
#define A_LCD_WR      (A_LCD_BASE + 0x08) /*!< LCD data (wo) */
#define A_LCD_RD      (A_LCD_BASE + 0x0c) /*!< LCD data (ro) */
#define A_LCD_ON      (A_LCD_BASE + 0x10) /*!< LCD switch on */

#ifndef __ASSEMBLY__
  // devices
#define LCD_BASE  (*((uint32_t volatile *) A_LCD_BASE))
#define LCD_CMD   (*((uint8_t volatile *)  A_LCD_CMD )) /*!< LCD command (wo) */
#define LCD_ST    (*((uint8_t volatile *)  A_LCD_ST  )) /*!< LCD status (ro) */
#define LCD_WR    (*((uint8_t volatile *)  A_LCD_WR  )) /*!< LCD data (wo) */
#define LCD_RD    (*((uint8_t volatile *)  A_LCD_RD  )) /*!< LCD data (ro) */
#define LCD_ON    (*((uint8_t volatile *)  A_LCD_ON  )) /*!< LCD switch on */
#endif /* __ASSEMBLY__ */


  //////////
  // Buttons
  //////////

#define HAVE_BUTTONS 1

  // adresses
#define A_PIO_BT_BASE     0xf00000e0 /*!< Four buttons base address */
#define A_PIO_BT_DATA     (A_PIO_BT_BASE + 0x0) /*!< Data */
#define A_PIO_BT_DIR      (A_PIO_BT_BASE + 0x4) /*!< Direction */
#define A_PIO_BT_IRQ_MASK (A_PIO_BT_BASE + 0x8) /*!< IRQ mask */
#define A_PIO_BT_EC       (A_PIO_BT_BASE + 0xc) /*!< Edge Capture */

#ifndef __ASSEMBLY__
  // devices
#define PIO_BT_BASE      (*((uint8_t volatile *) A_PIO_BT_BASE))
#define PIO_BT_DATA      (*((uint8_t volatile *) A_PIO_BT_DATA)) /*!< Four buttons */
#define PIO_BT_DIR       (*((uint8_t volatile *) A_PIO_BT_DIR)) /*!< Buttons IRQ maske */
#define PIO_BT_IRQ_MASK  (*((uint8_t volatile *) A_PIO_BT_IRQ_MASK)) /*!< Buttons IRQ maske */
#define PIO_BT_EC        (*((uint8_t volatile *) A_PIO_BT_EC)) /*!< Buttons IRQ maske */
#endif /* __ASSEMBLY__ */


  ///////////
  // Switches
  ///////////

#define HAVE_SWITCHES 1

  // addresses
#define A_SWITCHES    0xf0000100 /*!< 18 Switches */

#ifndef __ASSEMBLY__
  // devices
#define SWITCHES  (*((uint32_t volatile *) A_SWITCHES)) /*!< 18 Switches - for assignment refer to #LEDR_MASK */
#endif /* __ASSEMBLY__ */


  ///////
  // PS/2
  ///////

#define HAVE_PS2 1

  // addresses
#define A_PS2_BASE    0xf0000500 /*!< PS/2 base address */
#define A_PS2_RX      (A_PS2_BASE + 0x0) /*!< PS/2 RX */
#define A_PS2_ST      (A_PS2_BASE + 0x4) /*!< PS/2 Status */

#ifndef __ASSEMBLY__
  // devices
#define PS2_BASE (*((uint32_t volatile *) A_PS2_BASE))
#define PS2_RX   (*((uint32_t volatile *) A_PS2_RX  )) /*!< PS/2 RX */
#define PS2_ST   (*((uint32_t volatile *) A_PS2_ST  )) /*!< PS/2 Status */
#endif /* __ASSEMBLY__ */


  ///////
  // UART
  ///////

#define HAVE_UART 1

  // addresses
#define A_UART_BASE   0xf0000000 /*!< UART base address */
#define A_UART_RX     (A_UART_BASE + 0x00) /*!< UART rxdata */
#define A_UART_TX     (A_UART_BASE + 0x04) /*!< UART txdata */
#define A_UART_ST     (A_UART_BASE + 0x08) /*!< UART status */
#define A_UART_CT     (A_UART_BASE + 0x0c) /*!< UART control */
#define A_UART_DV     (A_UART_BASE + 0x10) /*!< UART divisor */
#define A_UART_EP     (A_UART_BASE + 0x14) /*!< UART end-of-packet */

#ifndef __ASSEMBLY__
  // devices
#define UART_BASE (*((uint32_t volatile *) A_UART_BASE ))
#define UART_RX   (*((uint8_t volatile *)  A_UART_RX   )) /*!< UART rxdata */
#define UART_TX   (*((uint8_t volatile *)  A_UART_TX   )) /*!< UART txdata */
#define UART_ST   (*((uint32_t volatile *) A_UART_ST   )) /*!< UART status */
#define UART_CT   (*((uint32_t volatile *) A_UART_CT   )) /*!< UART control */
#define UART_DV   (*((uint32_t volatile *) A_UART_DV   )) /*!< UART divisor */
#define UART_EP   (*((uint32_t volatile *) A_UART_EP   )) /*!< UART end-of-packet */
#endif /* __ASSEMBLY__ */


  ///////////
  // Ethernet
  ///////////

#define HAVE_ETH_DE2_70 1

  // addresses
#define ETH_ADDR 0xf0000800

#ifndef __ASSEMBLY__
  // devices
#define ETH_INDEX (uint16_t *) (ETH_ADDR + 0x0)
#define ETH_DATA (uint16_t *) (ETH_ADDR + 0x4)
#endif /* __ASSEMBLY__ */


  //////
  // VGA
  //////

#define HAVE_VGA_DE2_70 1

  // addresses
#define A_VGA_BASE    0xe0000000
#define A_VGA         0xe0000000 /*!< VGA controller base address */
#define A_VGA_MUX_CLK 0xe0000040 /*!< VGA MUX clock address */

#ifndef __ASSEMBLY__
  // devices
#define VGA_BASE    (*((uint32_t volatile *) A_VGA_BASE))
//#define VGA_BASE    A_VGA_BASE
#define VGA         (*((uint32_t volatile *) A_VGA)) /*!< VGA controller base address */
#define VGA_MUX_CLK (*((uint8_t volatile *)  A_VGA_MUX_CLK)) /*!< VGA MUX clock address */
#endif /* __ASSEMBLY__ */

/*
#define VGA_CTRL   0x00
#define VGA_FRAMES 0x08
#define VGA_RES    0x0c
#define VGA_HSYNC  0x10
#define VGA_VSYNC  0x14
#define VGA_TFT    0x18
*/

  //////
  // USB
  //////

  // not implemented

  // addresses
#define A_USB         0xf0000200

#ifndef __ASSEMBLY__
  // devices
#define USB       (*((uint8_t volatile *) A_USB))
#endif /* __ASSEMBLY__ */



  ///////
  // IrDA
  ///////

  // not implemented

  // addresses
#define A_IRDA        0xf0000600

#ifndef __ASSEMBLY__
  // devices
#define IRDA (*((uint32_t volatile *) A_IRDA))
#endif /* __ASSEMBLY__ */


  //////////
  // SD Card
  //////////

  // not implemented

  // addresses
#define A_SD_CMD      0xf0000300
#define A_SD_DAT      0xf0000310
#define A_SD_CLK      0xf0000320
#ifndef __ASSEMBLY__
  // devices
#define SD_CMD (*((uint32_t volatile *) A_SD_CMD))
#define SD_DAT (*((uint32_t volatile *) A_SD_DAT))
#define SD_CLK (*((uint32_t volatile *) A_SD_CLK))
#endif /* __ASSEMBLY__ */


  //////////
  // Mermory
  //////////

  // addresses
#define A_SDRAM         0x00000000 /*!< on-board SDRAM */
#define A_FLASH         0x04000000 /*!< on-board Flash-ROM */
#define A_LSRAM         0x05000000 /*!< on-board SRAM - used for VGA */
#define A_OCRAM         0xff000000 /*!< on-chip RAM - here reside the execption vectors */

#ifndef __ASSEMBLY__
  // devices
#define SDRAM (*((uint32_t volatile *) A_SDRAM )) /*!< on-board SDRAM */
#define FLASH (*((uint32_t volatile *) A_FLASH )) /*!< on-board Flash-ROM */
#define LSRAM (*((uint32_t volatile *) A_LSRAM )) /*!< on-board SRAM - used for VGA */
#define OCRAM (*((uint32_t volatile *) A_OCRAM )) /*!< on-chip RAM - here reside the execption vectors */
#endif /* __ASSEMBLY__ */

  ////////////
  // SPI / CAN
  ////////////

  // addresses
  // TODO
#ifndef __ASSEMBLY__
  // devices
  // TODO
#endif /* __ASSEMBLY__ */


  //////
  // ???
  //////

  // addresses
#ifndef __ASSEMBLY__
  // devices
#endif /* __ASSEMBLY__ */


  ///////
  // IRQs
  ///////

#define N_IRQS     20 /*!< Number of IRQs available */

#define IRQ_UART    2 /*!< IRQ from UART */
#define IRQ_BUTTON  3 /*!< IRQ from push-button */
#define IRQ_DMA     4 /*!< IRQ from ethernet controller */
#define IRQ_PS2     5 /*!< IRQ from keyboard */


/****************************************************************** Typedefs */


/************************************************************** Global const */


/********************************************************** Global variables */


/************************************************ Global function prototypes */


/*************************************************** Global inline functions */


/******************************************************************** Macros */

	/*
name:	just a name for the section
addr:	8-bit part of the exception adress, use (entry & 0xff00)>>8
	*/
#define UNHANDLED_EXCEPTION(name, addr)\
	.global name ## _vec		   ;\
name ## _vec:				   ;\
	LOAD_SYMBOL_2_GPR(r5, A_SEVEN_SEG) ;\
	l.addi	r6, r0, lo(addr)	;\
	l.slli	r6, r6, 16		;\
	l.movhi r7, 0xee00	    ;\
	l.or	r6, r6, r7	    ;\
	l.mfspr	r7, r0, SPR_EPCR_BASE ;\
	l.andi	r7, r7, 0xffff	      ;\
	l.or	r6, r6, r7	      ;\
	l.sw    0x0(r5), r6	    ;\
	l.j	0		           ;\
	l.nop


#ifdef __cplusplus
}
#endif

#endif /* !_DE2_70_H */
