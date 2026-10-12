#####################################################################################
# Chippy CHIP-8 Emulator
# Copyright (c) 2026 Kyle Morris.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#####################################################################################

# Must be 1st
cmake_minimum_required(VERSION 3.10)

# The project definition
project(chippy VERSION 0.0
    DESCRIPTION "Chippy CHIP-8 Emulator"
    HOMEPAGE_URL https://github.com/kylemmorris/chippy)

# Currently using C17
set(CMAKE_C_STANDARD 17)

# Add all source files
file(GLOB_RECURSE sources src/*.c src/*.h)

# Create the executable
add_executable(chippy ${sources})

# Compiler flags
target_compile_options(chippy PUBLIC -Wall)

# Includes
target_include_directories(chippy PUBLIC src)

# Dependency: GLFW
if(NOT TARGET glfw)
	# Set some flags
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

	# Get content from GitHub
	include(FetchContent)
	FetchContent_Declare(
		glfw
		GIT_REPOSITORY https://github.com/glfw/glfw.git
		GIT_SHALLOW 1)
	FetchContent_MakeAvailable(glfw)
endif()

# Link against GLFW
target_link_libraries(chippy PRIVATE glfw)
