/*
 *	field_instructions.h
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

static inline void GETSTATIC_I()
{
	uint8_t infusion_id = fetch();
	dj_infusion *infusion = dj_infusion_resolve(dj_exec_getCurrentInfusion(), infusion_id);
	pushInt(infusion->staticIntFields[fetch()]);
}

static inline void GETSTATIC_A()
{
	uint8_t infusion_id = fetch();
	dj_infusion *infusion = dj_infusion_resolve(dj_exec_getCurrentInfusion(), infusion_id);
	pushRef(infusion->staticReferenceFields[fetch()]);
}

static inline void GETSTATIC_B()
{
	uint8_t infusion_id = fetch();
	dj_infusion *infusion = dj_infusion_resolve(dj_exec_getCurrentInfusion(), infusion_id);
	pushShort((int8_t)infusion->staticByteFields[fetch()]);
}

static inline void GETSTATIC_C()
{
	GETSTATIC_B();
}

static inline void GETSTATIC_S()
{
	uint8_t infusion_id = fetch();
	dj_infusion *infusion = dj_infusion_resolve(dj_exec_getCurrentInfusion(), infusion_id);
	pushShort((int16_t)infusion->staticShortFields[fetch()]);
}

static inline void PUTSTATIC_I()
{
	uint8_t infusion_id = fetch();
	dj_infusion *infusion = dj_infusion_resolve(dj_exec_getCurrentInfusion(), infusion_id);
	infusion->staticIntFields[fetch()] = popInt();
}

static inline void PUTSTATIC_A()
{
	uint8_t infusion_id = fetch();
	dj_infusion *infusion = dj_infusion_resolve(dj_exec_getCurrentInfusion(), infusion_id);
	infusion->staticReferenceFields[fetch()] = popRef();
}

static inline void PUTSTATIC_B()
{
	uint8_t infusion_id = fetch();
	dj_infusion *infusion = dj_infusion_resolve(dj_exec_getCurrentInfusion(), infusion_id);
	infusion->staticByteFields[fetch()] = (int8_t)popShort();
}

static inline void PUTSTATIC_C()
{
	PUTSTATIC_B();
}

static inline void PUTSTATIC_S()
{
	uint8_t infusion_id = fetch();
	dj_infusion *infusion = dj_infusion_resolve(dj_exec_getCurrentInfusion(), infusion_id);
	infusion->staticShortFields[fetch()] = (int16_t)popShort();
}

static inline void GETFIELD_I()
{
	dj_object *object = REF_TO_VOIDP(popRef());

	if (object==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else if (dj_object_getRuntimeId(object)==CHUNKID_INVALID)
		dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException);
	else {
		uint16_t index = (fetch()<<8) + fetch();
		pushInt( *((int32_t*)((int)object+index)) );
	}

}

static inline void GETFIELD_B()
{
	dj_object *object = REF_TO_VOIDP(popRef());

	if (object==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else if (dj_object_getRuntimeId(object)==CHUNKID_INVALID)
		dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException);
	else {
		uint16_t index = (fetch()<<8) + fetch();
		pushShort( *((int8_t*)((int)object+index)) );
	}

}

static inline void GETFIELD_C()
{
	GETFIELD_B();
}

static inline void GETFIELD_S()
{
	dj_object *object = REF_TO_VOIDP(popRef());

	if (object==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else if (dj_object_getRuntimeId(object)==CHUNKID_INVALID)
		dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException);
	else {
		uint16_t index = (fetch()<<8) + fetch();
		pushShort( *((int16_t*)((int)object+index)) );
	}

}

static inline void GETFIELD_A()
{
	dj_di_pointer classDef;
	dj_object *object = REF_TO_VOIDP(popRef());

	if (object==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else if (dj_object_getRuntimeId(object)==CHUNKID_INVALID)
		dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException);
	else {
		uint16_t index = (fetch()<<8) + fetch();

		// resolve class
		// TODO: is there a faster way to do this?
		classDef = dj_vm_getRuntimeClassDefinition(dj_exec_getVM(), dj_mem_getChunkId(object));

		ref_t* refs =(ref_t*) ( ((char*)object) + dj_di_classDefinition_getOffsetOfFirstReference(classDef));
		pushRef( refs[index] );
	}

}

static inline void PUTFIELD_A()
{
	ref_t value = popRef();
	dj_di_pointer classDef;
	dj_object *object = REF_TO_VOIDP(popRef());

	if (object==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else if (dj_object_getRuntimeId(object)==CHUNKID_INVALID)
		dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException);
	else
	{
		uint16_t index = (fetch()<<8) + fetch();

		// resolve class
		// TODO: is there a faster way to do this?
		classDef = dj_vm_getRuntimeClassDefinition(dj_exec_getVM(), dj_mem_getChunkId(object));

        ref_t* refs =(ref_t*)( ((char*)object) + dj_di_classDefinition_getOffsetOfFirstReference(classDef) );
        refs[index] = value;
	}

}

static inline void PUTFIELD_I()
{
	int32_t value = popInt();
	dj_object *object = REF_TO_VOIDP(popRef());

	if (object==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else if (dj_object_getRuntimeId(object)==CHUNKID_INVALID)
		dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException);
	else
	{
		uint16_t index = (fetch()<<8) + fetch();
		*(int32_t*)((char*)object+index) = value;
	}


}

static inline void PUTFIELD_B()
{
	int8_t value = popShort();
	dj_object *object = REF_TO_VOIDP(popRef());

	if (object==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else if (dj_object_getRuntimeId(object)==CHUNKID_INVALID)
		dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException);
	else
	{
		uint16_t index = (fetch()<<8) + fetch();
		*(int8_t*)((char*)object+index) = value;
	}

}

static inline void PUTFIELD_C()
{
	PUTFIELD_B();
}

static inline void PUTFIELD_S()
{
	int16_t value = popShort();
	dj_object *object = REF_TO_VOIDP(popRef());

	if (object==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else if (dj_object_getRuntimeId(object)==CHUNKID_INVALID)
		dj_exec_createAndThrow(BASE_CDEF_javax_darjeeling_vm_ClassUnloadedException);
	else
	{
		uint16_t index = (fetch()<<8) + fetch();
		*(int16_t*)((char*)object+index) = value;
	}
}
