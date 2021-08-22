/*
 * Structs and defs related to rendering the graphics for a chip8 emulator
 */

#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H
#include "SDL2/SDL.h"

/* All constants related to rendering the screen */
#define WIDTH 64
#define HEIGHT 32
#define SCREEN_SCALE 10
#define SCREEN_SIZE (WIDTH * SCREEN_SCALE) * (HEIGHT * SCREEN_SCALE)

/* Struct that represents the display itself */
struct display {
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    int             screen[HEIGHT * SCREEN_SCALE][WIDTH * SCREEN_SCALE];
};

/* Variable declarations */
/* create a window and renderer and sets the screen */
void initialize_graphics(struct display *display);
/* Free all memory related to the display */
void destroy_display(struct display *display);
/* sets all elements in screen array to 0 */
void clear_screen(struct display *display);
/* Draws a sprite of N height at a given x and y coordinate*/
void draw(struct display *gfx, int Vx, int Vy, int N);


#endif //CHIP8_DISPLAY_H
