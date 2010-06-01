#include "types.h"
#include "program_mem.h"

uint8_t getFarU8(dj_di_pointer pointer)
{
        /* Some assembly to read memory above 64k.
         * Note: interrupthandlers that do not save the extended part (bits
         * 16..19) of the registers, but do use extended instructions might
         * corrupt the extended part of the registers. Therefore interrupts are
         * disabled during a flash read.
         */
        uint8_t retval;
        asm("push.w r2\n"\
            "mov.w %1,r14\n"\
            "mov.w #0x1, r15\n"\
            "push.w r15\n"\
            "push.w r14\n"\
            "dint\n"\
            "popx.a r14\n"\
            "movx.b 0x0(r14), %0\n"\
            "pop.w r2\n"\
             : [res] "=r" (retval):[input]"r" (pointer):"r14", "r15");
        return retval;
}
