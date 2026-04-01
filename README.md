# YA CHIP-8
Yet another CHIP-8 interpreter (or emulator).

## Description
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

## Games
- **CC0 CHIP-8 Games**: https://johnearnest.github.io/chip8Archive/

## Resources
- **CHIP-8 Technical Reference**: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

- **SDL2 Documentation**: https://wiki.libsdl.org/SDL2/

- **More info about CHIP-8 and its extensions**: https://chip-8.github.io/links/


