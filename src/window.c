#include "window.h"

#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static struct window_config window_config;

bool window_init(const struct window_config* config)
{
    bool success = true;

    window_config = *config;

    int window_w = DISPLAY_W * window_config.scale;
    int window_h = DISPLAY_H * window_config.scale;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "E: SDL video could not initialize: %s\n", SDL_GetError());
        success = false;
    }

    if (success) {
        window = SDL_CreateWindow(
            "Yet Another CHIP-8",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_w,
            window_h,
            SDL_WINDOW_SHOWN);

        if (window == NULL) {
            fprintf(stderr, "E: window could not be created: %s\n", SDL_GetError());
            success = false;
        }
    }

    if (success) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (renderer == NULL) {
            fprintf(stderr, "E: renderer could not be created: %s\n", SDL_GetError());
            success = false;
        }
    }

    if (!success) {
        window_destroy();
    }

    return success;
}

void window_handle_events(enum interpreter_state* state)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *state = INTERPRETER_QUIT;
        }
    }
}

void window_draw(const struct chip8* c8)
{
    SDL_SetRenderDrawColor(
        renderer,
        window_config.bg_color.r,
        window_config.bg_color.g,
        window_config.bg_color.b,
        window_config.bg_color.a);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(
        renderer,
        window_config.fg_color.r,
        window_config.fg_color.g,
        window_config.fg_color.b,
        window_config.fg_color.a);
    for (int y = 0; y < DISPLAY_H; y++) {
        for (int x = 0; x < DISPLAY_W; x++) {
            int pixel_index = y * DISPLAY_W + x;

            if (c8->display[pixel_index] != 0) {
                SDL_Rect rect = {
                    x * window_config.scale,
                    y * window_config.scale,
                    window_config.scale,
                    window_config.scale
                };

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void window_destroy(void)
{
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();
}
