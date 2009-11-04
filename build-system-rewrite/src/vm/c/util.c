/*
 *	util.c
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

#include "types.h"

// platform-spefific header file
#include "program_mem.h"

/**
 * Compares two strings. Both strings must be in program memory (the location where .di files are stored).
 * @param str1 a string in program memory.
 * @param str2 a string in program memory.
 * @return true if both strings are equal, false otherwise
 */
char dj_di_strEquals(dj_di_pointer str1, dj_di_pointer str2)
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
