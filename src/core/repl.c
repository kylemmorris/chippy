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
#include "vm.h"
#include "assembler.h"

typedef enum c8_repl_mode
{
    C8_REPL_MODE_RAW,
    C8_REPL_MODE_ASM,
    C8_REPL_MODE_SCRIPT
} c8_repl_mode_t;

#define C8_LINE_MAX 1024

static char _user_input[C8_LINE_MAX];
static c8_repl_mode_t _mode = C8_REPL_MODE_RAW;

/*********************************************************************************/

static void c8_reverse_string(char* string) 
{
    size_t left = 0;
    size_t right = strlen(string);

    if (right == 0)
    {
        return;
    }
    // Last valid character (non-null term)
    right--; 

    char temp;
    while (left < right)
    {
        temp = string[left];
        string[left] = string[right];
        string[right] = temp;
        left++;
        right--;
    }
}

static void c8_get_user_input(void)
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
    if(!fgets(_user_input, sizeof(_user_input), stdin))
    {
        exit(C8_ERROR_INTERNAL_EXCEPTION);
    }
}

static void c8_handle_raw(void)
{
    // For the raw input, always assume the user
    // is inputting it left-to-right, in other
    // words, big-endian. Since most computers
    // are little-endian, we *should* flip it.
    if (!c8_big_endian())
    {
        c8_reverse_string(_user_input);
    }
    errno = 0;
    char* end = NULL;
    uint64_t input = strtoul(_user_input, &end, 16);
    if (errno == ERANGE || input > 65535ul)
    {
        printf("Out of range. Enter hex value 0x000...0xFFFF.\n");
        return;
    }
    if (end == _user_input)
    {
        printf("Input exceeded maximum line length (%d).\n", C8_LINE_MAX);
        return;
    }
    // Allow only trailing whitespace after the number
    while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')
    {
        end++;
    }
    if (*end != '\0')
    {
        printf("Extra characters found. Enter only one hex integer 0x0000..0xFFFF.\n");
        return;
    }

    // Dispatch
    c8_vm_interp((U16)input);
#ifdef C8_DEBUG
    c8_vm_show_state();
#endif
}

static void c8_handle_asm(void)
{
    c8_asm_load(_user_input);
    c8_asm_status_t status = C8_ASM_STATUS_OK;
    U16 result = 0;

    status = c8_asm_emit(&result);
    printf("> status: %d; result: " U8_HEX_FMT "\n", status, result);

    #if 0
    while (1)
    {
        status = c8_asm_emit(&result);
        switch (status)
        {
            case C8_ASM_STATUS_OK:
            {
                c8_vm_interp(result);
                #ifdef C8_DEBUG
                c8_vm_show_state();
                #endif
                break;
            }
            case C8_ASM_STATUS_DONE:
            {
                return;
            }
            case C8_ASM_STATUS_ERROR:
            {
                return;
            }
            default:
            {
                return;
            }
        }
    }
    #endif
}

static void c8_help(void)
{
    printf("*******************************************\n");
    printf("This is the CHIP-8 interpreter.\n");
    printf("Exit with 'quit' or 'exit'.\n");
    printf("Type 'clear' to clear the console.\n");
    printf("Type 'state' to show the emulator's state.\n");
    printf("Type 'help' to see this message.\n");
    printf("Type 'mode' to switch input modes.\n");
    printf("*******************************************\n");
}

/*********************************************************************************/

void c8_repl()
{
    c8_help();
    while(1)
    {
        c8_get_user_input();
        // Quit
        if(strcmp(_user_input, "quit\n") == 0 ||
           strcmp(_user_input, "exit\n") == 0)
        {
            return;
        }
        // Show state
        if (strcmp(_user_input, "state\n") == 0)
        {
            c8_vm_show_state();
            continue;
        }
        // Clear
        if(strcmp(_user_input, "clear\n") == 0)
        {
            c8_clear_console();
            continue;
        }
        // Help
        if(strcmp(_user_input, "help\n") == 0)
        {
            c8_help();
            continue;
        }
        // Mode
        if (strcmp(_user_input, "mode\n") == 0)
        {
            switch (_mode)
            {
                case C8_REPL_MODE_RAW:      
                    _mode = C8_REPL_MODE_ASM; 
                    break;
                case C8_REPL_MODE_ASM:      
                    _mode = C8_REPL_MODE_SCRIPT; 
                    break;
                case C8_REPL_MODE_SCRIPT:   
                    _mode = C8_REPL_MODE_RAW; 
                    break;
                default: break;
            }
            continue;
        }
        
        // Otherwise interpret user input
        switch (_mode)
        {
            case C8_REPL_MODE_RAW:
            {
                c8_handle_raw();
                break;
            }
            case C8_REPL_MODE_ASM:
            {
                c8_handle_asm();
                break;
            }
            case C8_REPL_MODE_SCRIPT:
            {
                printf("Script mode not supported.\n");
                break;
            }
            default:
            {
                printf("Unidentified mode: %d.\n", (int)_mode);
                break;
            }
        }
    }
}
