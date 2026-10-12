/**********************************************************************************
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

#include <stdio.h>
#include <string.h>

#include "emulator.h"
#include "log.h"
#include "version.h"

// The space reserved for the interpreter
// within the program memory (in U16 words).
#define C8_RESERVED_SPACE_U16 (256)

// The rest of the program memory. Derived
// by subtracting the total (2048) by the
// reserved space.
#define C8_FREE_SPACE_U16 (1792)

// The size of the stack according
// to the spec.
#define C8_STACK_SIZE (16)

// The number of general-purpose
// registers.
#define C8_NUM_REGISTERS (16)

struct c8_emulator
{
    U16 reserved[C8_RESERVED_SPACE_U16]; // Space reserved for the interpreter
    U16 mem[C8_FREE_SPACE_U16];          // Program memory
    U16 stack[C8_STACK_SIZE];            // The stack used to store return addresses for subroutines
    U8 v_reg[C8_NUM_REGISTERS];          // 8-bit general-purpose registers.
    U16 i;         // I 16-bit register (for memory addresses)
    U16 pc;        // 16-bit program counter (current address)
    U8 sp;         // 8-bit stack pointer
};

static struct c8_emulator _emulator;

/*********************************************************************************/

void c8_nibbles(U16 instruction, U8* n0_msn, U8* n1, U8* n2, U8* n3_lsn)
{
    // Mask the nibble we want, and
    // then shift it to the least-significant
    // nibble (lsn) to get the final value into
    // a U8.
    if (c8_big_endian())
    {
        // Big-Endian
        // 0000 0000 0000 0000 [16 bits]
        // ^msn                [ 4 bits]
        *n3_lsn = (U8)(instruction & 0x000F);
        *n2     = (U8)((instruction & 0x00F0) >> 4);
        *n1     = (U8)((instruction & 0x0F00) >> 8);
        *n0_msn = (U8)((instruction & 0xF000) >> 12);
    }
    else
    {
        // Little-Endian
        // 0000 0000 0000 0000 [16 bits]
        //                ^msn [ 4 bits]
        *n0_msn = (U8)(instruction & 0x000F);
        *n1     = (U8)((instruction & 0x00F0) >> 4);
        *n2     = (U8)((instruction & 0x0F00) >> 8);
        *n3_lsn = (U8)((instruction & 0xF000) >> 12);
    }
}

/*********************************************************************************/

static void c8_emu_reset_stack(void)
{
    // Reset stack pointer
    _emulator.sp = 0;
    // Zero out memory
    memset(_emulator.stack, (U16)0, C8_STACK_SIZE-1);
}

/*********************************************************************************/

void c8_emu_init(void)
{
    // Zero out memory.
    memset(_emulator.reserved, (U16)0, C8_RESERVED_SPACE_U16-1);
    memset(_emulator.mem, (U16)0, C8_FREE_SPACE_U16-1);
    c8_emu_reset_stack();
    memset(_emulator.v_reg, (U8)0, C8_NUM_REGISTERS);
    _emulator.i = (U16)0;
    _emulator.pc = (U16)0;
    // Add display sigils to reserved memory.
    // TODO
}

void c8_emu_load(U16* program_data, U16 size)
{
    // Make sure we won't overflow the program memory
    if(size >= C8_FREE_SPACE_U16)
    {
        exit(C8_ERROR_INVALID_MEMORY);
    }
    memcpy(_emulator.mem, program_data, (size_t)size);
}

void c8_emu_interp(U16 instruction)
{
    // Extract all nibbles (4 bits).
    // According to spec, all instructions are 
    // 2-bytes long (U16) and stored most-significant
    // byte first. So, n0 will be the most-significant
    // nibble.
    U8 n0_msn = 0;
    U8 n1 = 0;
    U8 n2 = 0;
    U8 n3 = 0;
    c8_nibbles(instruction, &n0_msn, &n1, &n2, &n3);

    // Print'em out (for now)
    printf("Input: " U16_HEX_FMT "\n", instruction);
    printf("Provided nibbles: " U8_HEX_FMT ", " U8_HEX_FMT ", " U8_HEX_FMT ", " U8_HEX_FMT "\n", n0_msn, n1, n2, n3);

    // Dispatch starting with most-significant 
    // nibble like a tree.
    switch (n0_msn)
    {
        case 0x6:
            // 6xkk - Set Vx = kk
            // Remember: technically U8s, but really U4s.
            _emulator.v_reg[n1] = (U8)((n2 << 4) | n3);
            break;
        case 0x8:
            // The 8 family performs operations
            // on the provided registers.
            U8* regX = &_emulator.v_reg[n1];
            U8* regY = &_emulator.v_reg[n2];
            switch (n3)
            {
                case 0x0:
                    // 8xy0 - Set Vx = Vy
                    *regX = *regY;
                    break;
                case 0x1:
                    // 8xy1 - Set Vx = Vx OR Vy
                    *regX = *regX | *regY;
                    break;
                case 0x2:
                    // 8xy2 - Set Vx = Vx AND Vy
                    *regX = *regX & *regY;
                    break;
                case 0x3:
                    // 8xy3 - Set Vx = Vx XOR Vy
                    *regX = *regX ^ *regY;
                    break;
                case 0x4:
                    // 8xy4 - Add Vx, Vy
                    // Set Vx = Vx + Vy, set VF = carry
                    U16 tmp = *regX + *regY;
                    if (tmp > U8_MAX)
                    {
                        _emulator.v_reg[0xF] = 1;
                    }
                    *regX = (U8)tmp;
                    break;
            }
            break;
        default:
            c8_log_error(U8_HEX_FMT " family not implemented.", n0_msn); 
            break;
    }
}

void c8_emu_deinit(void)
{
    //
}

/*******************************************************************************/

void c8_emu_show_state(void)
{
    printf("+-------------------------+\n");
    printf("|          STATE          |\n");
    printf("+-------------------------+\n");

    printf("+-------------------------+\n");
    printf("|        REGISTERS        |\n");
    printf("+-------------------------+\n");

#define reg_helper(a, b, c, d)\
    printf("V" #a " = " U8_HEX_FMT \
    " | V" #b " = " U8_HEX_FMT \
    " | V" #c " = " U8_HEX_FMT \
    " | V" #d " = " U8_HEX_FMT " \n", _emulator.v_reg[a], _emulator.v_reg[b], _emulator.v_reg[c], _emulator.v_reg[d])

    reg_helper(0x0, 0x4, 0x8, 0xC);
    reg_helper(0x1, 0x5, 0x9, 0xD);
    reg_helper(0x2, 0x6, 0xA, 0xE);
    reg_helper(0x3, 0x7, 0xB, 0xF);

#undef reg_helper

    //printf("V0 = 0x%02X | V1 = 0x%02X | V2 = 0x%02X | V3 = 0x%02X \n",
    //       _emulator.v0, _emulator.v1, _emulator.v2, _emulator.v3);
    //printf("V4 = 0x%02X | V5 = 0x%02X | V6 = 0x%02X | V7 = 0x%02X \n",
    //       _emulator.v4, _emulator.v5, _emulator.v6, _emulator.v7);
    //printf("V8 = 0x%02X | V9 = 0x%02X | VA = 0x%02X | VB = 0x%02X \n",
    //       _emulator.v8, _emulator.v9, _emulator.vA, _emulator.vB);
    //printf("VC = 0x%02X | VD = 0x%02X | VE = 0x%02X | VF = 0x%02X \n",
    //       _emulator.vC, _emulator.vD, _emulator.vE, _emulator.vF);
    printf(" I = " U16_HEX_FMT "\n", _emulator.i);
    printf("PC = " U16_HEX_FMT " | " U16_HEX_FMT "\n", _emulator.pc, _emulator.mem[_emulator.pc]);

    printf("+-------------------------+\n");
    printf("|          STACK          |\n");
    printf("+-------------------------+\n");
    // Go top to bottom
    for(U8 i = C8_STACK_SIZE; i > 0; --i)
    {
        if(_emulator.sp == (i-1))
        {
            printf("["U16_HEX_FMT"] <sp \n", _emulator.stack[i-1]);
        }
        else
        {
            printf("["U16_HEX_FMT"]\n", _emulator.stack[i-1]);
        }
    }

    printf("+-------------------------+\n");
    printf("|           END           |\n");
    printf("+-------------------------+\n");
}




