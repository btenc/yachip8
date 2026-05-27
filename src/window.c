#include "window.h"

#include <stdio.h>

#define SAMPLE_RATE 44100
#define BEEP_HZ 440
#define BEEP_VOL 3000

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static struct window_config window_config;

static bool sound_active = false;
static int sound_phase = 0;

static void audio_cb(void *userdata, uint8_t *stream, int len)
{
    int16_t *buf = (int16_t *)stream;
    int period = SAMPLE_RATE / BEEP_HZ;
    int n = len / (int)sizeof(int16_t);

    (void)userdata;

    for (int i = 0; i < n; i++) {
        if (!sound_active) {
            buf[i] = 0;
        } else if (sound_phase % period < period / 2) {
            buf[i] = BEEP_VOL;
        } else {
            buf[i] = -BEEP_VOL;
        }
        sound_phase++;
    }
}

bool window_init(const struct window_config *config)
{
    bool success = true;

    window_config = *config;

    int window_w = DISPLAY_W * window_config.scale;
    int window_h = DISPLAY_H * window_config.scale;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "E: SDL video could not initialize: %s\n",
                SDL_GetError());
        success = false;
    }

    if (success) {
        window = SDL_CreateWindow("Yet Another CHIP-8", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, window_w, window_h,
                                  SDL_WINDOW_SHOWN);

        if (window == NULL) {
            fprintf(stderr, "E: window could not be created: %s\n",
                    SDL_GetError());
            success = false;
        }
    }

    if (success) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (renderer == NULL) {
            fprintf(stderr, "E: renderer could not be created: %s\n",
                    SDL_GetError());
            success = false;
        }
    }

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "W: SDL audio could not initialize: %s\n",
                SDL_GetError());
    } else {
        SDL_AudioSpec want = {0};
        want.freq = SAMPLE_RATE;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = 512;
        want.callback = audio_cb;
        if (SDL_OpenAudio(&want, NULL) < 0) {
            fprintf(stderr, "W: audio device could not be opened: %s\n",
                    SDL_GetError());
        } else {
            SDL_PauseAudio(0);
        }
    }

    if (!success) {
        window_destroy();
    }

    return success;
}

/* standard CHIP-8 hex keypad layout mapped to QWERTY keyboard
 * CHIP-8: 1 2 3 C    Keyboard: 1 2 3 4
 *         4 5 6 D              Q W E R
 *         7 8 9 E              A S D F
 *         A 0 B F              Z X C V */
static int sdl_key_to_chip8(SDL_Keycode key)
{
    switch (key) {
    case SDLK_1:
        return 0x1;
    case SDLK_2:
        return 0x2;
    case SDLK_3:
        return 0x3;
    case SDLK_4:
        return 0xC;
    case SDLK_q:
        return 0x4;
    case SDLK_w:
        return 0x5;
    case SDLK_e:
        return 0x6;
    case SDLK_r:
        return 0xD;
    case SDLK_a:
        return 0x7;
    case SDLK_s:
        return 0x8;
    case SDLK_d:
        return 0x9;
    case SDLK_f:
        return 0xE;
    case SDLK_z:
        return 0xA;
    case SDLK_x:
        return 0x0;
    case SDLK_c:
        return 0xB;
    case SDLK_v:
        return 0xF;
    default:
        return -1;
    }
}

void window_handle_events(enum interpreter_state *state, struct chip8 *c8)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *state = INTERPRETER_QUIT;
        } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            int chip8_key = sdl_key_to_chip8(event.key.keysym.sym);
            if (chip8_key != -1) {
                c8->keypad[chip8_key] = (event.type == SDL_KEYDOWN) ? 1 : 0;
            }
        }
    }
}

void window_draw(const struct chip8 *c8)
{
    SDL_SetRenderDrawColor(renderer, window_config.bg_color.r,
                           window_config.bg_color.g, window_config.bg_color.b,
                           window_config.bg_color.a);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, window_config.fg_color.r,
                           window_config.fg_color.g, window_config.fg_color.b,
                           window_config.fg_color.a);
    for (int y = 0; y < DISPLAY_H; y++) {
        for (int x = 0; x < DISPLAY_W; x++) {
            int pixel_index = y * DISPLAY_W + x;

            if (c8->display[pixel_index] != 0) {
                SDL_Rect rect = {x * window_config.scale,
                                 y * window_config.scale, window_config.scale,
                                 window_config.scale};

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void window_set_sound(bool active) { sound_active = active; }

void window_destroy(void)
{
    SDL_CloseAudio();

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
