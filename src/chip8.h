#ifndef CHIP8_HEADER
#define CHIP8_HEADER

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 4096 /* bytes */
#define NUM_REGISTERS 16 /* 16 8-bit registers: V0-VF */
#define STACK_SIZE 16    /* 16 16-bit values */
#define DISPLAY_W 64     /* px */
#define DISPLAY_H 32     /* px */
#define DISPLAY_SIZE (DISPLAY_W * DISPLAY_H)
#define NUM_KEYS 16
#define ROM_START 0x200
#define FONT_START 0x050
#define FONT_SIZE (16 * 5) /* bytes */

struct chip8_quirks {
    bool vf_reset;     /* 8xy1/2/3 reset VF to 0 after operation */
    bool memory;       /* Fx55/Fx65 increment I by x+1 */
    bool display_wait; /* DRW waits for vblank before continuing */
    bool clipping;     /* sprites clip at screen edges instead of wrapping */
    bool shifting;     /* 8xy6/E operate on VX; when off, copies VY first */
    bool jumping;      /* Bnnn uses VX; when off, uses V0 */
};

struct chip8 {
    uint8_t memory[MEMORY_SIZE];

    uint8_t V[NUM_REGISTERS];
    uint16_t I;  /* index register */
    uint16_t pc; /* program counter */

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint8_t sp; /* stack pointer */
    uint16_t stack[STACK_SIZE];

    uint8_t keypad[NUM_KEYS];
    int8_t fx0a_key; /* -1 = not waiting, 0-15 = waiting for this key to release */
    uint8_t display[DISPLAY_SIZE];

    struct chip8_quirks quirks;
    bool waiting_for_vblank;
};

void chip8_init(struct chip8 *c8);
bool chip8_load_rom(struct chip8 *c8, const char *path);
void chip8_step(struct chip8 *c8);
void chip8_tick_timers(struct chip8 *c8);

#ifdef DEBUG
void chip8_dump_memory(const struct chip8 *c8);
#endif

#endif
