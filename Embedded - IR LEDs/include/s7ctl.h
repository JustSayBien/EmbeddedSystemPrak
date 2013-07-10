/**
 * $Id: s7ctl.h 672 2013-04-12 10:30:44Z klugeflo $
 * 7 segment LED controller
 */

#ifndef DRIVER_S7CTL_H
#define DRIVER_S7CTL_H 1

#ifdef HAVE_S7CTL

#define S7_CTL 0
#define S7_NAT0 0x10
#define S7_NAT1 0x11
#define S7_NAT2 0x12
#define S7_NAT3 0x13
#define S7_NAT4 0x14
#define S7_NAT5 0x15
#define S7_NAT6 0x16
#define S7_NAT7 0x17

#define S7_LUT  0x18
#define S7_LUT0 0x18
#define S7_LUT1 0x19
#define S7_LUT2 0x1a
#define S7_LUT3 0x1b

#define S7_DP   0x1c



// Control register:
// =================
//
// | 31..1    |  0  |
// | reserved | LUT |
// LUT = 0: use direct control
// LUT = 1: use LUT (default)

#define S7_CTL_LUT 0x00000001
#define S7_CTL_NAT 0x00000000


// Direct control:
// ===============
//
// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
// | DP | T  | RT | RB | B  | LB | LT | M  |
//
//     ---t----
//     |  6   |
//   1 lt    rt 5
//     |      |
//     ---m----
//     |  0   |
//   2 lb    rb 4
//     |  3   |
//     ---b---- .dp 7

#define S7_NAT_M  (~0x01)
#define S7_NAT_LT (~0x02)
#define S7_NAT_LB (~0x04)
#define S7_NAT_B  (~0x08)
#define S7_NAT_RB (~0x10)
#define S7_NAT_RT (~0x20)
#define S7_NAT_T  (~0x40)
#define S7_NAT_DP (~0x80)



void inits7ctl(void);


#endif /* HAVE_S7CTL */

#endif /* DRIVER_S7CTL_H */

