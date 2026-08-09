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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "version.h"
#include "repl.h"
#include "platform.h"
#include "common.h"
#include "emulator.h"

enum c8_repl_mode
{
    C8_REPL_MODE_RAW,
    C8_REPL_MODE_ASM,
    C8_REPL_MODE_SCRIPT
};

#define C8_LINE_MAX 1024

static char _buffer[C8_LINE_MAX];
static enum c8_repl_mode _mode = C8_REPL_MODE_RAW;

/*********************************************************************************/

static void c8_reverse_str(char* s) {
    size_t i = 0;
    size_t j = strlen(s);
    if (j == 0) return;
    j--; // last valid character index

    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

/*********************************************************************************/

static void c8_input(void)
{
    switch(_mode)
    {
        case C8_REPL_MODE_RAW:
            printf("(raw): ");
            break;
        case C8_REPL_MODE_ASM:
            printf("(asm): ");
            break;
        case C8_REPL_MODE_SCRIPT:
            printf("(scr): ");
            break;
        default:
            exit(C8_ERROR_INTERNAL_EXCEPTION);
            break;
    }
    if(!fgets(_buffer, sizeof(_buffer), stdin))
    {
        exit(C8_ERROR_INTERNAL_EXCEPTION);
    }
}

static void c8_help(void)
{
    printf("*******************************************\n");
    printf("This is the CHIP-8 interpreter.\n");
    printf("Exit with 'quit' of 'exit'.\n");
    printf("Type 'clear' to clear the console.\n");
    printf("Type 'state' to show the emulator's state.\n");
    printf("Type 'help' to see this message.\n");
    printf("*******************************************\n");
}

/*********************************************************************************/

void c8_repl(void)
{
    c8_help();
    for(;;)
    {
        // Populate "_buffer" w/ user input
        c8_input();
        // Quit
        if(strcmp(_buffer, "quit\n") == 0 ||
            strcmp(_buffer, "exit\n") == 0)
        {
            return;
        }
        // Show state
        if (strcmp(_buffer, "state\n") == 0)
        {
            c8_emu_show_state();
            continue;
        }
        // Clear
        if(strcmp(_buffer, "clear\n") == 0)
        {
            c8_clear_console();
            continue;
        }
        // Help
        if(strcmp(_buffer, "help\n") == 0)
        {
            c8_help();
            continue;
        }
        // Otherwise interpret user input
        // For the raw input, always assume the user
        // is inputting it left-to-right, in other
        // words, big-endian. Since most computers
        // are little-endian, we *should* flip it.
        if (!c8_big_endian())
        {
            c8_reverse_str(_buffer);
        }
        errno = 0;
        char* end = NULL;
        uint64_t input = strtoul(_buffer, &end, 16);\
        if(errno == ERANGE || input > 65535ul)
        {
            printf("Out of range. Enter hex value 0x000...0xFFFF.\n");
            continue;
        }
        if(end == _buffer)
        {
            printf("Input exceeded maximum line length (%d).\n", C8_LINE_MAX);
            continue;
        }
        // Allow only trailing whitespace after the number
        while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') end++;
        if (*end != '\0')
        {
            printf("Extra characters found. Enter only one hex integer 0x0000..0xFFFF.\n");
            continue;
        }

        // Dispatch
        c8_emu_interp((U16)input);
#ifdef C8_DEBUG
        c8_emu_show_state();
#endif
    }
}
