#ifndef __dj_tos_message__
#define __dj_tos_message__

typedef nx_struct radio_count_msg {
	  nx_uint8_t payload[24];
	  nx_uint8_t length;
//  nx_uint16_t counter;
} radio_count_msg_t;

/*
enum {
  AM_RADIO_COUNT_MSG = 6,
};
*/

/*

typedef nx_struct tos_message
{
  nx_uint8_t payload[24];
} tos_message_t;

enum {
	  DJ_TOS_MESSAGE = 0xf1,
};
*/

#endif
