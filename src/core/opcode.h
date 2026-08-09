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

#ifndef C8_H_OPCODE
#define C8_H_OPCODE

#include "common.h"

// CHIP-8 has distinct 35 opcodes, which are all two
// bytes long (U16) and stored big-endian.
// The format is that the first 4-bits are the general
// type, and the remaining 12-bits provide the arguments
// for the operation. E.g. JUMP is 1NNN =
// type:1 arg:NNN.
//enum c8_opcode : U8
//{
//    // Calls machine code routine at
//    // address NNN. Not necessary.
//    // [0NNN]
//    C8_OPCODE_MACHINE_ROUTINE,
//    // Clears the screen. [00E0]
//    C8_OPCODE_DISP_CLEAR,
//    // Returns from a subroutine. [00EE]
//    C8_OPCODE_RETURN,
//    // Jumps to address NNN. [1NNN]
//    C8_OPCODE_JUMP,
//    // Calls subroutine at NNN. [2NNN]
//    C8_OPCODE_CALL,
//    // Skips next instr if VX == NN. [3XNN]
//    C8_OPCODE_JUMP_CONST_EQ,
//    // Skips next instr if VX != NN [4XNN]
//    C8_OPCODE_JUMP_CONST_NEQ,
//    // Skips next instr if VX == VY [5XY0]
//    C8_OPCODE_JUMP_REG_EQ,
//    // Sets VX to NN [6XNN]
//    C8_OPCODE_SET_CONST,
//    // Adds NN to VX (carry flag not changed)
//    // [7XNN]
//    C8_OPCODE_ADD_CONST,
//    // Sets VX to value of VY. [8XY0]
//    C8_OPCODE_SET_REG,
//    // Sets VX to VX OR VY. [8XY1]
//    C8_OPCODE_BITWISE_OR,
//    // Sets VX to VX AND VY. [8XY2]
//    C8_OPCODE_BITWISE_AND,
//    // Sets VX to VX XOR VY. [8XY3]
//    C8_OPCODE_BITWISE_XOR,
//    // Adds VY to VX. VF set to 1 when
//    // overflow, 0 when not. [8XY4]
//    C8_OPCODE_ADD_REG,
//    // Sub VY from VX. VF set to 0 when
//    // underflow, 1 when not. [8XY5]
//    C8_OPCODE_SUB_REG,
//    // Shift VX right by 1, stores LSB
//    // prior to shift into VF. [8XY6]
//    C8_OPCODE_RSHIFT,
//    // Set VX to VY - VX. VF set to 0
//    // when underflow, 1 when not.
//    // (i.e. VF set to 1 if VY >= VX)
//    // [8XY7]
//    C8_OPCODE_SUB_REG_SET,
//    // Shift VX to left by 1, then set VF
//    // to 1 if MSB of VX prior to shift
//    // was set, or 0 if it was unset.
//    // [8XYE]
//    C8_OPCODE_LSHIFT,
//    // Skip next instr if VX != VY [9XY0]
//    C8_OPCODE_JUMP_REG_NEQ,
//    // Sets I to the address NNN [ANNN]
//    C8_OPCODE_MEMSET,
//    // Jumps to address NNN + V0 [BNNN]
//    C8_OPCODE_JUMP_OFFSET,
//    // Sets VX to result of a bitwise
//    // operation on a random number
//    // (0 to 255) and NN. [CXNN]
//    C8_OPCODE_RAND,
//    // ... [DXYN]
//    C8_OPCODE_DISPLAY,
//    // Skip next instr if the key stored
//    // in VX (only consider lowest nibble)
//    // is pressed. [EX9E]
//    C8_OPCODE_KEY_ON,
//    // Skip next instr if the key stored
//    // in VX (only consider lowest nibble)
//    // is NOT pressed. [EXA1]
//    C8_OPCODE_KEY_OFF,
//    // Sets VX to value of the delay
//    // timer. [FX07]
//    C8_OPCODE_GET_TIMER_DELAY,
//    // (Blocking) A key press is awaited
//    // and stored in VX (all instructions
//    // halted, but delay & sound timers
//    // should continue processing) [FX0A]
//    C8_OPCODE_GET_KEY,
//    // Set delay timer to VX. [FX15]
//    C8_OPCODE_SET_DELAY_TIMER,
//    // Set sound timer to VX. [FX18]
//    C8_OPCODE_SET_SOUND_TIMER,
//    // Adds VX to I. VF not effected.
//    // [FX1E]
//    C8_OPCODE_MEMINC_REG,
//    // Sets I to the location of the sprite
//    // for the character in VX (onlt consider
//    // the lowest nibble). Characters 0-F (in hex)
//    // are represented by a 4x5 font. [FX29]
//    C8_OPCODE_MEM_SPRITE,
//    // Stores the binary-coded decimal representation
//    // of VX, with hundreds digit at location I,
//    // tens digit at location I+1, and ones at
//    // location I+2. [FX33]
//    C8_OPCODE_BCD,
//    // Stores from V0 to VX (including VX) in
//    // memory, starting at address I. The offset
//    // from I is increased by 1 for each value
//    // wirrten, but I itself is left unmodified.
//    // [FX55]
//    C8_OPCODE_MEM_DUMP,
//    // Fills from V0 to VX (including VX) with
//    // values from memory, starting at address
//    // I. The offset from I is increased by 1 for
//    // each value read, but I itself is left
//    // is left unmodified. [FX65]
//    C8_OPCODE_MEM_LOAD,
//    C8_OPCODE_COUNT
//};

// Converts an instruction to a string representation.
//const char* c8_op2str(const U16* instr);

#endif // C8_H_OPCODE
