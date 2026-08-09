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
#include <time.h>

#include "log.h"
#include "platform.h"

static const char* const _logFileName = "chippy_log.txt";
static const char* const _timeFormat = "%X";

static void _log(const char* type, const char* fmt, va_list args)
{
    static FILE* _logFile = NULL;
    static int _firstCall = 1;

    static char _timeBuf[80];
    static time_t _now;
    static struct tm _tstruct;

    // Create copy for later
    va_list _forcout;
    va_copy(_forcout, args);

    // Open file
    int32_t err = c8_fopen(&_logFile, _logFileName, _firstCall ? "w" : "a");
    if(err)
    {
        perror("ERROR: _log(): fopen() failed!");
        fclose(_logFile);
        return;
    }
    _firstCall = 0;
    int errCode = 0;

    // Write: "time [type] msg\n"
    // -- time + type
    _now = time(0);
    _tstruct = *localtime(&_now);
    strftime(_timeBuf, sizeof(_timeBuf), _timeFormat, &_tstruct);
    errCode = fprintf(_logFile, "%s [%s] ", _timeBuf, type);
    if(errCode < 0)
    {
        perror("ERROR: _log(): fprintf() produced a negative value!");
        fclose(_logFile);
        return;
    }
    errCode = fprintf(stdout, "[%s] ", type); // also write to stdout
    if(errCode < 0)
    {
        perror("ERROR: _log(): fprintf() produced a negative value!");
        fclose(_logFile);
        return;
    }
    //  -- rest of message
    errCode = vfprintf(_logFile, fmt, args);
    if(errCode < 0)
    {
        perror("ERROR: _log(): vfprintf() produced a negative value!");
        fclose(_logFile);
        return;
    }
    errCode = vfprintf(stdout, fmt, _forcout);
    if(errCode < 0)
    {
        perror("ERROR: _log(): vfprintf() produced a negative value!");
        fclose(_logFile);
        return;
    }

    fprintf(_logFile, "\n");
    fprintf(stdout, "\n");
    fclose(_logFile);
}

/**********************************************************************************/

void c8_log(const char* msgFmt, ...)
{
    va_list args;
    va_start(args, msgFmt);
    _log("INFO", msgFmt, args);
    va_end(args);
}

void c8_log_warn(const char* msgFmt, ...)
{
    va_list args;
    va_start(args, msgFmt);
    _log("WARN", msgFmt, args);
    va_end(args);
}

void c8_log_error(const char* msgFmt, ...)
{
    va_list args;
    va_start(args, msgFmt);
    _log("!ERR", msgFmt, args);
    va_end(args);
}
