#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "display.h"

#define GET_VX(op)  ((op >> 8) & 0xF)
#define GET_VY(op)  ((op >> 4) & 0xF)
#define GET_NNN(op) (op & 0xFFF)
#define GET_NN(op)  (op & 0xFF)
#define GET_N(op)   (op & 0xF)

static uint16_t fetch(struct cpu *cp);

void
cpu_initilize(struct cpu *cp)
{
    cp->pc = 0x200; /* The first 516 bytes of memory are reserved */
    cp->IR = 0;
    //cp->opcode = 0;

    memset(cp->memory, 0, 4096);
    memset(cp->registers, 0, 16);
    memset(cp->stack, 0, 16 * sizeof(uint16_t));
}

/* creates an opcode by combing the next 2 bits pointed to by the pc, then points the pc at the next opcode */
static uint16_t
fetch(struct cpu *cp)
{
    uint16_t opcode;
    opcode = (cp->memory[cp->pc] << 8) | cp->memory[cp->pc + 1];

    cp->pc += 2;
    return opcode;
}

void
execute_cpu_cycle(struct cpu *cp, struct display *gfx)
{
    uint16_t opcode;
    opcode = fetch(cp);
    printf("%X\n", opcode);

    switch (opcode >> 12) {
        case 0x0000: /* Clear screen */
            clear_screen(gfx);
            break;
        case 0x0001: /* Jump */
            cp->pc = GET_NNN(opcode);
            break;
        case 0x0006: /* Set VX */
            cp->registers[GET_VX(opcode)] = GET_NN(opcode);
            break;
        case 0x0007: /* Add value to VX */
            printf("new Vx:%d->%d\n", cp->registers[GET_VX(opcode)], GET_NN(opcode));
            cp->registers[GET_VX(opcode)] += GET_NN(opcode);
            printf("new Vx:%d->%d\n", cp->registers[GET_VX(opcode)], GET_NN(opcode));
            break;
        case 0x000A: /* Set IR */
            cp->IR = GET_NNN(opcode);
            break;
        case 0x000D: /* Draw to screen */
            cp->registers[0xF] = 0;
            if (detect_collisions(gfx, cp->registers[GET_VX(opcode)], cp->registers[GET_VY(opcode)], GET_N(opcode))) {
               cp->registers[0xF] = 1;
               draw(gfx, cp->registers[GET_VX(opcode)], cp->registers[GET_VY(opcode)], GET_N(opcode));
            }
               draw(gfx, cp->registers[GET_VX(opcode)], cp->registers[GET_VY(opcode)], GET_N(opcode));

            break;
        default:
            printf("opcode:%X not yet implemented\n", opcode);
            break;
    }
}
