#include <stddef.h>
//#include <osek/osservices.h>
//#include <spr-defs.h>
//#include <intrinsics.h>
#include <output.h>
#include <asm/nios2intrinsics.h>
#include <asm/ptrace.h>

void handle_ge(uint32_t cause, sc_stackframe_t *sf) {
    switch (cause) {
        case 2: // Internal Interrupt
        case 3: // Trap
                // both handled in exception.S
            break;
        case 6:
        case 7:
            uart_printf("badaddr: %x\n", __rdctl_badaddr());
            while (1);
            break;
        // all other exceptions
        default:
            uart_printf("general exception %d\n", cause);
            break;
    }
}
