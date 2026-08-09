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

#ifndef C8_H_VERSION
#define C8_H_VERSION

#include "platform.h"

/**********************************************************************************/

#define C8_VERSION "0.1"
#define C8_COPYRIGHT "Copyright (c) 2026 Kyle Morris."

// TODO: Enable this for debug, disable otherwise
#define C8_DEBUG 1

static const char* const c8_banner =
    "Chippy CHIP-8 Emulator v" C8_VERSION
    #if C8_PLATFORM_WINDOWS
    " (Windows)"
    #elif C8_PLATFORM_MACOS
    " (Mac OS)"
    #elif C8_PLATFORM_LINUX
    " (Linux)"
    #elif C8_PLATFORM_FREEBSD
    " (FreeBSD)"
    #elif C8_PLATFORM_UNIX
    " (Unix)"
    #endif
    "\n" C8_COPYRIGHT "\n";

#endif // C8_H_VERSION
