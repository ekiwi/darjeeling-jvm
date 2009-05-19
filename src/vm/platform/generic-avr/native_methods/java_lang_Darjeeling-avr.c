#include <stdio.h>
#include <avr/pgmspace.h>

#include "common/execution/execution.h"

// void java.lang.Darjeeling.assertTrue(int, boolean)
void java_lang_Darjeeling_void_assertTrue_int_boolean()
{
	// pop argument from the stack
	int32_t value = dj_exec_stackPopInt();
	int32_t id = dj_exec_stackPopInt();
	if (value==0)
		printf_P(PSTR("ASSERT[%3d] FAILED\n"), id);
	else
		printf_P(PSTR("ASSERT[%3d] PASSED\n"), id);
}

// void java.lang.Darjeeling.print(java.lang.String)
void java_lang_Darjeeling_void_print_java_lang_String()
{
	char *str = (char*)dj_exec_stackPopRef();
	printf_P(PSTR("%s"), str);
}

// void java.lang.Darjeeling.print(int)
void java_lang_Darjeeling_void_print_int()
{
	int32_t value = dj_exec_stackPopInt();
	printf_P(PSTR("%d"), value);
}

///////////// the following should one day go to javax.fleck

// int java.lang.Darjeeling.getNodeId()
void java_lang_Darjeeling_int_getNodeId()
{
	dj_exec_stackPushInt(0);
}

// int java.lang.Darjeeling.getTemperature()
void java_lang_Darjeeling_int_getTemperature()
{
	dj_exec_stackPushInt(0);
}

// int java.lang.Darjeeling.getSecond()
void java_lang_Darjeeling_int_getSecond()
{
	dj_exec_stackPushInt(0);
}

// int java.lang.Darjeeling.getMinute()
void java_lang_Darjeeling_int_getMinute()
{
	dj_exec_stackPushInt(0);
}

// int java.lang.Darjeeling.getHour()
void java_lang_Darjeeling_int_getHour()
{
	dj_exec_stackPushInt(0);
}

// int java.lang.Darjeeling.getBatteryVoltage()
void java_lang_Darjeeling_int_getBatteryVoltage()
{
	dj_exec_stackPushInt(0);
}

// int java.lang.Darjeeling.getSolarVoltage()
void java_lang_Darjeeling_int_getSolarVoltage()
{
	dj_exec_stackPushInt(0);
}

// int java.lang.Darjeeling.getSolarCurrent()
void java_lang_Darjeeling_int_getSolarCurrent()
{
	dj_exec_stackPushInt(0);
}

// int java.lang.Darjeeling.getADC()
void java_lang_Darjeeling_int_getADC_short()
{
	// pop byte argument off the stack
	int32_t arg = dj_exec_stackPopInt();
	dj_exec_stackPushInt(0);
}
