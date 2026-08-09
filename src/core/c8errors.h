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

#ifndef C8_H_ERRORS
#define C8_H_ERRORS

#include <stdbool.h>

 /*********************************************************************************/

// Error thrown when the input given to
// the Chippy program is invalid.
#define C8_ERROR_INVALID_INPUT (0x1110)

// Error thrown when some part of the
// interpreter or program tries to access
// memory outside the space provided.
#define C8_ERROR_INVALID_MEMORY (0x1111)

// Error thrown when there is some
// exceptional internal issue.
#define C8_ERROR_INTERNAL_EXCEPTION (0x2222)

// Error thrown when an 
// assertion fails.
#define C8_ERROR_ASSERT_FAILURE (0x3331)

/*********************************************************************************/

// Asserts the condition.
void c8_assert(bool condition);

// Asserts n is between low and 
// high (inclusive).
void c8_assert_range(int n, int low, int high);

#endif // C8_H_ERRORS
