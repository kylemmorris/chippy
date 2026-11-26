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

#include <stdio.h>  // for sscanf()
#include <string.h> // for strcmp(), strlen()
#include <ctype.h>  // for tolower()

#include "assembler.h"

// The Assembler converts CHIP-8
// assembly to machine code for
// the Virtual Machine (VM).
typedef struct c8_assembler
{
    const char* source;
    int line;
    int column;
} c8_assembler_t;

// This is the table of
// opcodes, i.e. the optable.
typedef struct c8_optable
{
    char* name;
    U8 opcode;
    int num_operands;
} c8_optable_t;

static c8_assembler_t _assembler;

static c8_optable_t _optable[] =
{
    { "cls",    0x0, 0 },  // 00E0 - CLS
    { "ret",    0x0, 0 },  // 00EE - RET
    { "sys",    0x0, 0 },  // 0nnn - SYS addr
    { "jp",     0x1, 0 },  // 1nnn - JP addr
    { "call",   0x2, 0 },  // 2nnn - CALL addr
    { "se",     0x3, 0 },  // 3xkk - SE Vx, byte
    { "sne",    0x4, 0 },  // 4xkk - SNE Vx, byte
    { "se",     0x5, 0 },  // 5xy0 - SE Vx, Vy
    { "ld",     0x6, 0 },  // 6xkk - LD Vx, byte
    { "add",    0x7, 0 },  // 7xkk - ADD Vx, byte
    { "ld",     0x8, 0 },  // 8xy0 - LD Vx, Vy
    { "or",     0x8, 0 },  // 8xy1 - OR Vx, Vy
    { "and",    0x8, 0 },  // 8xy2 - AND Vx, Vy
    { "xor",    0x8, 0 },  // 8xy3 - XOR Vx, Vy
    { "add",    0x8, 0 },  // 8xy4 - ADD Vx, Vy
    { "sub",    0x8, 0 },  // 8xy5 - SUB Vx, Vy
    { "shr",    0x8, 0 },  // 8xy6 - SHR Vx {, Vy}
    { "subn",   0x8, 0 },  // 8xy7 - SUBN Vx, Vy
    { "shl",    0x8, 0 },  // 8xyE - SHL Vx {, Vy}
    { "sne",    0x9, 0 },  // 9xy0 - SNE Vx, Vy
    { "ld",     0xA, 0 },  // Annn - LD I, addr
    { "jp",     0xB, 0 },  // Bnnn - JP V0, addr
    { "rnd",    0xC, 0 },  // Cxkk - RND Vx, byte
    { "drw",    0xD, 0 },  // Dxyn - DRW Vx, Vy, nibble
    { "skp",    0xE, 0 },  // Ex9E - SKP Vx
    { "sknp",   0xE, 0 },  // ExA1 - SKNP Vx
    { "ld",     0xF, 0 },  // Fx07 - LD Vx, DT
    { "ld",     0xF, 0 },  // Fx0A - LD Vx, K
    { "ld",     0xF, 0 },  // Fx15 - LD DT, Vx
    { "ld",     0xF, 0 },  // Fx18 - LD ST, Vx
    { "add",    0xF, 0 },  // Fx1E - ADD I, Vx
    { "ld",     0xF, 0 },  // Fx29 - LD F, Vx
    { "ld",     0xF, 0 },  // Fx33 - LD B, Vx
    { "ld",     0xF, 0 },  // Fx55 - LD [I], Vx
    { "ld",     0xF, 0 }   // Fx65 - LD Vx, [I]
};

static const size_t C8_NUM_OPERANDS = sizeof(_optable) / sizeof(c8_optable_t);

/*********************************************************************************/

/*********************************************************************************/

void c8_asm_load(const char* source)
{
    _assembler.line = 0;
    _assembler.column = 0;
    _assembler.source = source;
}

c8_asm_status_t c8_asm_emit(U16* emitted)
{
    if(!_assembler.source)
    {
        // Invalid source, or nothing loaded.
        *emitted = U16_MAX;
        return C8_ASM_STATUS_ERROR;
    }

    char opcode[5]; // 4 max + null term
    memset(opcode, 0, 5);
    int result = sscanf(_assembler.source, "%4s", opcode);

    for(size_t i = 0; i < 5; i++)
    {
        opcode[i] = tolower(opcode[i]);
    }

    if(result == 0)
    {
        // No matches from sscanf().
        *emitted = U16_MAX;
        return C8_ASM_STATUS_ERROR;
    }
    else
    {
        size_t consumed = strlen(opcode);
        _assembler.source += consumed;
        _assembler.column += consumed;
    }

    // Try to find a match
    int found = 0;
    for(size_t i = 0; i < C8_NUM_OPERANDS; i++)
    {
        if(strcmp(opcode, _optable[i].name) == 0)
        {
            *emitted = _optable[i].opcode;
            found = 1;
            break;
        }
    }

    if(!found)
    {
        *emitted = U16_MAX;
        return C8_ASM_STATUS_ERROR;
    }

    return C8_ASM_STATUS_DONE;
}
