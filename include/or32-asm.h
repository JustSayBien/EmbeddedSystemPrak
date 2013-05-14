/**
 * OR32 assembler macros
 *
 * $Id: or32-asm.h 467 2011-02-21 12:49:09Z klugeflo $
 */

#define LOAD_SYMBOL_2_GPR(gpr,symbol)   \
    l.movhi gpr, hi(symbol)              ;\
    l.ori   gpr, gpr, lo(symbol);

