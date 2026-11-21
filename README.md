# Chippy
Chippy is my take on a [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) emulator, written in C. This is a little project I am embarking on to learn more about computer architecture.

# Status
This is a work-in-progress. A basic REPL (Read-Eval-Print-Loop) exists that implements a couple basic opcodes.

# Building
```bash
chippy$: mkdir build
chippy$: cd build
chippy/build$: cmake -S .. -B .
chippy/build$: make
```

# Planned Features
- Virtual Machine: Implements the fundamental CHIP-8 instruction set (not Super CHIP-8 at this time).
- Assembler: Converts CHIP-8 assemby (defined [here](doc/assembly_language.md)) into machine code.
- Disassembler: Performs the inverse operation of the Assembler.
- Debugger: A simple debugging utility which, at this time, means step-through execution, intermediate
  inputs, and state printing.

# Running
Currently, it only supports a Read-Eval-Print-Loop (REPL),
by providing no arguments:
```bash
chippy
```

However, the following structure will be implemented, through
it's still up in the air:
```shell
chippy command <flags> input_file
```

## Commands
- `run`: Runs an inputted CHIP-8 ROM. If the input is a CHIP-8 ROM.
- `asm`: Assembles an inputted assembly file (`*.c8asm`).
- `dasm`: Disassembles a CHIP-8 ROM to assembly.
## Flags
- `--debug`: Provides a line-by-line debugger during `run`.
