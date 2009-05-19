#include <stdio.h>
#include <avr/pgmspace.h>

#include "fos/boards/motorcar.h"
#include "fos/adc.h"

#include "common/types.h"
#include "common/execution/execution.h"

void javax_fleck_boards_Motorcar_void_initMotorcar() {
	printf_P(PSTR("Motorcar: initializing daughterboard\n"));
	fos_motorcar_init();
}

void javax_fleck_boards_Motorcar_short_readSensor_byte() {
	int32_t arg = dj_exec_stackPopInt();
	uint16_t result = -1;

	fos_adc_single_read(0, arg, &result);

	printf("Motorcar: sensor %ld reads %d\n", arg, result);

	dj_exec_stackPushInt((int32_t) result);
}

void javax_fleck_boards_Motorcar_void_drive_byte() {
	int32_t arg = dj_exec_stackPopInt();
	printf_P(PSTR("Motorcar: driving %ld\n"), arg);
	if (arg == -1)
		fos_motorcar_drive_backward();
	if (arg == 0)
		fos_motorcar_stop();
	if (arg == 1)
		fos_motorcar_drive_forward();
}

void javax_fleck_boards_Motorcar_void_setSpeed_short() {
	uint32_t speed = dj_exec_stackPopInt();
	printf_P(PSTR("Motorcar: setting speed %ld\n"), speed);

	fos_motorcar_set_speed((uint16_t) speed);
}

void javax_fleck_boards_Motorcar_void_steer_byte() {
	int32_t arg = dj_exec_stackPopInt();
	if (arg == -1) {
		printf_P(PSTR("Motorcar: steering left\n"));
		fos_motorcar_steer_left();
	}
	if (arg == 0) {
		printf_P(PSTR("Motorcar: steering straight\n"));
		fos_motorcar_steer_straight();
	}
	if (arg == 1) {
		printf_P(PSTR("Motorcar: steering right\n"));
		fos_motorcar_steer_right();
	}
}

void javax_fleck_boards_Motorcar_void_start() {
	fos_motorcar_start();
}

void javax_fleck_boards_Motorcar_void_stop() {
	fos_motorcar_stop();
}

