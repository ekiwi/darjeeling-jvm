#include "contiki.h"
#include "dev/leds.h"

#include "stdio.h"

#include "common/debug.h"
#include "common/vm.h"
#include "common/heap/heap.h"
#include "common/infusion.h"
#include "common/types.h"
#include "common/vmthread.h"
#include "common/djtimer.h"
#include "common/execution/execution.h"

#include "loader.h"

#ifdef HAS_USART
#include "dev/rs232.h"
#endif

/*---------------------------------------------------------------------------*/
PROCESS(blink_process, "Darjeeling");
AUTOSTART_PROCESSES(&blink_process);
/*---------------------------------------------------------------------------*/

static unsigned char mem[HEAPSIZE];
static struct etimer et;
static dj_vm * vm;

PROCESS_THREAD(blink_process, ev, data)
{

	PROCESS_EXITHANDLER(goto exit;)
	PROCESS_BEGIN();

	// initialise memory manager
	dj_mem_init(mem, HEAPSIZE);

	// initialise timer
	dj_timer_init();

	// init hw
	leds_init();

#ifdef HAS_USART
	rs232_init(RS232_PORT_0, USART_BAUD_57600, USART_PARITY_NONE | USART_DATA_BITS_8 | USART_STOP_BITS_1);
	rs232_redirect_stdout(RS232_PORT_0);
#endif

	// create a new VM
	vm = dj_vm_create();

	// tell the execution engine to use the newly created VM instance
	dj_exec_setVM(vm);

	// load the embedded infusions
	dj_loadEmbeddedInfusions(vm);

	while (true)
	{

		// start the main execution loop
		if (dj_vm_countLiveThreads(vm)>0)
		{
			dj_vm_schedule(vm);

			if (vm->currentThread!=NULL)
				if (vm->currentThread->status==THREADSTATUS_RUNNING)
					dj_exec_run(RUNSIZE);

		}

		// can't get PROCESS_YIELD to work, quick hack to wait 1 clock tick
	    etimer_set(&et, 1);
	    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	}

exit:
	leds_off(LEDS_ALL);
	PROCESS_END();
}
