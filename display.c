/*
 * Handles all operations related to graphics and creating a window
 */

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#include "display.h"

static bool clear_screen_array(struct display *gfx, int X, int Y, int N);

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
draw(struct display *gfx, int Vx, int Vy, int N, uint8_t *draw_flag)
{
    Vx = Vx % (WIDTH * SCREEN_SCALE);
    Vy = Vy % (HEIGHT * SCREEN_SCALE);
    draw_flag = 0;

    if (clear_screen_array(gfx, Vx, Vy, N))
        draw_flag = (uint8_t *) 1;
}

/* Returns true if a bit was flipped from on to off */
static bool
clear_screen_array(struct display *gfx, int X, int Y, int N)
{
    int i, j;
    bool bit_was_flipped = false;

    for (i = 0; i < N; i++) {
      for (j = 0; j < 8 * SCREEN_SCALE; j++) {
          if (gfx->screen[Y + i][X + j] == 0) {
              gfx->screen[Y + 1][X + j] = 1;
          } else {
              gfx->screen[Y + i][X + j] = 0;
              bit_was_flipped = true;
          }
      }
    }

    return bit_was_flipped;
}


void
destroy_display(struct display *display)
{
    SDL_DestroyWindow(display->window);
    SDL_DestroyRenderer(display->renderer);
}
