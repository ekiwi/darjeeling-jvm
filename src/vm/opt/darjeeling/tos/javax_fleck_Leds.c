#include <stdint.h>

#include "common/array.h"
#include "common/execution/execution.h"

#include "nesc.h"

// void javax.fleck.Leds.setLed(int, boolean)
void javax_fleck_Leds_void_setLed_int_boolean()
{
	uint16_t on = dj_exec_stackPopShort();
	uint32_t nr = dj_exec_stackPopInt();

	nesc_setLed(nr, on);

}
