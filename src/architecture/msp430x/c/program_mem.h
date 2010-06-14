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
//typedef uint16_t dj_di_pointer;

#define DJ_DI_NOT_SET ((void*)-1)
#define DJ_DI_NOT_FOUND ((void*)-2)


uint8_t getFarU8(dj_di_pointer pointer);
uint16_t getFarU16(dj_di_pointer pointer);
#define dj_di_getU8(pointer) (getFarU8(pointer))
#define dj_di_getU16(pointer)(getFarU8((void*)(pointer)+1)<<8 | getFarU8(pointer))
/*
#define dj_di_getU32(pointer) (\
                dj_di_getU8((uint32_t)(pointer)+0x3)<<24 |\
                dj_di_getU8((uint32_t)(pointer)+0x2)<<16 |\
                dj_di_getU8((uint32_t)(pointer)+0x1)<<8 |\
                dj_di_getU8((uint32_t)(pointer)+0x0)
*/
#define dj_di_getLocalId(pointer) ((dj_local_id){dj_di_getU8(pointer),dj_di_getU8(pointer+1)})

#endif
