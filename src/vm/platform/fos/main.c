#include <stdlib.h>
#include <stdio.h>

//needed for printf_P
#include <avr/pgmspace.h>

/*
 * LEDS:
 *  red - darjeeling panic
 *  green - FOS sleep in progress
 *
 * TIMERS:
 *  0   var timer
 *  1   dj_timer
 *  2   MAC
 *  3   
 */

#include "fos/fos.h"
#include "fos/kernel.h"
#include "fos/serial.h"
#include "fos/timer.h"
#include "fos/leds.h"
#include "fos/thread.h"
#include "fos/rtc.h"
#include "fos/temp.h"
#include "fos/mac.h"
#include "fos/radio.h"
#include "fos/adc.h"
#include "fos/system.h"
#include "fos/vartimer.h"

// this one does mot seem to be declared anywhere in fos headers ??
void fos_thread_yield();

#include "common/debug.h"
#include "common/vm.h"
#include "common/heap/heap.h"
#include "common/infusion.h"
#include "common/types.h"
#include "common/vmthread.h"
#include "common/djtimer.h"
#include "common/execution/execution.h"

// interface between DJ and FOS radio API
#include "radio.h"

#include "base_definitions.h"

#include "loader.h"

//void receive_thread(void * arg);
void * vm_thread(void * arg);
void * mac_thread(void * arg);

unsigned char mem[HEAPSIZE];

fos_thread_t    *vmthread;

#define BAUD_RATE   FOS_BAUD_57600

void fix_system_block()
{
    // because all FOS code relies on the EEPROM system block
    if(fos_system_get_nodeid() == 0xFFFF)
        fos_system_set_nodeid(NODEID);
}



int main()
{

    // using  the bootloader  typically  results in  having no  system
    // block, so we must fix it by hand.
    fix_system_block();



    
	// initialise FOS
	fos_init();
    
	fos_leds_init();
    fos_leds_setval(0);

    fos_serial_config(0, BAUD_RATE, FOS_DATABITS_8, FOS_STOPBITS_1, FOS_PARITY_NONE, NULL, 64);
    fos_serial_config(1, BAUD_RATE, FOS_DATABITS_8, FOS_STOPBITS_1, FOS_PARITY_NONE, NULL, 64);
	fos_serial_enable(0, FOS_SERIAL_MODE_RX | FOS_SERIAL_MODE_TX);
	fos_serial_enable(1, FOS_SERIAL_MODE_RX | FOS_SERIAL_MODE_TX);
	fos_serial_printf_enable(0);
	fos_temp_init();
	fos_adc_config(0);

    // GS-01/10/2008-08:18(AEST) Beware,  the motorcar driver needs the FOS_TIMER_3 timer

    // Enable/disable snooping for, the Fleck nanos are addressing node F3 so the sensorgui app needs this

#ifdef ENABLE_MAC
#ifdef ENABLE_SNOOP
	fos_mac_init(FOS_TIMER_2, FOS_MAC_ENABLE_ACK|FOS_MAC_ENABLE_SNOOP);
#else
    fos_mac_init(FOS_TIMER_2, FOS_MAC_ENABLE_ACK);
#endif
#endif

    fos_vartimer_init(FOS_TIMER_0);

    // show a friendly welcome message
#ifdef SHOW_WELCOME
    printf_P(PSTR("\n\n\nDarjeeling\n\n\n"));//     clean screen in foslisten

    printf_P(PSTR("Node %d starting\n"),fos_system_get_nodeid());
    
    printf_P(PSTR("FOS available memory: %d\n"), fos_system_get_available_mem());
    DEBUG_LOG("FOS available memory: %d\n", fos_system_get_available_mem());
#endif

	// initialise memory manager
	dj_mem_init(mem, HEAPSIZE);

	// initialise timer
	dj_timer_init();

	// start threads
	vmthread = fos_thread_create (vm_thread, NULL, 200, 2, 0, 0);

#ifdef ENABLE_MAC
	fos_thread_create (mac_thread, NULL, 100, 1, 0, 0);
#endif

	fos_thread_scheduler_start (1,0);

	return 0;
}

static uint8_t volatile blockedOnIO;

void * vm_thread(void * arg)
{
    static dj_vm *vm;
    static dj_thread    *thread;
    static uint32_t     scheduleTime;

	// create a new VM
	vm = dj_vm_create();

	// tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);

	// load the embedded infusions
	dj_loadEmbeddedInfusions(vm);

	// pre-allocate an OutOfMemoryError object
	dj_object *obj = dj_vm_createSysLibObject(vm, BASE_CDEF_java_lang_OutOfMemoryError);
	dj_mem_setPanicExceptionObject(obj);

    DEBUG_LOG("VM Thread: starting the main execution loop\n");

	// start the main execution loop
	while (dj_vm_countLiveThreads(vm)>0)
	{
        // see who wants to run
		dj_vm_schedule(vm);

		if (vm->currentThread==nullref) {
            // no threads to run, let's see if we can go to sleep for a while

            blockedOnIO = 0;    // number of threads blocked on i/o
            scheduleTime = 0xffffffff;
            for (thread=vm->threads; thread!=nullref; thread=thread->next) {
                if (thread->status == THREADSTATUS_BLOCKED_FOR_IO)
                    blockedOnIO++;
                else if (thread->status == THREADSTATUS_SLEEPING) {
                    if (thread->scheduleTime < scheduleTime)
                        scheduleTime = thread->scheduleTime;
                }
            }

            // now blockedOnIO > 0 if there are threads that are blocked on i/o
            // and scheduleTime < 0xffffffff if there are threads blocked on timers

            if (scheduleTime != 0xffffffff) {
                fos_leds_green_on();
                scheduleTime -= dj_timer_getTimeMillis();   // work out the time to sleep: wakeup - current

                // we need to sleep, but our timer only goes up to 8s, so we sleep in 5s chunks
                while (scheduleTime > 0) {
                    if (scheduleTime > 5000) {
                        fos_vartimer_sleep((uint16_t)5000);
                        scheduleTime -= 5000;
                    } else
                        fos_vartimer_sleep((uint16_t)scheduleTime);
                        break;
                }
                fos_leds_green_off();
            } else if (blockedOnIO > 0) {
                // suspend the VM until a relevant event happens
                //  only the radio at this point
                FOS_ATOMIC_BEGIN
                fos_leds_green_on();
                fos_thread_suspend(0);
                fos_leds_green_off();
                FOS_ATOMIC_END
            }

        } else {
            // we have a thread that wants to work
			if (vm->currentThread->status==THREADSTATUS_RUNNING)
				dj_exec_run(RUNSIZE);
            fos_thread_yield();
        }
	}

    printf_P(PSTR("No more java threads to schedule\n"));

    printf_P(PSTR("FOS thread summary:\n"));
    fos_thread_print();

	while (1)
	{
		fos_leds_red_toggle();
		fos_timer_delay(FOS_TIMER_1, 2000, FOS_TIMER_MS);
	}

    // dead code, but fos threads must return a void pointer
    return NULL;
}


#ifdef ENABLE_MAC
static fos_mac_message_t message;

void * mac_thread(void * arg)
{
	dj_radio_init();
	while (1)
	{
		uint8_t s = fos_mac_receive(0, &message);
		if (s!=255) {
			dj_radio_receive_message(&message);
            if (blockedOnIO) {
                // we were blocked on i/o, now we have some, let's unblock
                fos_thread_wake(vmthread, 0);
                blockedOnIO = 0;
            }
        }
	}
    // dead code, but fos threads must return a void pointer
    return NULL;
}
#endif
