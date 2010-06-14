/*
 * debug.h
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
 
#ifndef __debug_h
#define __debug_h

/* =================================================

   A few macros to ease debugging.

   =================================================

   Printing of  Debbuging information can be enabled  by #defining the
   DARJEELING_DEBUG maro  (either in the  config.h header, or  file by
   file, to trace specific aspects).

   Note:    if   per-file    debugging   is    desired,    the   macro
   DARJEELING_DEBUG_PERFILE must  be #defined in config.h  so that the
   buffers of config.c get actually declared.


   DEBUG_LOG is  to be  used like "printf",  but will indent  the code
   according   to    previous   calls   of    DEBUG_ENTER_NEST()   and
   DEBUG_EXIT_NEST().


   DEBUG_ENTER_NEST() and DEBUG_EXIT_NEST() print out a message
   (typically, DEBUG_WHEREAMI) and update the current indentation
   level

   DEBUG_WHEREAMI resoves to a string of the form __FILE__:__LINE__

   DEBUG_LOG_WHEREAMI() nicely  calls DEBUG_LOG with  a DEBUG_WHEREAMI
   argument and takes care of the newline


   Please note than this debugging facility comes at the cost of a few
   global variables, most notably a large char array.


*/

#include <stdio.h>

/* GS-26/09/2008-13:03(AEST) TODO: find an elegant way to use printf_P
 * instead of printf on the AVR */

// platform-specific header file
#include "config.h"

#ifndef DARJEELING_DEBUG

/* Then do nothing (easy case) */

#define DEBUG_LOG(format, args...)

#define DEBUG_ENTER_NEST(name)
#define DEBUG_EXIT_NEST(name)

#define DEBUG_ENTER_NEST_LOG(format, args...)
#define DEBUG_EXIT_NEST_LOG(format, args...)


#define DEBUG_LOG_WHEREAMI()

#else

/* Then define  our precise macros  (I'm sure these strings  will love
 * beeing stored  in RAM on the AVR,  but hey, this is  debug code)
 */

/*extern*/ int  darjeeling_debug_nesting_level;
/*extern*/ int  darjeeling_debug_indent_index;
/*extern*/ char darjeeling_debug_char_buffer[256];

#define DEBUG_LOG(format, args...)do {                                     \
        DEBUG_PRINT_INDENT;                                             \
        DARJEELING_PRINTF(DARJEELING_PGMSPACE_MACRO(format),##args);    \
    } while(0)


#define DEBUG_PRINT_INDENT do {                                         \
        for(darjeeling_debug_indent_index=0;                            \
            darjeeling_debug_indent_index<darjeeling_debug_nesting_level; \
            darjeeling_debug_indent_index++)                            \
            DARJEELING_PRINTF(DARJEELING_PGMSPACE_MACRO("   "));        \
    } while(0)

#define DEBUG_ENTER_NEST(name) do {             \
        DEBUG_LOG(">> %s\n", name);             \
        darjeeling_debug_nesting_level++;       \
    } while(0)

#define DEBUG_ENTER_NEST_LOG(format,args...) do {   \
        DEBUG_LOG(">> ");                                               \
        DARJEELING_PRINTF(DARJEELING_PGMSPACE_MACRO(format),##args);    \
        darjeeling_debug_nesting_level++;                               \
    } while(0)


#define DEBUG_EXIT_NEST(name) do {                      \
        darjeeling_debug_nesting_level--;               \
        if(darjeeling_debug_nesting_level<0)            \
        {                                               \
            DEBUG_LOG("<< negative nesting level !\n"); \
            darjeeling_debug_nesting_level=0;           \
        }                                               \
        DEBUG_LOG("<< %s\n",name);                      \
    } while(0)

#define DEBUG_EXIT_NEST_LOG(format,args...) do {        \
        darjeeling_debug_nesting_level--;               \
        if(darjeeling_debug_nesting_level<0)            \
        {                                               \
            DEBUG_LOG("<< negative nesting level !\n"); \
            darjeeling_debug_nesting_level=0;           \
        }                                               \
        DEBUG_LOG("<< ");                               \
        DARJEELING_PRINTF(DARJEELING_PGMSPACE_MACRO(format),##args);    \
    } while(0)



#define DEBUG_WHEREAMI (                                                \
        snprintf(darjeeling_debug_char_buffer,256,"%s:%d",__FILE__,__LINE__), \
        darjeeling_debug_char_buffer                                    \
                                                                        )

#define DEBUG_LOG_WHEREAMI() do {               \
        DEBUG_LOG("%s\n",DEBUG_WHEREAMI);       \
    }while(0)


#endif// DARJEELING_DEBUG



#endif // __debug_h
