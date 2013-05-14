/**
 * $Id: gprintf.c 672 2013-04-12 10:30:44Z klugeflo $
 */

/******************************************************************************

File: gprintf.c

Project: Roomba Embedded Systems Training

Description: simple printf (based on printf code found somewhere on the web)

Author: Florian Kluge <kluge@informatik.uni-augsburg.de>
        Universität Augsburg

Created: 21.02.2011

*******************************************************************************

Modification history:
---------------------
21.02.2011 (FAK) Created from RTOS Training

*/

/****************************************************************** Includes */

#include <stdarg.h>
#include <stdint.h>

#include <gprintf.h>

/******************************************************************* Defines */

#define PAD_RIGHT 1
#define PAD_ZERO 2
/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12
#define LPRINT_BUF_LEN 24

/******************************************************* Function prototypes */

static int gprints(int32_t (*putchar_fn)(int32_t),
			  const char *string, int width, int pad);

static int gprinti(int32_t (*putchar_fn)(int32_t),
			  int i, int b, int sg,
			  int width, int pad, int letbase);

static int gprintl(int32_t (*putchar_fn)(int32_t),
			  long long i, int b, int sg,
			  int width, int pad, int letbase);

static int gprint(int32_t (*putchar_fn)(int32_t),
			 const char *format, va_list args );


/************************************************************** Global const */


/********************************************************** Global variables */


/*************************************************************** Local const */


/*********************************************************** Local variables */


/******************************************************************** Macros */


/********************************************************** Global functions */

int32_t gputs(int32_t (*putchar_fn)(int32_t), const char *p) {
  return gprintf(putchar_fn, p);
}

int32_t gprintf(int32_t (*putchar_fn)(int32_t), const char *format, ...) {
  va_list args;
  va_start( args, format );
  return gprint(putchar_fn, format, args );
}

/*
int sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start( args, format);
  return gprint(0, &str, format, args);
}
*/

/*********************************************************** Local functions */

// usually provided by compiler/linux kernel/etc..
//extern uint32_t __div64_32(uint64_t *dividend, uint32_t divisor);
 
static int gprints(int32_t (*putchar_fn)(int32_t), const char *string, int width, int pad)
{
  register int pc = 0, padchar = ' ';
  
  if (width > 0) {
    register int len = 0;
    register const char *ptr;
    for (ptr = string; *ptr; ++ptr) ++len;
    if (len >= width) width = 0;
    else width -= len;
    if (pad & PAD_ZERO) padchar = '0';
  }
  if (!(pad & PAD_RIGHT)) {
    for ( ; width > 0; --width) {
      putchar_fn(padchar);
      ++pc;
    }
  }
  for ( ; *string ; ++string) {
    putchar_fn(*string);
    ++pc;
  }
  for ( ; width > 0; --width) {
    putchar_fn(padchar);
    ++pc;
  }
  
  return pc;
}


static int gprinti(int32_t (*putchar_fn)(int32_t), int i, int b, int sg,
		   int width, int pad, int letbase)
{
  char print_buf[PRINT_BUF_LEN];
  register char *s;
  register int t, neg = 0, pc = 0;
  register unsigned int u = i;
  
  if (i == 0) {
    print_buf[0] = '0';
    print_buf[1] = '\0';
    return gprints (putchar_fn, print_buf, width, pad);
  }
  
  if (sg && b == 10 && i < 0) {
    neg = 1;
    u = -i;
	}
  
  s = print_buf + PRINT_BUF_LEN-1;
  *s = '\0';
  
  while (u) {
    t = u % b;
    if( t >= 10 )
      t += letbase - '0' - 10;
    *--s = t + '0';
    u /= b;
  }
  
  if (neg) {
    if( width && (pad & PAD_ZERO) ) {
      putchar_fn('-');
      ++pc;
      --width;
    }
    else {
      *--s = '-';
    }
  }
  
  return pc + gprints (putchar_fn, s, width, pad);
}


static int gprintl(int32_t (*putchar_fn)(int32_t), long long i, int b,
		   int sg, int width, int pad, int letbase)
{
  char print_buf[LPRINT_BUF_LEN];
  register char *s;
  register long long t, neg = 0;
  register int pc = 0;
  register unsigned long long u = i;
  
  if (i == 0) {
    print_buf[0] = '0';
    print_buf[1] = '\0';
    return gprints (putchar_fn, print_buf, width, pad);
  }
  
  if (sg && b == 10 && i < 0) {
    neg = 1;
    u = -i;
  }
  
  s = print_buf + LPRINT_BUF_LEN-1;
  *s = '\0';
  
  while (u) {
    t = u % b;
    if( t >= 10 )
      t += letbase - '0' - 10;
    *--s = t + '0';
    u /= b;
  }
  
  if (neg) {
    if( width && (pad & PAD_ZERO) ) {
      putchar_fn('-');
      ++pc;
      --width;
    }
    else {
      *--s = '-';
    }
  }
  
  return pc + gprints (putchar_fn, s, width, pad);
}


static int gprint(int32_t (*putchar_fn)(int32_t), const char *format,
		  va_list args ) {
  register int width, pad;
  register int pc = 0;
  char scr[2];
  
  for (; *format != 0; ++format) {
    if (*format == '%') {
      ++format;
      width = pad = 0;
      if (*format == '\0') break;
      if (*format == '%') goto out;
      if (*format == '-') {
	++format;
	pad = PAD_RIGHT;
      }
      while (*format == '0') {
	++format;
	pad |= PAD_ZERO;
      }
      for ( ; *format >= '0' && *format <= '9'; ++format) {
	width *= 10;
	width += *format - '0';
      }
      if( *format == 's' ) {
	register char *s = (char *)va_arg( args, int );
	pc += gprints (putchar_fn, s?s:"(null)", width, pad);
	continue;
      }
      if( *format == 'd' ) {
	pc += gprinti (putchar_fn, va_arg( args, int ), 10, 1, width, pad, 'a');
	continue;
      }
      if( *format == 'i' ) {
	pc += gprinti (putchar_fn, va_arg( args, int ), 10, 1, width, pad, 'a');
	continue;
      }
      if (*format == 'p') {
	pc += gprints (putchar_fn, "0x", width, pad);
	pc += gprinti (putchar_fn, va_arg( args, int ), 16, 0, width, pad, 'a');
	continue;
      }
      if (*format == 'P') {
	pc += gprints (putchar_fn, "0x", width, pad);
	pc += gprinti (putchar_fn, va_arg( args, int ), 16, 0, width, pad, 'A');
	continue;
      }
      if( *format == 'x' || *format == 'p' ) {
	pc += gprinti (putchar_fn, va_arg( args, int ), 16, 0, width, pad, 'a');
	continue;
      }
      if( *format == 'X' ) {
	pc += gprinti (putchar_fn, va_arg( args, int ), 16, 0, width, pad, 'A');
	continue;
      }
      if( *format == 'u' ) {
	pc += gprinti (putchar_fn, va_arg( args, int ), 10, 0, width, pad, 'a');
	continue;
      }
      if( *format == 'c' ) {
	/* char are converted to int then pushed on the stack */
	scr[0] = (char)va_arg( args, int );
	scr[1] = '\0';
	pc += gprints (putchar_fn, scr, width, pad);
	continue;
      }
      if (*format == 'l') {
	++format;
	if (*format == 'l') {
	  ++format;
	  if( *format == 'd' ) {
	    pc += gprintl (putchar_fn, va_arg( args, long long ), 10, 1, width, pad, 'a');
	  continue;
	  }
	  if( *format == 'x' || *format == 'p' ) {
	    pc += gprintl (putchar_fn, va_arg( args, long long ), 16, 0, width, pad, 'a');
	    continue;
	  }
	  if( *format == 'X' ) {
	    pc += gprintl (putchar_fn, va_arg( args, long long ), 16, 0, width, pad, 'A');
	    continue;
	  }
	  if( *format == 'u' ) {
	    pc += gprintl (putchar_fn, va_arg( args, long long ), 10, 0, width, pad, 'a');
	    continue;
	  }
	}
	else {
	  ++format;
	  if( *format == 'd' ) {
	    pc += gprintl (putchar_fn, va_arg( args, int ), 10, 1, width, pad, 'a');
	  continue;
	  }
	  if( *format == 'x' || *format == 'p' ) {
	    pc += gprintl (putchar_fn, va_arg( args, int ), 16, 0, width, pad, 'a');
	    continue;
	  }
	  if( *format == 'X' ) {
	    pc += gprintl (putchar_fn, va_arg( args, int ), 16, 0, width, pad, 'A');
	    continue;
	  }
	  if( *format == 'u' ) {
	    pc += gprintl (putchar_fn, va_arg( args, int ), 10, 0, width, pad, 'a');
	    continue;
	  }
	}
      }
    }
    else {
    out:
      putchar_fn(*format);
      ++pc;
    }
  }
  //if (out) **out = '\0';
  va_end( args );
  return pc;
}



