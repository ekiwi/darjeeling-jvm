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
