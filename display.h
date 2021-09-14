/*
 * Structs and defs related to rendering the graphics for a chip8 emulator
 */

#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H
#include "SDL.h"
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

/* All constants related to rendering the screen */
#define WIDTH 64
#define HEIGHT 32
#define SCREEN_SCALE 10

/* Struct that represents the display itself */
struct display {
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    int             screen[HEIGHT][WIDTH];
};

/* Variable declarations */
/* create a window and renderer and sets the screen */
void initialize_graphics(struct display *display);
/* Free all memory related to the display */
void destroy_display(struct display *display);
/* Sets all elements in screen array to 0 */
void clear_screen(struct display *display);
/* Detects if a collision was made in game */
bool detect_collisions(struct display *gfx, int X, int Y, int N, const uint8_t memory[4096], int IR);
/* Draws a sprite of N height at a given x and y coordinate*/
void draw(struct display *gfx);


#endif //CHIP8_DISPLAY_H
