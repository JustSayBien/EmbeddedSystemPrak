/**
 * $Id: lcd.c 672 2013-04-12 10:30:44Z klugeflo $
 */

/******************************************************************************

File: lcd.c

Project: Roomba Embedded Systems Training

Description: LCD text output

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training

*/


/****************************************************************** Includes */

#include <lcd.h>

#include <board.h>
#include <tools.h>

#include <stddef.h>

#ifdef HAVE_LCD

/******************************************************************* Defines */

#define CHAR_SPACE 0x20

#define NUM_TABCHARS 4
#define TABCHAR      CHAR_SPACE

#define LCD_LINES 2    /*!< number of lines the display provides */
#define LCD_LEN   0x10 /*!< chars per line */

#define LCD_BUSY    (0x00000080u)

  // LCD commands from ALTERA file

enum /* Write to character RAM */
{
  LCD_CMD_WRITE_DATA    = 0x80
  /* Bits 6:0 hold character RAM address */
};

enum /* Write to character generator RAM */
{
  LCD_CMD_WRITE_CGR     = 0x40
  /* Bits 5:0 hold character generator RAM address */
};

enum /* Function Set command */
{
  LCD_CMD_FUNCTION_SET  = 0x20,
  LCD_CMD_8BIT          = 0x10,
  LCD_CMD_TWO_LINE      = 0x08,
  LCD_CMD_BIGFONT       = 0x04
};

enum /* Shift command */
{
  LCD_CMD_SHIFT         = 0x10,
  LCD_CMD_SHIFT_DISPLAY = 0x08,
  LCD_CMD_SHIFT_RIGHT   = 0x04
};

enum /* On/Off command */
{
  LCD_CMD_ONOFF         = 0x08,
  LCD_CMD_ENABLE_DISP   = 0x04,
  LCD_CMD_ENABLE_CURSOR = 0x02,
  LCD_CMD_ENABLE_BLINK  = 0x01
};

enum /* Entry Mode command */
{
  LCD_CMD_MODES         = 0x04,
  LCD_CMD_MODE_INC      = 0x02,
  LCD_CMD_MODE_SHIFT    = 0x01
};

enum /* Home command */
{
  LCD_CMD_HOME          = 0x02
};

enum /* Clear command */
{
  LCD_CMD_CLEAR         = 0x01
};


/******************************************************* Function prototypes */

/*!
  Select the line, where you want your output to appear
  \param l &lt; #LCD_LINES
*/
int32_t lcd_setline(uint32_t l);

/*!
  Move the cursor to the first position in line
*/
void lcd_home(void);

/*!
  Simply write one char onto the display. Will NOT check for newlines,
  overflows and similar. Just writes on and on and......
*/
int32_t __lcd_putchar(int32_t c);

/*!
  Initialise buffered output
*/
void lcd_init_buf(void);


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */

static char lcd_line_adr[2] = { 0x00, 0x40 }; /*!< line adresses */


// handle a buffer to write to LCD
static int8_t lcd_pos = 0; /*!< Current position in line */
static int8_t lcd_cline = 0; /*!< Current line */
static int8_t lcd_line_buf[LCD_LINES][LCD_LEN]; /*!< Lines Buffer; array
						  for faster buffer handling */


/******************************************************************** Macros */


/********************************************************** Global functions */

void lcd_init(void) {
  // enable LCD
  LCD_ON = 0x1;

  // Wait for 15 ms then reset
  my_usleep(15000);
  LCD_CMD = LCD_CMD_FUNCTION_SET | LCD_CMD_8BIT;

  // Wait for another 4.1ms and reset again
  my_usleep(4100);  
  LCD_CMD = LCD_CMD_FUNCTION_SET | LCD_CMD_8BIT;

  // Wait a further 1 ms and reset a third time
  my_usleep(1000);
  LCD_CMD = LCD_CMD_FUNCTION_SET | LCD_CMD_8BIT;

  // Setup interface parameters: 8 bit bus, 2 rows, 5x7 font
  while(LCD_ST == LCD_BUSY){}
  my_usleep(1000);
  LCD_CMD = LCD_CMD_FUNCTION_SET | LCD_CMD_8BIT | LCD_CMD_TWO_LINE;

  // Turn display off
  while(LCD_ST == LCD_BUSY);
  my_usleep(1000);
  LCD_CMD = LCD_CMD_ONOFF;

  // Clear display
  while(LCD_ST == LCD_BUSY);
  my_usleep(1000);
  LCD_CMD = LCD_CMD_CLEAR;

  // Set mode: increment after writing, don't shift display
  while(LCD_ST == LCD_BUSY);
  my_usleep(1000);
  LCD_CMD = LCD_CMD_MODES | LCD_CMD_MODE_INC;

  // Turn display on
  while(LCD_ST == LCD_BUSY);
  my_usleep(1000);
  //LCD_CMD = LCD_CMD_ONOFF | LCD_CMD_ENABLE_DISP | LCD_CMD_ENABLE_CURSOR | LCD_CMD_ENABLE_BLINK;
  LCD_CMD = LCD_CMD_ONOFF | LCD_CMD_ENABLE_DISP | LCD_CMD_ENABLE_BLINK;

  lcd_init_buf();
}


int32_t lcd_putchar(int32_t c) {
  uint32_t i;
  uint32_t savepos;

  // newline/line full
  // if line was full, an immediate newline is automatically ignored!
  if ( (lcd_pos & 0x10) || (c == '\n') || (c == '\r') || (c == '\f') ) {
    savepos = lcd_pos;
    lcd_pos = 0;
    if (lcd_cline) {
      lcd_clear();
      lcd_home();
      lcd_setline(0);
      for (i = 0; i < LCD_LEN; i++){
	lcd_line_buf[0][i] = lcd_line_buf[1][i];
	__lcd_putchar(lcd_line_buf[0][i]);
	lcd_line_buf[1][i] = CHAR_SPACE;
      }
    }
    lcd_cline = 1;
    lcd_setline(lcd_cline);
    if ((c == '\f')) { // set new position
      lcd_pos = savepos;
      for (i=0; i<savepos; ++i) {
	__lcd_putchar(lcd_line_buf[1][i]);
      }
    }
  }

  if (c == '\b') { // backspace
    --lcd_pos;
    lcd_line_buf[lcd_cline][lcd_pos] = CHAR_SPACE;

    // rewrite display
    lcd_clear();
    lcd_home();
    lcd_setline(0);
    for (i=0; i<(lcd_cline?LCD_LEN:lcd_pos); ++i) {
      __lcd_putchar(lcd_line_buf[0][i]);
    }
    if (lcd_cline) {
      lcd_setline(1);
      for (i=0; i<lcd_pos; ++i) {
	__lcd_putchar(lcd_line_buf[1][i]);
      }
    }
  }
  else if (c == '\f') {
    // do nothing (already done above)
  }
  else if (c == '\n') {
    // do nothing (already done above)
  }
  else if (c == '\r') {
    // do nothing (already done above)
  }
  else if (c == '\t') { // tab
    int i = 0;
    for (i = 0; i < NUM_TABCHARS; i++) __lcd_putchar(TABCHAR);
  }
  else { // just a char
    __lcd_putchar(c);
    lcd_line_buf[lcd_cline][lcd_pos] = c;
    ++lcd_pos;
  }

  return (uint8_t)c;
}


int32_t lcd_puts(char * p) {
  uint32_t ctr = 0;
  if (p == NULL) {
    return 0;
  }
  do {
    lcd_putchar(*p);
    ++p;
    ++ctr;
  } while (*p);
  
  return ctr;
}


void lcd_clear(void) {
  // Clear display
  while(LCD_ST == LCD_BUSY);
  my_usleep(1000);
  LCD_CMD = LCD_CMD_CLEAR;
}


/*********************************************************** Local functions */

void lcd_init_buf(void) {
  lcd_clear();
  lcd_home();
  lcd_cline = 0;
  lcd_pos = 0;
  lcd_setline(0);
  uint32_t i;
  for (i = 0; i < LCD_LEN; i++){
    lcd_line_buf[0][i] = CHAR_SPACE;
    lcd_line_buf[1][i] = CHAR_SPACE;
  }
}


int32_t lcd_setline(uint32_t l) {
  if (l >= LCD_LINES) {
    return -1;
  }
  else {
    while(LCD_ST == LCD_BUSY);
    my_usleep(1000);
    LCD_CMD = 0x80 | lcd_line_adr[l];
    return 0;
  }
}


void lcd_home(void) {
  while(LCD_ST == LCD_BUSY);
  my_usleep(1000);
  LCD_CMD = LCD_CMD_HOME;
}


int32_t __lcd_putchar(int32_t c) {
  while(LCD_ST == LCD_BUSY);
  my_usleep(1000);
  LCD_WR = (uint8_t)c;
  return (uint8_t)c;
}

#endif /* HAVE_LCD */
