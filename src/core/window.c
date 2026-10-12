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

#include "log.h"
#include "version.h"
#include "window.h"

#define C8_WINDOW_SIZE (800)

/*********************************************************************************/

static void c8_err_callback(int error, const char* description)
{
	c8_log_error("GLFW Error (%d): %s", error, description);
}

/*********************************************************************************/

GLFWwindow* c8_gui_init(void)
{
	if (!glfwInit())
	{
		return NULL;
	}

	glfwSetErrorCallback(c8_err_callback);

	return glfwCreateWindow(C8_WINDOW_SIZE, C8_WINDOW_SIZE, "Chippy - C8 Emulator v" C8_VERSION, NULL, NULL);
}

void c8_gui_deinit(GLFWwindow* window)
{
	glfwDestroyWindow(window);

	glfwTerminate();
}
