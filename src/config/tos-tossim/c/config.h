/*
 *	config.h
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
#ifndef __config_h
#define __config_h

// define heap size
#define RUNSIZE 32
#define HEAPSIZE 4096

// define wether to pack structs (this is fine on all AVR targets)
#define PACK_STRUCTS

/* Please see common/debug.h */

//#define DARJEELING_DEBUG
//#define DARJEELING_DEBUG_TRACE
//#define DARJEELING_DEBUG_CHECK_HEAP_SANITY
//#define DARJEELING_DEBUG_PERFILE
#ifndef __TEXT_BUF__
#define __TEXT_BUF__
#include <stdio.h>
char *textBuf;
#endif
//#define DARJEELING_PRINTF(...)

#define DARJEELING_PRINTF(...) textBuf = dj_mem_alloc(100, CHUNKID_REFARRAY);\
							if (textBuf != NULL) { snprintf(textBuf, 65, __VA_ARGS__);\
							tossim_printf(textBuf);\
							dj_mem_free(textBuf);}

#define DARJEELING_PGMSPACE_MACRO



#define MAZANIN_IS_DEBUGGING 0
#if MAZANIN_IS_DEBUGGING
#ifndef __TEXT_BUF__
#define __TEXT_BUF__
#include <stdio.h>
char *textBuf;
#endif
#define MAZANIN_DEBUG(...)  textBuf = dj_mem_alloc(65, CHUNKID_REFARRAY);\
							if (textBuf != NULL) { snprintf(textBuf, 65, __VA_ARGS__);\
							tossim_printf(textBuf);\
							dj_mem_free(textBuf);}
#define MAZANIN_DEBUG_PREFIX
#define MAZANIN_DEBUG_POSTFIX tossim_printf("\n")
//#define MAZANIN_DEBUG(...) printf(__VA_ARGS__)
#else
#define MAZANIN_DEBUG(...)
#endif
#endif
