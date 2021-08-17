/*
 * Handles all operations related to graphics and creating a window
 */

#include <SDL2/SDL_render.h>
#include <string.h>
#include <sys/types.h>
#include "display.h"

static void clear_screen_array(struct display *gfx, int X, int Y);

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
    memset(display->screen, 0, SCREEN_SIZE * sizeof(int));
}


void
clear_screen(struct display *display)
{
    memset(display->screen, 0, SCREEN_SIZE * sizeof(int));

    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
    SDL_RenderClear(display->renderer);
    SDL_RenderPresent(display->renderer);
}

void
draw(struct display *gfx, int Vx, int Vy, int N)
{
    clear_screen_array(gfx, Vx, Vy);
}

static void
clear_screen_array(struct display *gfx, int X, int Y)
{

}


void
destroy_display(struct display *display)
{
    SDL_DestroyWindow(display->window);
    SDL_DestroyRenderer(display->renderer);
}
