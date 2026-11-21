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

#include "vm.h"
#include "log.h"
#include "version.h"

/*********************************************************************************/

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

// The instructions defined by the
// CHIP-8 specification contain the
// opcode AND its operands. This enum
// is purely for readability within
// c8_vm_interp().
typedef enum c8_opcode_group
{
    C8_OPCODE_GROUP_SET_CONST = 0x6,
    C8_OPCODE_GROUP_OPERATIONS = 0x8,
    C8_OPCODE_GROUP_COUNT
} c8_opcode_group_t;

// Structure that defines the Virtual Machine (VM)
typedef struct c8_vm
{
    U16 reserved[C8_RESERVED_SPACE_U16]; // Space reserved for the interpreter
    U16 mem[C8_FREE_SPACE_U16];          // Program memory
    U16 stack[C8_STACK_SIZE];            // The stack used to store return addresses for subroutines
    U8  v_reg[C8_NUM_REGISTERS];         // 8-bit general-purpose registers.
    U16 i;                  // I 16-bit register (for memory addresses)
    U16 pc;                 // 16-bit program counter (current address)
    U8  sp;                 // 8-bit stack pointer
} c8_vm_t;

// This is the static Virtual Machine instance
static c8_vm_t _vm;

/*********************************************************************************/

static void c8_nibbles(U16 instruction, U8* n0_msn, U8* n1, U8* n2, U8* n3_lsn)
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

static void c8_vm_reset_stack(void)
{
    // Reset stack pointer
    _vm.sp = 0;
    // Zero out memory
    memset(_vm.stack, (U16)0, C8_STACK_SIZE-1);
}

/*********************************************************************************/

void c8_vm_init(void)
{
    // Zero out memory.
    memset(_vm.reserved, (U16)0, C8_RESERVED_SPACE_U16-1);
    memset(_vm.mem, (U16)0, C8_FREE_SPACE_U16-1);
    c8_vm_reset_stack();
    memset(_vm.v_reg, (U8)0, C8_NUM_REGISTERS);
    _vm.i = (U16)0;
    _vm.pc = (U16)0;
    _vm.sp = (U8)0;

    // Add display sigils to reserved memory.
    // TODO
}

void c8_vm_load(U16* program_data, U16 size)
{
    c8_assert_range(size, 0, C8_FREE_SPACE_U16);
    memcpy(_vm.mem, program_data, (size_t)size);
}

void c8_vm_interp(U16 instruction)
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

    // High + low are inclusive
    c8_assert_range(n0_msn, 0, C8_OPCODE_GROUP_COUNT-1);

    // LSBIT: Gets least-significant bit by
    // ANDing with 0000 0001
    #define LSBIT(V) (V & 0x01)
    // MSBIT: Gets least-significant bit by
    // ANDing with 1000 0000 (or 0x80)
    #define MSBIT(V) (V & 0x80)

    // Dispatch starting with most-significant 
    // nibble like a tree.
    switch (n0_msn)
    {
        case C8_OPCODE_GROUP_SET_CONST:
        {
            // 6xkk - Set Vx = kk
            // Remember: technically U8s, but really U4s.
            _vm.v_reg[n1] = (U8)((n2 << 4) | n3);
            break;
        }
        case C8_OPCODE_GROUP_OPERATIONS:
        {
            // The 8 family performs operations
            // on the provided registers.
            U8* regX = &_vm.v_reg[n1];
            U8* regY = &_vm.v_reg[n2];
            U16 tmp;
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
                    tmp = *regX + *regY;
                    if (tmp > U8_MAX)
                    {
                        _vm.v_reg[0xF] = 1;
                    }
                    *regX = (U8)tmp;
                    break;
                case 0x5:
                    // 8xy5 - Set Vx = Vx - Vy, set VF = NOT borrow
                    // If Vx > Vy, then VF is set to 1, otherwise 0.
                    // Then Vy is subtracted from Vx, and the results stored in Vx.
                    _vm.v_reg[0xF] = (*regX > *regY);
                    *regX = *regX - *regY;
                    break;
                case 0x6:
                    // 8xy6 - Set Vx = Vx SHR 1
                    // If the least-significant bit of Vx is 1, then
                    // VF is set to 1, otherwise 0. Then Vx is divided by 2.
                    _vm.v_reg[0xF] = LSBIT(*regX);
                    *regX = *regX >> 1;
                    break;
                case 0x7:
                    // 8xy7 - Set Vx = Vy - Vx, set VF = NOT borrow.
                    // If Vy > Vx, then VF is set to 1, otherwise 0.
                    // Then Vx is subtracted from Vy, and the results stored in Vx.
                    _vm.v_reg[0xF] = (*regY > *regX);
                    *regX = *regY - *regY;
                    break;
                case 0xE:
                    // 8xyE - Set Vx = Vx SHL 1.
                    // If the most-significant bit of Vx is 1, then VF is
                    // set to 1, otherwise to 0. Then Vx is multiplied by 2.
                    _vm.v_reg[0xF] = LSBIT(*regX);
                    *regX = *regX << 1;
                    break;
            }
            break;
        }
        default:
        {
            c8_log_error(U8_HEX_FMT " opcode family not implemented.", n0_msn);
            return;
        }
    }

    #undef LSBIT
    #undef MSBIT
}

void c8_vm_deinit(void)
{
    //
}

/*******************************************************************************/

void c8_vm_show_state(void)
{
    // TODO: Show output in binary as well as hex
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
    " | V" #d " = " U8_HEX_FMT " \n", _vm.v_reg[a], _vm.v_reg[b], _vm.v_reg[c], _vm.v_reg[d])

    reg_helper(0x0, 0x4, 0x8, 0xC);
    reg_helper(0x1, 0x5, 0x9, 0xD);
    reg_helper(0x2, 0x6, 0xA, 0xE);
    reg_helper(0x3, 0x7, 0xB, 0xF);

#undef reg_helper

    //printf("V0 = 0x%02X | V1 = 0x%02X | V2 = 0x%02X | V3 = 0x%02X \n",
    //       _vm.v0, _vm.v1, _vm.v2, _vm.v3);
    //printf("V4 = 0x%02X | V5 = 0x%02X | V6 = 0x%02X | V7 = 0x%02X \n",
    //       _vm.v4, _vm.v5, _vm.v6, _vm.v7);
    //printf("V8 = 0x%02X | V9 = 0x%02X | VA = 0x%02X | VB = 0x%02X \n",
    //       _vm.v8, _vm.v9, _vm.vA, _vm.vB);
    //printf("VC = 0x%02X | VD = 0x%02X | VE = 0x%02X | VF = 0x%02X \n",
    //       _vm.vC, _vm.vD, _vm.vE, _vm.vF);
    printf(" I = " U16_HEX_FMT "\n", _vm.i);
    printf("PC = " U16_HEX_FMT " | " U16_HEX_FMT "\n", _vm.pc, _vm.mem[_vm.pc]);

    printf("+-------------------------+\n");
    printf("|          STACK          |\n");
    printf("+-------------------------+\n");
    // Go top to bottom
    for(U8 i = C8_STACK_SIZE; i > 0; --i)
    {
        if(_vm.sp == (i-1))
        {
            printf("["U16_HEX_FMT"] <sp \n", _vm.stack[i-1]);
        }
        else
        {
            printf("["U16_HEX_FMT"]\n", _vm.stack[i-1]);
        }
    }

    printf("+-------------------------+\n");
    printf("|           END           |\n");
    printf("+-------------------------+\n");
}




