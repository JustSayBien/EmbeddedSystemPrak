/*
 * boot.c
 *
 *  Created on: Mar 23, 2011
 *      Author: bradatch
 */


typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;


/*
 * JATG UART boot loader for DB_4CE10 Altera board with OR1K
 */

#define SDRAM_ADDR		0x0
#define JTAG_ADDR		0xF0000020
#define LED_ADDR		0xF00000C0
#define BUTTON_ADDR		0xF00000E0

/*
 * The JTAG registers are arranged in little endian order!
 */
// The data register is implemented via a FIFO and thus has to be read via data word (unsigned integer) access. If only 1 data byte is accessed per read, the other 3 bytes will get lost.
#define JTAG_DATAREG	JTAG_ADDR
// The control register can be read byte wise.
#define JTAG_CONTROLREG	(JTAG_ADDR + 0x4)

#define LED_DATA			(volatile char *) (LED_ADDR)
#define BUTTON_DATA			(volatile char *) (BUTTON_ADDR)

/*
 *  JTAG status bits
 */

// Indicates whether the DATA field is valid. If RVALID=1, the DATA field is valid, otherwise DATA is undefined.
#define JTAG_DATAREG_RVALID	0x80
// Indicates that there has been JTAG activity since the bit was cleared. Writing 1 to AC clears it to 0.
#define JTAG_CONTROLREG_AC	0x4



void main(void)
{
	uint8_t read_b = 0;
	uint16_t read_hw = 0;
	uint32_t read_w = 0;
	uint32_t byte_counter = 0;

	volatile uint32_t * sdram_p;
	sdram_p = SDRAM_ADDR;

	int i = 0;

/*
	// *** Checks if connection to JTAG terminal is established ***
	do {
		read_b = *(volatile uchar8_t *) (JTAG_CONTROLREG + 0x1);
		read_b &= JTAG_CONTROLREG_AC;
	} while (!read_b);
*/

	*LED_DATA = 0x01;


	// *** The following control loop is only valid for big endian systems ***
	for (i = 0; i < 4;) {
		read_hw = *(volatile uint16_t *) JTAG_DATAREG;// only lower 2 bytes of data register are required
		if (read_hw & JTAG_DATAREG_RVALID) {			// check if data is valid
			read_b = (read_hw >> 8);					// extract data byte
			byte_counter = (byte_counter << 8);			// build byte_counter out of 4 following, incoming bytes
			byte_counter |= read_b;
			i++;
		}
	}

	*LED_DATA = 0x03;


/*
	for (i = 0; i < byte_counter;) {
		read_hw = *(volatile ushort16_t *) JTAG_DATAREG;// only lower 2 bytes of data register are required
		if (read_hw & JTAG_DATAREG_RVALID) {			// check if data is valid
			*sdram_p++ = (byte) (read_hw >> 8);			// write byte to SDRAM
			i++;
		}
	}
*/
	for (i = 0; i < byte_counter;) {
		read_hw = *(volatile uint16_t *) JTAG_DATAREG;// only lower 2 bytes of data register are required
		if (read_hw & JTAG_DATAREG_RVALID) {			// check if data is valid
			read_w = (read_w << 8);
			read_w |= (uint8_t) (read_hw >> 8);
			i++;
			if ((i & 0x3) == 0) {						// every fourth loop cycle
				//*(volatile uint32_t *) 0xA0000010 = read_w;
				*sdram_p++ = read_w;					// write word to SDRAM
				read_w = 0;								// clear word to store next four bytes
			}
		}
	}

	*LED_DATA = 0x00;


	*(volatile uint8_t *) JTAG_DATAREG = 0x04;			// terminate JTAG terminal session


	// *** Boot code completed, jump to SDRAM start address ***
	asm (	"l.jr %0"
			:
			:"r" (SDRAM_ADDR)
			 );
	asm ("l.nop");
}
