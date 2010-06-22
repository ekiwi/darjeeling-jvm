/*
 * panic.c
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
#include <stdlib.h>
#include <stdint.h>
#include "panic.h"
#include "nesc.h"

#include "config.h"
//TODO: check if exit (-1) terminates the node
void dj_panic(int32_t panictype)
{
	switch(panictype)
	{
		case DJ_PANIC_OUT_OF_MEMORY:
			nesc_printf("PANIC: OUT OF MEMORY\n");
			break;
		case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
			nesc_printf("PANIC: ILLEGAL INTERNAL STATE\n");
			break;
		case DJ_PANIC_UNIMPLEMENTED_FEATURE:
			nesc_printf("PANIC: UNIMPLEMENTED FEATURE\n");
			break;
		case DJ_PANIC_UNCAUGHT_EXCEPTION:
			nesc_printf("PANIC: UNCAUGHT EXCEPTION\n");
			break;
		default:
			nesc_printf("PANIC: UNKNOWN TYPE\n");
			break;
	}
//	exit(-1);
	while(1) {};
}
