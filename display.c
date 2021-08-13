/*
 * Handles all operations related to graphics and creating a window
 */

#include <string.h>
#include <sys/types.h>
#include "display.h"

void
initialize_graphics(struct display *display)
{
    SDL_Window      *window;
    SDL_Renderer    *renderer;

    window = SDL_CreateWindow(
            "Chip8",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH * SCREEN_SCALE,
            HEIGHT * SCREEN_SCALE,
            0
            );
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (window == NULL || renderer == NULL)
        printf("FATAL, failed to start display: %s\n", SDL_GetError());

    display->window = window;
    display->renderer = renderer;
    memset(display->screen, 0, SCREEN_SIZE * sizeof(u_int16_t));
}

void
destroy_display(struct display *display)
{
    SDL_DestroyWindow(display->window);
    SDL_DestroyRenderer(display->renderer);
}
