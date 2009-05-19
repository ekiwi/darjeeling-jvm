#include <stdint.h>

#include "common/array.h"
#include "common/execution/execution.h"
#include "base_definitions.h"

#include "dev/leds.h"

// void javax.fleck.Leds.setLed(int, boolean)
void javax_fleck_Leds_void_setLed_int_boolean()
{
	int16_t on = dj_exec_stackPopShort();
	int32_t nr = dj_exec_stackPopInt();

	if (nr==0) if (on) leds_on(LEDS_BLUE); else leds_off(LEDS_BLUE);
	if (nr==1) if (on) leds_on(LEDS_GREEN); else leds_off(LEDS_GREEN);
	if (nr==2) if (on) leds_on(LEDS_RED); else leds_off(LEDS_RED);
}
