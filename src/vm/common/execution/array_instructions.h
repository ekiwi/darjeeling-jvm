/**
 * Executes the NEWARRAY instruction. The array type is fetched from code (uint8_t) and the length is
 * popped from the stack. Note that only types T_BOOLEAN, T_BYTE, T_SHORT and T_INT are currently
 * supported. An array of references is created with the ANEWARRAY instruction.
 *
 */
static inline void NEWARRAY()
{
	dj_int_array *arr = dj_int_array_create(fetch(), popShort());

	if (arr==nullref)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_OutOfMemoryError);
	else
		pushRef(VOIDP_TO_REF(arr));
}

static inline void ANEWARRAY()
{
	dj_local_id classLocalId = dj_fetchLocalId();
	dj_global_id classGlobalId = dj_global_id_resolve(dj_exec_getCurrentInfusion(), classLocalId);
	uint16_t id = dj_global_id_getRuntimeClassId(classGlobalId);
	dj_ref_array *arr = dj_ref_array_create(id, popShort());

	if (arr==nullref)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_OutOfMemoryError);
	else
		pushRef(VOIDP_TO_REF(arr));
}

/**
 * Executes the ARRAYLENGTH instruction. A reference to an array is popped from the stack, and the length
 * of that array is pushed into the stack.
 *
 */
static inline void ARRAYLENGTH()
{
	dj_array *array = REF_TO_VOIDP(popRef());
	if (array==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		pushShort(array->length);
}

/**
 * Executes the BASTORE instruction. A byte value, index and array reference are popped from the stack.
 * The byte value is then stored in the array at index. When index is out of bounds,  (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void BASTORE()
{
	uint8_t value = popShort();
	uint32_t index = popInt();
	dj_int_array *arr = REF_TO_VOIDP(popRef());
	if (arr==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		if ((index>=0) && (index<((dj_array*)arr)->length))
			arr->data.bytes[index] = value;
		else
			dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);
}

/**
 * Executes the CASTORE instruction. A char value, index and array reference are popped from the stack.
 * The char value is then stored in the array at index. When index is out of bounds,  (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void CASTORE()
{
	BASTORE();
}

/**
 * Executes the BALOAD instruction. And index and array are popped from the stack. The byte value in the
 * array at index is then pushed on the stack. When index is out of bounds, (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void BALOAD()
{
	uint32_t index = popInt();
	dj_int_array *arr = REF_TO_VOIDP(popRef());

	if (arr==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		if ((index>=0) && (index<((dj_array*)arr)->length))
			pushShort((int16_t)arr->data.bytes[index]);
		else
			dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);

}

/**
 * Executes the CALOAD instruction. And index and array are popped from the stack. The char value in the
 * array at index is then pushed on the stack. When index is out of bounds, (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void CALOAD()
{
	BALOAD();
}

/**
 * Executes the SASTORE instruction. A short value, index and array reference are popped from the stack.
 * The short value is then stored in the array at index. When index is out of bounds,  (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void SASTORE()
{
	int16_t value = popShort();
	uint32_t index = popInt();
	dj_int_array *arr = REF_TO_VOIDP(popRef());

	if (arr==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		if ((index>=0) && (index<((dj_array*)arr)->length))
			arr->data.shorts[index] = value;
		else
			dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);
}

/**
 * Executes the SALOAD instruction. And index and array are popped from the stack. The short value in the
 * array at index is then pushed on the stack. When index is out of bounds, (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void SALOAD()
{
	uint32_t index = popInt();
	dj_int_array *arr = REF_TO_VOIDP(popRef());

	if (arr==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		if ((index>=0) && (index<((dj_array*)arr)->length))
			pushShort((int16_t)arr->data.shorts[index]);
		else
			dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);

}

/**
 * Executes the IASTORE instruction. An integer value, index and array reference are popped from the stack.
 * The integer value is then stored in the array at index. When index is out of bounds,  (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void AASTORE()
{
	ref_t value = popRef();
	uint32_t index = popInt();
	dj_ref_array *arr = REF_TO_VOIDP(popRef());

	if (arr==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		if ((index>=0) && (index<((dj_array*)arr)->length))
			arr->refs[index] = value;
		else
			dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);

}

/**
 * Executes the IASTORE instruction. An integer value, index and array reference are popped from the stack.
 * The integer value is then stored in the array at index. When index is out of bounds,  (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void IASTORE()
{
	int32_t value = popInt();
	uint32_t index = popInt();
	dj_int_array *arr = REF_TO_VOIDP(popRef());

	if (arr==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		if ((index>=0) && (index<((dj_array*)arr)->length))
			arr->data.ints[index] = value;
		else
			dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);

}

/**
 * Executes the IALOAD instruction. And index and array are popped from the stack. The integer value in the
 * array at index is then pushed on the stack. When index is out of bounds, (outside
 * the [0..size-1] range) throwOutOfBoundsException() is called to handle the exception.
 */
static inline void IALOAD()
{
	uint32_t index = popInt();
	dj_int_array *arr = REF_TO_VOIDP(popRef());

	if (arr==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		if ((index>=0) && (index<((dj_array*)arr)->length))
			pushInt((int32_t)arr->data.ints[index]);
		else
			dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);
}


static inline void AALOAD()
{
	uint32_t index = popInt();
	dj_ref_array *arr = REF_TO_VOIDP(popRef());

	if (arr==NULL)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);
	else
		if ((index>=0) && (index<((dj_array*)arr)->length))
			pushRef(arr->refs[index]);
		else
			dj_exec_createAndThrow(BASE_CDEF_java_lang_IndexOutOfBoundsException);
}
