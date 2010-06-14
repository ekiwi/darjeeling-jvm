/*
 * javax_darjeeling_Darjeeling.c
 * 
 * Copyright (c) 2008-2010 CSIRO, Delft University of Technology.
 * 
 * This file is part of Darjeeling.
 * 
 * Darjeeling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Darjeeling is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// generated at infusion time
#include "jlib_base.h"

#include "execution.h"
#include "heap.h"
#include "djtimer.h"
#include "panic.h"
#include "array.h"

// void javax.darjeeling.Darjeeling.assertTrue(int, boolean)
void javax_darjeeling_Darjeeling_void_assertTrue_int_boolean()
{
	// pop argument from the stack
	int32_t value = dj_exec_stackPopShort();
	int32_t id = dj_exec_stackPopInt();
	if (value==0)
		printf("%c[31mASSERT[%3d] FAILED%c[0m\n", 0x1b, (int)id, 0x1b);
	else
		printf("%c[32mASSERT[%3d] PASSED%c[0m\n", 0x1b, (int)id, 0x1b);
}

// void javax.darjeeling.Darjeeling.printBytesAsString(byte[])
void javax_darjeeling_Darjeeling_void_printBytesAsString_byte__()
{
	dj_int_array* byteStr = REF_TO_VOIDP(dj_exec_stackPopRef());
	printf("%s", byteStr->data.bytes);
}

// int javax.darjeeling.Darjeeling.getNodeId()
void javax_darjeeling_Darjeeling_int_getNodeId()
{
	dj_exec_stackPushInt(0);

}
