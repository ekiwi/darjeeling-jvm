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
#define RUNSIZE 128
#define HEAPSIZE 8192

// define wether to pack structs (this is fine on all AVR targets)
// don't pack structs on MSP430 targets
// #define PACK_STRUCTS
#define ALIGN_16

// #define HAS_USART

/* Please see common/debug.h */
//#define DARJEELING_DEBUG
//#define DARJEELING_DEBUG_TRACE
//#define DARJEELING_DEBUG_CHECK_HEAP_SANITY
//#define DARJEELING_DEBUG_PERFILE
#define DARJEELING_PRINTF printf_P
#define DARJEELING_PGMSPACE_MACRO PSTR

#define IS_COOJA

#define MAZANIN_IS_DEBUGGING 0
#if MAZANIN_IS_DEBUGGING
#include "stdio.h"
extern FILE *logFile;
#define MAZANIN_DEBUG(...) fprintf(logFile, __VA_ARGS__)
#define MAZANIN_DEBUG_PREFIX fprintf(logFile, "(DEBUG in node %d)=> ", node_id)
#define MAZANIN_DEBUG_POSTFIX fprintf(logFile, "\n")
//#define MAZANIN_DEBUG(...) printf(__VA_ARGS__)
#else
#define MAZANIN_DEBUG(...)
#endif

#endif
