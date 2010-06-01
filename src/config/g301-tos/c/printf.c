#include "stdio.h"
#include "string.h"

int nesc_printf(char*);

void g301_serialVPrint(char * format, va_list arg)
{
	char temp[128];
	vsnprintf(temp, 128, format, arg);
	nesc_printf(temp);
}

void g301_serialPrintf(char * format, ...)
{
	va_list arg;

	va_start(arg, format);
	g301_serialVPrint(format, arg);
	va_end(arg);

}

