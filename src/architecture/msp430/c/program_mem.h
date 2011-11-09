/*
 * program_mem.h
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
 
#ifndef __program_mem__
#define __program_mem__

#include "types.h"

typedef void * dj_di_pointer;

#define DJ_DI_NOT_SET (dj_di_pointer)(-1)
#define DJ_DI_NOT_FOUND (dj_di_pointer)(-2)

#if (PLATFORM==ocapi)

#include "faraccess.h"
#define dj_di_getU8(pointer)  xldb( (unsigned int)(pointer) )
#define dj_di_getU16(pointer) xldb2( (unsigned int)(pointer) )
#define dj_di_getU32(pointer) xldb4( (unsigned int)(pointer) )

#else
#error "No ocapi?!"
#define dj_di_getU8(pointer)  (*(uint8_t*) (pointer))
#define dj_di_getU16(pointer) ( ((*(uint8_t*) (pointer+1))<<8) | (*(uint8_t*) (pointer)) )
#define dj_di_getU32(pointer) (\
	((*(uint8_t*) (pointer+3))<<24) |\
	((*(uint8_t*) (pointer+2))<<16) |\
	((*(uint8_t*) (pointer+1))<<8) |\
	(*(uint8_t*) (pointer)) )

#endif

#define dj_di_getLocalId(pointer) ((dj_local_id){dj_di_getU8(pointer),dj_di_getU8(pointer+1)})

#endif
