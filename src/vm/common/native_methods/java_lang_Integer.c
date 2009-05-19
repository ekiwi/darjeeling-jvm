#include <string.h>
#include <stdio.h>

#include "common/heap/heap.h"
#include "common/execution/execution.h"
#include "common/panic.h"

#include "base_definitions.h"

// java.lang.String java.lang.Integer.toString(int)
void java_lang_Integer_java_lang_String_toString_int()
{
	char temp[8];
	char *str;
	int32_t value = dj_exec_stackPopInt();
	sprintf(temp,"%ld", (long)value);
	str = dj_mem_alloc(strlen(temp)+1, dj_vm_getSysLibClassRuntimeId(dj_exec_getVM(), BASE_CDEF_java_lang_String));

	if(str == NULL)
	{
    	dj_exec_createAndThrow(BASE_CDEF_java_lang_OutOfMemoryError);
    	return;
	}

	strcpy(str, temp);
	dj_exec_stackPushRef(VOIDP_TO_REF(str));
}
