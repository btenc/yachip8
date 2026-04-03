#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include "chip8.h"
#include "interpreter.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

struct window_config {
    int scale;
    SDL_Color fg_color;
    SDL_Color bg_color;
};

bool window_init(const struct window_config *config);
void window_handle_events(enum interpreter_state *state);
void window_draw(const struct chip8 *c8);
void window_destroy(void);

#endif
