#include "chip8.h"
#include "interpreter.h"
#include "window.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    srand((unsigned int)time(NULL));

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
        window_handle_events(&state, &c8);

        chip8_tick_timers(&c8);
        window_set_sound(c8.sound_timer > 0);

        if (state == INTERPRETER_RUNNING) {
            for (int i = 0; i < config.speed.steps_per_frame; i++) {
                if (c8.waiting_for_vblank)
                    break;
                chip8_step(&c8);
            }
        }
        c8.waiting_for_vblank = false;

        window_draw(&c8);

        SDL_Delay(config.speed.frame_delay_ms);
    }

#ifdef DEBUG
    chip8_dump_memory(&c8);
#endif

    window_destroy();
    fprintf(stderr, "I: window exited\n");
    return 0;
}
