/*
 * panic.h
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
 

#ifndef __panic_h
#define __panic_h

#define DJ_PANIC_OUT_OF_MEMORY              42
#define DJ_PANIC_ILLEGAL_INTERNAL_STATE     43
#define DJ_PANIC_UNIMPLEMENTED_FEATURE      44
#define DJ_PANIC_UNCAUGHT_EXCEPTION         45
#define DJ_PANIC_UNSATISFIED_LINK			46
#define DJ_PANIC_MALFORMED_INFUSION			47
#define DJ_PANIC_ASSERTION_FAILURE			48
#define DJ_PANIC_SAFE_POINTER_OVERFLOW		49

void dj_panic(int32_t panictype);

#endif
