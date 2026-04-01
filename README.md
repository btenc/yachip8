# YA CHIP-8
Yet another CHIP-8 interpreter (or emulator).

## Description
This is an emulator for **[CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)**, a mid 1970's "virtual machine" initially used on the COSMAC VIP and Telmac 1800. It has since been extended into Super-CHIP, XO-CHIP, and other variants that introduce features like higher resolutions, more memory, improved sound, color, and more opcodes.

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

## Resources
- **CHIP-8 Technical Reference**: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

- **SDL2 Documentation**: https://wiki.libsdl.org/SDL2/

- **More info about CHIP-8 and its extensions**: https://chip-8.github.io/links/

- **CHIP-8 archive**: https://github.com/JohnEarnest/chip8Archive


