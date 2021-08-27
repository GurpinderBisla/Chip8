#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "display.h"

#define GET_VX(op)  (((op) >> 8) & 0xF)
#define GET_VY(op)  (((op) >> 4) & 0xF)
#define GET_NNN(op) ((op) & 0xFFF)
#define GET_NN(op)  ((op) & 0xFF)
#define GET_N(op)   ((op) & 0xF)

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
    // printf("%X\n", opcode);

    switch (opcode >> 12) {
        case 0x0: /* Clear screen or return */
            if (GET_N(opcode) == 0x0)
                clear_screen(gfx);
            else
                cp->pc = cp->stack[0];
            break;
        case 0x1: /* Jump */
            cp->pc = GET_NNN(opcode);
            break;
        case 0x2: /* Call subroutine */
            cp->stack[0] = cp->pc;
            cp->pc = GET_NNN(opcode);
            break;
        case 0x3: /* Skip if Vx equals NN */
            if (cp->registers[GET_VX(opcode)] == GET_NN(opcode))
                cp->pc += 2;
            break;
        case 0x4: /* Skip if Vx does not equal NN */
            if (cp->registers[GET_VX(opcode)] != GET_NN(opcode))
                cp->pc += 2;
            break;
        case 0x5: /* Skip if Vx equals Vy */
            if (cp->registers[GET_VX(opcode)] == cp->registers[GET_VY(opcode)])
                cp->pc += 2;
            break;
        case 0x6: /* Set VX */
            cp->registers[GET_VX(opcode)] = GET_NN(opcode);
            break;
        case 0x7: /* Add value to VX */
            cp->registers[GET_VX(opcode)] += GET_NN(opcode);
            break;
        case 0x8: /* Logical and arithmetic instructions */
            switch (GET_N(opcode)) {
                case 0x0: /* Set Vx to Vy */
                    cp->registers[GET_VX(opcode)] = cp->registers[GET_VY(opcode)];
                    break;
                case 0x1: /* Set Vx to the OR of Vy */
                    cp->registers[GET_VX(opcode)] |= cp->registers[GET_VY(opcode)];
                    break;
                case 0x2: /* Set Vx to the AND of Vy */
                    cp->registers[GET_VX(opcode)] &= cp->registers[GET_VY(opcode)];
                    break;
                case 0x3: /* Set Vx to the XOR of Vy */
                    cp->registers[GET_VX(opcode)] ^= cp->registers[GET_VY(opcode)];
                    break;
                case 0x4:
                    if (cp->registers[GET_VX(opcode)] + cp->registers[GET_VY(opcode)] < cp->registers[GET_VX(opcode)])
                        cp->registers[0xF] = 1;
                    else
                        cp->registers[0xF] = 0;

                    cp->registers[GET_VX(opcode)] += cp->registers[GET_VY(opcode)];
                    break;
            }
            break;
        case 0x9: /* skip if Vx does not equal Vy*/
            if (cp->registers[GET_VX(opcode)] != cp->registers[GET_VY(opcode)])
                cp->pc += 2;
            break;
        case 0xA: /* Set IR */
            cp->IR = GET_NNN(opcode);
            break;
        case 0xD: /* Draw to screen */
            cp->registers[0xF] = 0;
            if (detect_collisions(gfx,
                                  cp->registers[GET_VX(opcode)],
                                  cp->registers[GET_VY(opcode)],
                                  GET_N(opcode),
                                  cp->memory, cp->IR)) {
               cp->registers[0xF] = 1;
            }
            draw(gfx);
            break;
        default:
            printf("opcode:%X not yet implemented\n", opcode);
            break;
    }
}
