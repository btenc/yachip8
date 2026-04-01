#include "chip8.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const uint8_t chip8_font[FONT_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80 //  F
};

void chip8_init(struct chip8* c8)
{
    struct chip8 c8_zeroed = { 0 };
    *c8 = c8_zeroed;

    c8->pc = ROM_START;

    for (size_t i = 0; i < FONT_SIZE; i++) {
        c8->memory[FONT_START + i] = chip8_font[i];
    }

    c8->should_redraw = true;
}

bool chip8_load_rom(struct chip8* c8, const char* rom_path)
{
    // open stream to read from into chip-8 memory
    FILE* rom_file = fopen(rom_path, "rb");
    if (rom_file == NULL) {
        fprintf(stderr, "E: could not open rom '%s': %s\n", rom_path, strerror(errno));
        return false;
    }

    // read and copy stream in BUFSIZ chunks of bytes until EOF
    uint8_t buffer[BUFSIZ];
    size_t bytes_read;
    size_t memory_index = ROM_START;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), rom_file)) > 0) {
        if (memory_index + bytes_read > MEMORY_SIZE) {
            fprintf(stderr, "E: rom '%s' is too big\n", rom_path);
            fclose(rom_file);
            return false;
        }

        for (size_t i = 0; i < bytes_read; i++) {
            c8->memory[memory_index + i] = buffer[i];
        }

        memory_index += bytes_read;
    }

    fclose(rom_file);
    return true;
}

void chip8_step(struct chip8* c8)
{
    /* FETCH */
    if (c8->pc + 1 >= MEMORY_SIZE) {
        fprintf(stderr, "E: program counter out of bounds: %u\n", c8->pc);
        return;
    }

    uint8_t high_byte = c8->memory[c8->pc];
    uint8_t low_byte = c8->memory[c8->pc + 1];

    // move the high byte into the upper 8 bits bitwise OR with the low byte
    uint16_t opcode = ((uint16_t)high_byte << 8) | low_byte;

#ifdef DEBUG
    // printf("pc=%03X opcode=%04X\n", c8->pc, opcode);
#endif

    c8->pc += 2;

    /* DECODE */
    uint8_t opcode_type = (opcode & 0xF000) >> 12; // first nibble
    uint8_t x = (opcode & 0x0F00) >> 8; // second nibble V[x]
    uint8_t y = (opcode & 0x00F0) >> 4; // third nibble V[y]
    uint8_t n = opcode & 0x000F; // fourth nibble
    uint8_t kk = opcode & 0x00FF; // second byte
    uint16_t nnn = opcode & 0x0FFF; // lowest 12 bits (address)

    /* EXECUTE */
    switch (opcode_type) {

    case 0x0:
        if (nnn == 0x0E0) { // 00E0 - CLS
            for (size_t i = 0; i < DISPLAY_SIZE; i++) {
                c8->display[i] = 0;
            }
            c8->should_redraw = true;
        } else {
            fprintf(stderr, "E: unknown opcode %04X\n", opcode);
        }
        break;

    case 0x1: // 1nnn - JP addr
        c8->pc = nnn;
        break;

    case 0x6: // 6xkk - LD Vx, byte
        c8->V[x] = kk;
        break;

    case 0x7: // 7xkk - ADD Vx, byte
        c8->V[x] += kk;
        break;

    case 0xA: // Annn - LD I, addr
        c8->I = nnn;
        break;

    case 0xD: { // Dxyn - DRW Vx, Vy, nibble
        uint8_t start_x = c8->V[x] % DISPLAY_W;
        uint8_t start_y = c8->V[y] % DISPLAY_H;

        c8->V[0xF] = 0; // collision flag cleared

        for (uint8_t row = 0; row < n; row++) {
            uint8_t screen_y = start_y + row;

            if (screen_y >= DISPLAY_H) {
                break;
            }

            // each sprite row is stored in 1 byte
            uint8_t sprite_row = c8->memory[c8->I + row];

            for (uint8_t col = 0; col < 8; col++) {
                uint8_t screen_x = start_x + col;

                // Stop this row if we hit the right edge
                if (screen_x >= DISPLAY_W) {
                    break;
                }

                // read each bit from L to R in the row byte
                uint8_t sprite_bit = (sprite_row >> (7 - col)) & 1;

                if (sprite_bit == 0) {
                    continue;
                } // else the sprite bit is on

                size_t screen_index = screen_y * DISPLAY_W + screen_x;

                // Collision happened somewhere during the draw
                if (c8->display[screen_index] == 1) {
                    c8->V[0xF] = 1;
                }

                // XOR: turns the pixel on if it was off, and off if it was on.
                c8->display[screen_index] ^= 1;
            }
        }

        c8->should_redraw = true;
        break;
    }

    default:
        fprintf(stderr, "E: unknown opcode %04X\n", opcode);
        break;
    }
}

void chip8_tick_timers(struct chip8* c8)
{
    if (c8->delay_timer > 0) {
        c8->delay_timer--;
    }

    if (c8->sound_timer > 0) {
        c8->sound_timer--;
    }
}

#ifdef DEBUG
void chip8_dump_memory(const struct chip8* c8)
{
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        if (i % 16 == 0) {
            printf("\n%03zX: ", i);
        }
        printf("%02X ", c8->memory[i]);
    }

    printf("\n");
}
#endif
