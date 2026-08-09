# Preamble
- **Modules** are `.h` / `.c` pairs (common practice).
- Modules have code-names to shorten function names.
- Follows [[Style Guide]].
# References
- [Cowgod's CHIP-8 Technical Reference](https://github.com/trapexit/chip-8_documentation/blob/master/Misc/Cowgod's%20CHIP-8%20Technical%20Reference.pdf).
- [Wikipedia Article](https://en.wikipedia.org/wiki/CHIP-8).
- [Test bed I will use someday](https://github.com/mattmikolay/chip-8).
# Emulator (Emu) Module
## During normal operation
- [ ] Loads CHIP-8 ROM into memory (`c8_emulator.data`), sets `c8_emulator.pc` to the start, and ingests each `U16` sequentially (via `c8_emu_interp()`).
- [ ] During each ingestion, it first parses the operation. It views the first 4 bits to determine the process, and further down determines the opcode.
- [ ] Once opcode is known, it performs the operation using the contents of the `U16`.

## During interp mode
- [ ] REPL starts awaiting user input in various modes:
	- [ ] `(raw)$: <input>`
	- [ ] `(asm)$: <input>`
	- [ ] `(scr)$: <input>`
- [ ] Data is interpreted as raw hexadecimal (raw mode), assembly (asm mode), or CHIPSCRIPT (script mode).
- [ ] Emulator uses `c8_interp()` on that information, performs the task, and then displays the status of the system via `c8_emu_show_state()`.

# Disassembler (Disa) Module
- [ ] First loads a binary assembled file (any extension) via `c8_fopen()`.
- [ ] Opens a new file for writing with the same name as the original binary file, with `_disa.c8` appended.
- [ ] Inserts a comment preamble with some information on the version of Chippy that was used to disassemble, along with date and time.
- [ ] TBD

# Assembler (Asm) Module
Assembler reads file line-by-line. For each line:
- [ ] Scanner scan the input string and outputs a token
- [ ] TBD
# Main Loop Design
## Raw ROM
1. Read file.
2. Insert contents to `c8_emu.program`.
3. Run main loop.
## Assembly
1. Read file.
2. Asm (`c8_asm`) chugs through, creates raw opcodes, inserts into `c8_emu.program` directly.
3. Run main loop.
## CHIPSCRIPT
1. Read file.
2. `c8_script_compiler` runs, creates assembly.
3. `c8_asm` runs.
4. Run main loop.
## The Main Loop
1. GLFW Process Inputs.
2. `c8_emu_interp()`
	1. Either from `c8_emu.pc`, or
	2. A single opcode from the REPL.
3. Draw screen.
4. Swap buffers (present).
5. Throttle to target FPS.