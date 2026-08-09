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

#include "version.h"
#include "repl.h"
#include "emulator.h"
#include "log.h"
#include "window.h"

int main(int argc, const char* argv[])
{
    printf("*******************************************\n");
    printf("%s", c8_banner);
    printf("*******************************************\n");

    // chippy [command] (flags) input
    // Commands:
    // > run: Builds & runs an assembly or CHIPSCRIPT file.
    // > build: Only builds an assembly or CHIPSCRIPT file. Outputs as raw binary.
    // > disa: Disassembles a binary input file.
    // Flags:
    // > --debug: Provides a line-by-line debugger during run & build

    // Init
    c8_log("Starting up...");
    c8_log("Endianess identified: %s", (c8_big_endian() ? "big" : "little"));
    c8_emu_init();

    // Create our window.
    // https://discourse.glfw.org/t/multithreading-glfw/573/6
    //GLFWwindow* window = c8_gui_init();
    //if (!window)
    //{
    //    c8_log_error("Could not init GUI. Aborting.");
    //    return EXIT_FAILURE;
    //}

    // No args = interpreter
    if(argc == 1)
    {
        c8_repl();
    }

    // 1 arg = input file
    if (argc == 2)
    {
        // If the file cannot be read, the program
        // terminates
        char* file_in = c8_read_file(argv[1]);

        c8_free(file_in);
    }

    // Parse args
    for(int i = 1; i < argc; i++)
    {
        // ....
    }

    // Deinit
    c8_emu_deinit();
    //c8_gui_deinit(window);
    return EXIT_SUCCESS;
}



