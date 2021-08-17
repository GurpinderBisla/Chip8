#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL_timer.h>
#include "cpu.h"
#include "display.h"

const uint8_t FONT[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct display  gfx;
struct cpu      chip8;

static void load_font();
static bool load_rom(char *filename);

int
main(int argc, char *argv[])
{
    if (argc != 2) {
      printf("Usage: ./chip8 path-to-rom");
      return 1;
    }

    initialize_graphics(&gfx);
    cpu_initilize(&chip8);
    load_font();

    if (!load_rom(argv[1])) {
        printf("Failed to load rom\n");
        return 1;
    }

    for (int i = 0; i < 2; i++) {
        execute_cpu_cycle(&chip8, &gfx);
    }

    SDL_Delay(3000);
    destroy_display(&gfx);
    SDL_Quit();
}

static void
load_font()
{
    int length =  sizeof(FONT) / sizeof(FONT[0]);

    /* Load font into memory blocks 0x50 to 0x9F */
    for (int i = 0; i < length; i++)
        chip8.memory[0x50 + i] = FONT[i];
}

/* Load roam into memory, starting at memory block 0x200 */
static bool
load_rom(char *filename)
{
    FILE *rom;
    long len;

    rom = fopen(filename, "r");
    if (rom == NULL)
        return false;

    fseek(rom, 0, SEEK_END);
    len = ftell(rom);
    fseek(rom, 0, SEEK_SET);
    fread(&chip8.memory[0x200], sizeof(uint8_t), len, rom);

    fclose(rom);
    return true;
}
