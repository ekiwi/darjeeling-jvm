#include <stdint.h>

#include "common/execution/execution.h"
#include "common/array.h"

#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"

#include "base_definitions.h"

static struct abc_conn uc;
static short receiveThreadId = -1;

static void recv_uc(struct abc_conn *c, rimeaddr_t *from)
{
	dj_vm_notify(dj_exec_getVM(), lock, true);
}

static const struct abc_callbacks abc_callbacks = {recv_uc};

void javax_radio_Radio_void__waitForMessage()
{
	// wait for radio
	dj_thread * currentThread = dj_exec_getCurrentThread();
	receiveThreadId = currentThread->id;
	currentThread->status = THREADSTATUS_BLOCKED_FOR_IO;
	dj_exec_breakExecution();
}

// byte[] javax.radio.Radio._readBytes()
void javax_radio_Radio_byte____readBytes()
{
	dj_int_array * ret;
	int length = rimebuf_datalen();

	ret = dj_int_array_create(T_BYTE, length);

	// copy data from the rimebuf to the return array
	rimebuf_copyto(ret->data.bytes);

	dj_exec_stackPushRef(VOIDP_TO_REF(ret));
}

// void javax.radio.Radio._init()
void javax_radio_Radio_void__init()
{
	abc_open(&uc, 9345, &abc_callbacks);
	lock = dj_exec_stackPopRef();
}

// byte javax.radio.Radio._getNumMessages()
void javax_radio_Radio_byte__getNumMessages()
{

}

// void javax.radio.Radio.setChannel(short)
void javax_radio_Radio_void_setChannel_short()
{
	int16_t channel = dj_exec_stackPopShort();
	// not implemented
}

// short javax.radio.Radio.getMaxMessageLength()
void javax_radio_Radio_short_getMaxMessageLength()
{

}

// void javax.radio.Radio._broadcast(byte[])
void javax_radio_Radio_void__broadcast_byte__()
{
    leds_on(LEDS_GREEN);

	rimeaddr_t addr;

	dj_int_array * byteArray = dj_exec_stackPopRef();

	// check null
	if (byteArray==nullref)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);

	// copy bytes to the rime buffer
    rimebuf_copyfrom(byteArray->data.bytes, byteArray->array.length);
    rimebuf_set_datalen(byteArray->array.length);

    // abc
    abc_send(&uc);

    leds_off(LEDS_GREEN);
}

// boolean javax.radio.Radio._send(short, byte[])
void javax_radio_Radio_boolean__send_short_byte__()
{
    leds_on(LEDS_GREEN);

	rimeaddr_t addr;

	dj_int_array * byteArray = dj_exec_stackPopRef();
	int16_t id = dj_exec_stackPopShort();

	// check null
	if (byteArray==nullref)
		dj_exec_createAndThrow(BASE_CDEF_java_lang_NullPointerException);

	// copy bytes to the rime buffer
    rimebuf_copyfrom(byteArray->data.bytes, byteArray->array.length);
    rimebuf_set_datalen(byteArray->array.length);

    // abc
    abc_send(&uc);

    leds_off(LEDS_GREEN);
}
