/* cpu.h: types and definitions for a cpu emulator */
/*
 * Created by Gurpinder Bisla on 2021-08-08.
 */

#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "display.h"

struct cpu {
    uint16_t pc;            /* Address of the current instruction */
    uint16_t IR;            /* Index register */
    uint16_t stack[16];     /* Chip8 stack represented as an array */
    uint8_t keyboard[16];   /* Holds the state of the 16 keyboard keys */
    uint8_t memory[4096];   /* Total memory */
    uint8_t registers[16];  /* registers V0 to VF */
    uint8_t delay_timer;
    uint8_t sound_timer;
};

/* Variable definitions */
/* sets everything to correct starting values */
void cpu_initilize(struct cpu *cp);
/* Fetch and execute the instructions of a single opcode */
void execute_cpu_cycle(struct cpu *cp, struct display *gfx);

#endif //CPU_H
