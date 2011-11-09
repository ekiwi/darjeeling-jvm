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

#include "execution.h"
#include "panic.h"

void dj_panic(int32_t panictype)
{
    switch(panictype)
    {
        case DJ_PANIC_OUT_OF_MEMORY:
            printf("Darjeeling panic: out of memory\n");
            break;
        case DJ_PANIC_ILLEGAL_INTERNAL_STATE:
            printf("Darjeeling panic: illegal internal state\n");
            break;
        case DJ_PANIC_UNIMPLEMENTED_FEATURE:
            printf("Darjeeling panic: unimplemented feature\n");
            break;
        case DJ_PANIC_UNCAUGHT_EXCEPTION:
            printf("Darjeeling panic: uncaught exception\n");
            break;
        case DJ_PANIC_MALFORMED_INFUSION:
            printf("Darjeeling panic: malformed infusion\n");
            break;
        case DJ_PANIC_ASSERTION_FAILURE:
        	printf("Darjeeling panic: Assertion failed\n");
        	break;
        default:
            printf("Darjeeling panic: unknown panic type\n");
            break;
    }
    exit(-1);
}
