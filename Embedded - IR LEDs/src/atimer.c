/**
 * $Id: atimer.c 672 2013-04-12 10:30:44Z klugeflo $
 * Timer handling
 */

#if 0

#include <stdint.h>
#include <spr-defs.h>
#include <driver/board.h>
#include <driver/timer.h>

void timer_periodic(uint32_t period) {
  IOWR32(A_TIMER, TIMER_PERIODL, period & 0xffff);
  IOWR32(A_TIMER, TIMER_PERIODH, period >> 16);
  IOWR32(A_TIMER, TIMER_CONTROL,
	 TIMER_CONTROL_ITO | TIMER_CONTROL_CONT | TIMER_CONTROL_START);
}


void timer_single(uint32_t period) {
  IOWR32(A_TIMER, TIMER_PERIODL, period & 0xffff);
  IOWR32(A_TIMER, TIMER_PERIODH, period >> 16);
  IOWR32(A_TIMER, TIMER_CONTROL, TIMER_CONTROL_ITO |
	 TIMER_CONTROL_START);
}


void timer_cb(void) {
  IOWR32(A_TIMER, TIMER_CONTROL, TIMER_CONTROL_STOP);
}

#endif /* 0 */
