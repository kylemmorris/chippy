/***********************************************************************************
 * Chippy CHIP-8 Emulator
 * Copyright (c) 2026 Kyle Morris.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************/

#ifndef C8_H_EMULATOR
#define C8_H_EMULATOR

#include "common.h"

/**********************************************************************************/

// Initializes the emulator.
void c8_emu_init(void);

// Loads the emulator's program
// data section with the ROM.
// If there is not enough memory
// for this data, the program is terminated.
void c8_emu_load(U16* program_data, U16 size);

// Performs a single instruction.
void c8_emu_interp(U16 instruction);

// De-initializes the emulator.
void c8_emu_deinit(void);

/**********************************************************************************/

// Displays the current state of
// the emulator. Prints to STDOUT.
void c8_emu_show_state(void);

#endif // C8_H_EMULATOR
