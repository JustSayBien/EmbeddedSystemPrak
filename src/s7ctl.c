/**
 * $Id: s7ctl.c 672 2013-04-12 10:30:44Z klugeflo $
 * 7 segment LED controller
 */


#include <s7ctl.h>

#ifdef HAVE_S7CTL


void inits7ctl(void) {
  IOWR32(A_S7CTL, S7_CTL, S7_CTL_LUT);
}

#endif /* HAVE_S7CTL */
