#include "fos/leds.h"

#include "common/execution/execution.h"


// void javax.fleck.Leds.setLed(int, boolean)
void javax_fleck_Leds_void_setLed_int_boolean()
{
	// pop arguments from the stack
	int16_t on = dj_exec_stackPopShort();
	int32_t id = dj_exec_stackPopInt();

	if (id==0) fos_leds_blue(on);
	if (id==1) fos_leds_green(on);
	if (id==2) fos_leds_red(on);
}
