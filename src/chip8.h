#ifndef CHIP8_HEADER
#define CHIP8_HEADER

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 4096 // bytes
#define NUM_REGISTERS 16 // 16 8-bit registers: V0-VF
#define STACK_SIZE 16 // 16 16-bit values
#define DISPLAY_W 64 // px
#define DISPLAY_H 32 // px
#define DISPLAY_SIZE (DISPLAY_W * DISPLAY_H)
#define NUM_KEYS 16
#define ROM_START 0x200
#define FONT_START 0x050
#define FONT_SIZE (16 * 5) // bytes

struct chip8 {
    uint8_t memory[MEMORY_SIZE];

    uint8_t V[NUM_REGISTERS];
    uint16_t I; // index register
    uint16_t pc;

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint8_t sp;
    uint16_t stack[STACK_SIZE];

    uint8_t keypad[NUM_KEYS];
    uint8_t display[DISPLAY_SIZE];

    bool should_redraw;
};

void chip8_init(struct chip8* c8);
bool chip8_load_rom(struct chip8* c8, const char* path);
void chip8_step(struct chip8* c8);
void chip8_tick_timers(struct chip8* c8);

#ifdef DEBUG
void chip8_dump_memory(const struct chip8* c8);
#endif

#endif
