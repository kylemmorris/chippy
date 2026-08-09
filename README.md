# Chippy
Chippy is my take on a [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) emulator, written in C. This is a little project I am embarking on to learn more about computer architecture. I plan to take this experience with me as I write my own virtual machine & language (someday).
# Status
This is a work-in-progress. A basic REPL (Read-Eval-Print-Loop) exists that implements a couple basic opcodes.
# Building
```bash
chippy$: mkdir build && cd build
chippy/build$: cmake -S .. -B .
chippy/build$: make
```

# Planned Features
- **Emulator (emu)**: Emulates a CHIP-8 machine.
- **Assembler (asm)**: Converts CHIP-8 assembly (in `*.c8a` files, defined [here](Assembly%20Language.md)) into machine code for the Emulator.
- **Disassembler (disa)**: Disassembles a CHIP-8 ROM into CHIP-8 assembly.
- **Debugger (dbg)**: A debugging utility for the emulator.
- ==Stretch==: **Simple High-Level Language**: A high-level language called CHIPSCRIPT (`*.chs`) that compiles to CHIP-8 assembly. TBD on what this would look like. Focusing on the other features first.

See the design of the system [here](Implementation).

# Running
Chippy's CLI will look like this:
```bash
chippy [command] (flags) input
```

On no input, it starts its interpreter from a fresh state. At this time, the REPL is the only option, so:
```shell
chippy
```
## Commands
- `run`: Emulates a supported file.
- `build`: Only builds an assembly or CHIPSCRIPT file. Outputs it as raw `input`.
- `disa`: Disassembles a binary input file to assembly.
## Flags
- `--debug`: Provides a line-by-line debugger during `build` and `run` modes

## Examples
```shell
chippy build test.c8a
chippy run --debug rom.c8
```