/**
 * OR32 assembler macros
 *
 * $Id: or32-asm.h 672 2013-04-12 10:30:44Z klugeflo $
 */

#define LOAD_SYMBOL_2_GPR(gpr,symbol)   \
    l.movhi gpr, hi(symbol)              ;\
    l.ori   gpr, gpr, lo(symbol);

