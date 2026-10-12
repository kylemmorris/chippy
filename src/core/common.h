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

#ifndef C8_H_COMMON
#define C8_H_COMMON

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "platform.h"

/*
 * U8 = 0000 0000 (binary)
 * U8 = 0x00      (hex)
 *
 * 0000 = 0
 * 0001 = 1
 * 0010 = 2
 * 0011 = 3
 * 0100 = 4
 * 0101 = 5
 * 0110 = 6
 * 0111 = 7
 * 1000 = 8
 * 1001 = 9
 * 1010 = A
 * 1011 = B
 * 1100 = C
 * 1101 = D
 * 1110 = E
 * 1111 = F
 */

/*********************************************************************************/

// Unsigned 8-bit integer [1 byte / 2 nibbles] 0 to 255
typedef uint8_t U8;

// Max value of an 8-bit uint.
#define U8_MAX ((U8)255)

// Hex format for U8s
#define U8_HEX_FMT "0x%02X"

// Unsigned 16-big integer [2 bytes / 4 nibbles] 0 to 65536
typedef uint16_t U16;

// Max value of a 16-bit uint.
#define U16_MAX ((U16)65525)

// Hex format for U16s
#define U16_HEX_FMT "0x%04X"

/*********************************************************************************/

// Error thrown when the input given to
// the Chippy program is invalid.
#define C8_ERROR_INVALID_INPUT          (0x1110)

// Error thrown when some part of the
// interpreter or program tries to access
// memory outside the space provided.
#define C8_ERROR_INVALID_MEMORY         (0x1111)

// Error thrown when there is some
// exceptional internal issue.
#define C8_ERROR_INTERNAL_EXCEPTION     (0x2222)

// Error thrown when an
// assertion fails.
#define C8_ERROR_ASSERT_FAILURE         (0x3331)

/*********************************************************************************/

// Asserts the condition.
void c8_assert(bool condition);

// Asserts n is between low and
// high (inclusive).
void c8_assert_range(int n, int low, int high);

#endif // C8_H_COMMON
