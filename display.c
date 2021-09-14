/*
 * Handles all operations related to graphics and creating a window
 */

#include <stdio.h>
#include <string.h>
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

    for (int i = 0; i < HEIGHT * SCREEN_SCALE; i++)
        memset(display->screen, 0, (WIDTH * SCREEN_SCALE) * sizeof(int));
}


void
clear_screen(struct display *display)
{
    for (int i = 0; i < HEIGHT * SCREEN_SCALE; i++)
        memset(display->screen, 0, (WIDTH * SCREEN_SCALE) * sizeof(int));

    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
    SDL_RenderClear(display->renderer);
    SDL_RenderPresent(display->renderer);
}

void
draw(struct display *gfx)
{
    int row, col;
    SDL_Rect sprite;

    /* Clear screen before drawing to get rid unused sprites */
    SDL_SetRenderDrawColor(gfx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gfx->renderer);

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++) {
            if (gfx->screen[row][col] == 1) {
                sprite.x = (col % WIDTH) * SCREEN_SCALE;
                sprite.y = (row % HEIGHT) * SCREEN_SCALE;
                sprite.w = 10;
                sprite.h = 10;

                SDL_SetRenderDrawColor(gfx->renderer, 255, 110, 100, 255);
                SDL_RenderFillRect(gfx->renderer, &sprite);
                SDL_RenderDrawRect(gfx->renderer, &sprite);
            }
        }
    }

    SDL_RenderPresent(gfx->renderer);
}

/* Returns true if a bit was flipped from on to off */
bool
detect_collisions(struct display *gfx, int X, int Y, int N, const uint8_t memory[4096], int IR)
{
    int i, j;
    uint8_t sprite;
    bool collision_detected = false;
    X %= WIDTH;
    Y %= HEIGHT;

    for (i = 0; i < N; i++) {
        sprite = memory[IR + i];

        for (j = 0; j < 8; j++) {
            if (gfx->screen[Y + i][X + j] == 1)
                collision_detected = true;

            gfx->screen[Y + i][X + j] ^= (sprite >> (7 - j)) & 0x1;
        }
    }

    return collision_detected;
}


void
destroy_display(struct display *display)
{
    SDL_DestroyWindow(display->window);
    SDL_DestroyRenderer(display->renderer);
    SDL_Quit();
}
