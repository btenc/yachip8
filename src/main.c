#include "chip8.h"
#include "interpreter.h"
#include "window.h"

#include <SDL2/SDL.h>
#include <stdio.h>

struct speed_config {
    int steps_per_frame;
    Uint32 frame_delay_ms;
};

struct interpreter_config {
    struct speed_config speed;
    struct window_config window;
};

int main(int argc, char *argv[])
{
    struct chip8 c8;
    enum interpreter_state state = INTERPRETER_RUNNING;

    const struct interpreter_config config = {
        {10, 16}, {24, {255, 255, 255, 255}, {0, 0, 0, 255}}};

    if (argc != 2) {
        fprintf(stderr, "Usage: %s ROM_FILE\n", argv[0]);
        return 1;
    }

    chip8_init(&c8);
    fprintf(stderr, "I: CHIP-8 Initialized\n");

    if (!chip8_load_rom(&c8, argv[1])) {
        return 1;
    }
    fprintf(stderr, "I: rom loaded\n");

    if (!window_init(&config.window)) {
        return 1;
    }
    fprintf(stderr, "I: window initialized\n");

    while (state != INTERPRETER_QUIT) {
        window_handle_events(&state);

        chip8_tick_timers(&c8);

        if (state == INTERPRETER_RUNNING) {
            for (int i = 0; i < config.speed.steps_per_frame; i++) {
                chip8_step(&c8);
            }
        }

        if (c8.should_redraw) {
            window_draw(&c8);
            c8.should_redraw = false;
        }

        SDL_Delay(config.speed.frame_delay_ms);
    }

#ifdef DEBUG
    chip8_dump_memory(&c8);
#endif

    window_destroy();
    fprintf(stderr, "I: window exited\n");
    return 0;
}
