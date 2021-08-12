#include "cpu.h"
#include "string.h"

void
cpu_initilize(struct cpu *cp)
{
    cp->pc = 0x200; /* The first 516 bytes of memory are reserved */
    cp->IR = 0;
    cp->opcode = 0;

    memset(cp->memory, 0, 4096);
    memset(cp->registers, 0, 16);
    memset(cp->stack, 0, 16);
}
