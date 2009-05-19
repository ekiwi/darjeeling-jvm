/*
 *	javax_fleck_TestBoard.c
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
#include <stdint.h>

#include "fos/fos_msg.h"
#include "fos/boards/testboard.h"

#include "common/array.h"
#include "common/execution/execution.h"

void javax_fleck_TestBoard_void_init()
{
	fos_testboard_init();
}

void javax_fleck_TestBoard_byte_getButtonState_int()
{
	dj_exec_stackPushInt( fos_testboard_buttons_get() & (1<<dj_exec_stackPopInt()) );
}
