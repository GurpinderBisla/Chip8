#include <stdio.h>
#include <stdbool.h>

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

const SDL_KeyCode keyboard_keys[] = {
        SDLK_1, SDLK_2, SDLK_3, SDLK_4,
        SDLK_q, SDLK_w, SDLK_e, SDLK_r,
        SDLK_a, SDLK_s, SDLK_d, SDLK_f,
        SDLK_z, SDLK_x, SDLK_c, SDLK_v,
};

struct display  gfx;
struct cpu      chip8;

static void load_font();
static bool load_rom(char *filename);
static void set_key_on(SDL_KeyCode target);

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

    SDL_Event e;
    bool quit = false;
    while (!quit){
        execute_cpu_cycle(&chip8, &gfx);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                destroy_display(&gfx);
                quit = true;
            }
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                destroy_display(&gfx);
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                set_key_on(e.key.keysym.sym);
            }
            if (e.type == SDL_KEYUP) {
                for (int i = 0; i < 0xF; i++)
                    chip8.keyboard[i] = 0;
            }
        }
    }
}

/* Load font into memory blocks 0x50 to 0x9F */
static void
load_font()
{
    int length =  sizeof(FONT) / sizeof(FONT[0]);
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

static void
set_key_on(SDL_KeyCode target)
{
    int i;
    for (i = 0; i < 0xf; i++) {
        if (target == keyboard_keys[i])
            chip8.keyboard[i] = 1;
    }
}
