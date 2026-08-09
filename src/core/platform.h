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

#ifndef C8_H_PLATFORM
#define C8_H_PLATFORM

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// -- WINDOWS --
#if defined(_WIN32) || defined(_WIN64)
#define C8_PLATFORM_WINDOWS (1)
#endif

// -- Mac OS --
#if defined(__APPLE__) || defined(__MACH__)
#define C8_PLATFORM_MACOS (1)
#endif

// -- Unix --
#if defined(__linux__)
#define C8_PLATFORM_LINUX (1)
#endif

#if defined(__FreeBSD__)
#define C8_PLATFORM_FREEBSD (1)
#endif

#if defined(__unix__) || defined(__unix)
#define C8_PLATFORM_UNIX (1)
#endif

/**********************************************************************************/

// Clears the console.
void c8_clear_console(void);

// Platform-independent fopen(). Essentially,
// uses fopen_s() on windows, and fopen() on all
// other platforms. Feel free to use fclose().
// Returns an error code, or (0) on success.
int32_t c8_fopen(FILE** fileOut, const char* path, const char* mode);

// Reads a file's contents. Returns
// a memory-allocated buffer, 
// null-terminated. If it cannot be read,
// this function will terminate the program.
char* c8_read_file(const char* path);

// Better version of std free().
void c8_free(void* ptr);

// Returns true if the 
// platform is big-endian,
// false if little-endian.
bool c8_big_endian(void);

#endif // C8_H_PLATFORM
