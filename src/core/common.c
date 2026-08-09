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

#include "common.h"
#include "c8errors.h"

/*********************************************************************************/

void c8_nibbles(U16 instruction, U8* n0_msn, U8* n1, U8* n2, U8* n3_lsn)
{
	// Mask the nibble we want, and
	// then shift it to the least-significant
	// nibble (lsn) to get the final value into
	// a U8.
	if (c8_big_endian())
	{
		// Big-Endian
		// 0000 0000 0000 0000 [16 bits]
		// ^msn                [ 4 bits]
		*n3_lsn = (U8)(instruction & 0x000F);
		*n2     = (U8)((instruction & 0x00F0) >> 4);
		*n1     = (U8)((instruction & 0x0F00) >> 8);
		*n0_msn = (U8)((instruction & 0xF000) >> 12);
	}
	else
	{
		// Little-Endian
		// 0000 0000 0000 0000 [16 bits]
		//                ^msn [ 4 bits]
		*n0_msn = (U8)(instruction & 0x000F);
		*n1     = (U8)((instruction & 0x00F0) >> 4);
		*n2     = (U8)((instruction & 0x0F00) >> 8);
		*n3_lsn = (U8)((instruction & 0xF000) >> 12);
	}
}
