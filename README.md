# YA CHIP-8
Yet another CHIP-8 interpreter (or emulator).

## Description
This is an emulator for **[CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)**, a mid 1970's "virtual machine" initially used on the COSMAC VIP and Telmac 1800. It has since been extended into SUPER-CHIP, XO-CHIP, and other variants that introduce features like higher resolutions, more memory, improved sound, color, and more instructions.

YA CHIP-8 is written in C using SDL2 for graphics and input.
The codebase is meant to be clear, readable, and easy to extend.
The goal is to have a spec-correct implementation of the CHIP-8 system, with "correct" behavior validated by passing roms from the **[CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite)**.

## Status
The project is very early with few instructions supported but it compiles and supports the first two basic display test roms (included under `./roms/tests`).

## Compiling and Running
Requires GCC and SDL2.

Compile:
```
$ cd yachip8/    
$ make
```

Run:
```
$ ./yachip8 ROM_FILE
```

## Games to Play (roms)
- **CC0 CHIP-8 Games**: https://johnearnest.github.io/chip8Archive/

## Supported Instructions
### Standard CHIP-8 Instructions
```
[x] 00E0 - CLS               [ ] 8xy0 - LD Vx, Vy          [ ] Ex9E - SKP Vx
[x] 00EE - RET               [ ] 8xy1 - OR Vx, Vy          [ ] ExA1 - SKNP Vx
[*] 0nnn - SYS addr          [ ] 8xy2 - AND Vx, Vy         [ ] Fx07 - LD Vx, DT
[x] 1nnn - JP addr           [ ] 8xy3 - XOR Vx, Vy         [ ] Fx0A - LD Vx, K
[x] 2nnn - CALL addr         [ ] 8xy4 - ADD Vx, Vy         [ ] Fx15 - LD DT, Vx
[ ] 3xkk - SE Vx, byte       [ ] 8xy5 - SUB Vx, Vy         [ ] Fx18 - LD ST, Vx
[ ] 4xkk - SNE Vx, byte      [ ] 8xy6 - SHR Vx {, Vy}      [ ] Fx1E - ADD I, Vx
[ ] 5xy0 - SE Vx, Vy         [ ] 8xy7 - SUBN Vx, Vy        [ ] Fx29 - LD F, Vx
[x] 6xkk - LD Vx, byte       [ ] 8xyE - SHL Vx {, Vy}      [ ] Fx33 - LD B, Vx
[x] 7xkk - ADD Vx, byte      [ ] 9xy0 - SNE Vx, Vy         [ ] Fx55 - LD [I], Vx
[x] Annn - LD I, addr        [ ] Bnnn - JP V0, addr        [ ] Fx65 - LD Vx, [I]
[ ] Cxkk - RND Vx, byte      [x] Dxyn - DRW Vx, Vy, nibble
```
*`0nnn` is ignored on modern interpreters (it would call a subroutine on the host machine).

## Resources
- **CHIP-8 Technical Reference**: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

- **SDL2 Documentation**: https://wiki.libsdl.org/SDL2/

- **More info about CHIP-8 and its extensions**: https://chip-8.github.io/links/


