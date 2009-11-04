/*
 *	java_lang_System.c
 *
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 *
 *	This file is part of Darjeeling.
 *
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stddef.h>
#include <string.h>

#include "types.h"

#include "execution.h"
#include "global_id.h"
#include "array.h"
#include "heap.h"
#include "panic.h"
#include "djtimer.h"

#include "pointerwidth.h"

// generated by the infuser
#include "jlib_base.h"

int32_t dj_timer_getTimeMillis();

// int java.lang.System.currentTimeMillis()
void java_lang_System_int_currentTimeMillis()
{
	dj_exec_stackPushInt((uint32_t)dj_timer_getTimeMillis());
}

// void java.lang.System.arraycopy(java.lang.Object, int, java.lang.Object, int, int)
void java_lang_System_void_arraycopy_java_lang_Object_int_java_lang_Object_int_int()
{
	int32_t length = dj_exec_stackPopInt();
	int32_t dst_pos = dj_exec_stackPopInt();
	dj_array * dst = (dj_array *)REF_TO_VOIDP(dj_exec_stackPopRef());
	int32_t src_pos = dj_exec_stackPopInt();
	dj_array * src = (dj_array *)REF_TO_VOIDP(dj_exec_stackPopRef());

	// check for null pointer
	if ((src==nullref)||(dst==nullref))
	{
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
		return;
	}

	// check for out of bounds
	if ((src_pos<0)||(src_pos+length>=src->length)||(dst_pos<0)||(dst_pos+length>=src->length))
	{
		dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);
		return;
	}

	// check types
	if (dj_mem_getChunkId(src)!=dj_mem_getChunkId(dst))
	{
		dj_exec_createAndThrow(BASE_CDEF_java_lang_ArrayStoreException);
		return;
	}

	// integer copy
	if (dj_mem_getChunkId(src)==CHUNKID_INTARRAY)
	{

		dj_int_array *srcint = (dj_int_array*)src;
		dj_int_array *dstint = (dj_int_array*)dst;

		// check if the source/destination arrays are of the same size
		if (srcint->type!=dstint->type)
		{
			dj_exec_createAndThrow(BASE_CDEF_java_lang_ArrayStoreException);
			return;
		}

		// copy
		switch (srcint->type)
		{
			case T_BOOLEAN:
			case T_BYTE:
			case T_CHAR:
				memccpy(dstint->data.bytes+length, srcint->data.bytes+length, length, 1);
			case T_SHORT:
				memccpy(dstint->data.shorts+length, srcint->data.shorts+length, length, 2);
			case T_INT:
			case T_FLOAT:
				memccpy(dstint->data.ints+length, srcint->data.ints+length, length, 4);
			case T_LONG:
			case T_DOUBLE:
				memccpy(dstint->data.longs+length, srcint->data.longs+length, length, 8);
		}


	} else
	{
		dj_ref_array *srcref = (dj_ref_array*)src;
		dj_ref_array *dstref = (dj_ref_array*)dst;

		// test class compatibility
		if (!dj_global_id_testClassType(
				dj_vm_getRuntimeClass(dj_exec_getVM(), dstref->runtime_class_id),
				dj_vm_getRuntimeClass(dj_exec_getVM(), srcref->runtime_class_id)
				))
		{
			dj_exec_createAndThrow(BASE_CDEF_java_lang_ArrayStoreException);
			return;
		}

		// copy references
		memccpy(dstref->refs, srcref->refs, length, sizeof(ref_t));

	}

}
