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

#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "platform.h"
#include "log.h"

/*********************************************************************************/


void c8_clear_console(void)
{
#if C8_PLATFORM_WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

int32_t c8_fopen(FILE** fileOut, const char* path, const char* mode)
{
#ifdef C8_PLATFORM_WINDOWS
    return (int32_t)fopen_s(fileOut, path, mode);
#else
    errno = 0;
    *fileOut = fopen(path, mode);
    if (fileOut == NULL)
    {
        return errno;
    }
#endif
}

char* c8_read_file(const char* path)
{
    // Open file - depends on OS the best 
    // version of fopen() to use.
    FILE* file;
    int32_t err = c8_fopen(&file, path, "rb");
    if (err)
    {
        printf("Unable to open file at \"%s\". Received error code %d.", path, err);
        fclose(file);
        exit(C8_ERROR_INVALID_INPUT);
    }

    // Get size of file
    fseek(file, 0L, SEEK_END);
    size_t file_sz = ftell(file);
    rewind(file);

    // Allocate buffer
    char* out = (char*)malloc(file_sz + 1);
    if (out == NULL)
    {
        c8_log_error("c8_read_file(): malloc() failed!");
        exit(C8_ERROR_INTERNAL_EXCEPTION);
    }
    size_t bytes_read = fread(out, sizeof(char), file_sz, file);
    out[bytes_read] = '\0'; // null term
    
    // Return
    fclose(file);
    return out;
}

void c8_free(void* ptr)
{
    if (ptr)
    {
        free(ptr);
        ptr = NULL;
    }
}

bool c8_big_endian(void)
{
    // On BE: 0x1000
    // On LE: 0x0001
    int test = 1;
    return ! *((char*)&test);
}