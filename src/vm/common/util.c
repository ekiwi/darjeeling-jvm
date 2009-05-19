
#include "common/types.h"

// platform-spefific header file
#include "program_mem.h"

/**
 * Compares two strings. Both strings must be in program memory (the location where .di files are stored).
 * @param str1 a string in program memory.
 * @param str2 a string in program memory.
 * @return true if both strings are equal, false otherwise
 */
char dj_str_equals(dj_di_pointer str1, dj_di_pointer str2)
{
	uint8_t a,b;
	do {
		// TODO replace with dj_di_fetchU8() ?
		a = dj_di_getU8(str1); str1++;
		b = dj_di_getU8(str2); str2++;
		if (a!=b) return 0;
	} while ((a!=0)&&(b!=0));
	return 1;
}
