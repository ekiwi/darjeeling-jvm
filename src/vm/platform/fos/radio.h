#ifndef __radio_h
#define __radio_h

#include "fos/mac.h"

#include "common/object.h"
#include "common/array.h"

// These functions are the interface between DJ and the FOS radio API


void dj_radio_init();
void dj_radio_receive_message(fos_mac_message_t * message);
uint8_t dj_radio_send_message(fos_mac_message_t * message);
uint8_t dj_radio_send_bytes(uint16_t addr, uint8_t type, uint8_t group, uint8_t length, uint8_t data[]);
int dj_radio_get_nr_messages();

int dj_radio_bufferHasNext();
int dj_radio_bufTopAddress();
int dj_radio_bufTopGroup();
int dj_radio_bufTopType();
int dj_radio_bufTopLength();
dj_int_array * dj_radio_bufTopBytes();
void dj_radio_bufPop();

void dj_radio_set_channel(uint8_t channel);

#endif // __radio_h
