/*
 * Handles all operations related to graphics and creating a window
 */

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
draw(struct display *gfx, int Vx, int Vy, int N)
{
    //printf("%d:%d/n", Vx, Vy);
    Vx = Vx % (WIDTH * SCREEN_SCALE);
    Vy = Vy % (HEIGHT * SCREEN_SCALE);

    SDL_Rect sprite;
    sprite.x = Vx;
    sprite.y = Vy;
    sprite.w = 8;
    sprite.h = N;

    printf("rect: %d, %d, %d, %d\n", sprite.x,sprite.y,sprite.w,sprite.h);
    SDL_SetRenderDrawColor(gfx->renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(gfx->renderer, &sprite);
    SDL_RenderPresent(gfx->renderer);
}

/* Returns true if a bit was flipped from on to off */
bool
detect_collisions(struct display *gfx, int X, int Y, int N)
{
    int i, j;
    bool collision_detected = false;
    X = X % WIDTH;
    Y = Y % HEIGHT;

    for (i = 0; i < N; i++) {
      for (j = 0; j < 8; j++) {
          if (gfx->screen[Y + i][X + j] == 0) {
              gfx->screen[Y + 1][X + j] = 1;
          } else {
              gfx->screen[Y + i][X + j] = 0;
              collision_detected = true;
          }
      }
    }

    return collision_detected;
}


void
destroy_display(struct display *display)
{
    SDL_DestroyWindow(display->window);
    SDL_DestroyRenderer(display->renderer);
}
