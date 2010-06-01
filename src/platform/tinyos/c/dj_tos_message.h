#ifndef __dj_tos_message__
#define __dj_tos_message__

typedef nx_struct tos_message
{
  nx_uint8_t payload[24];
} tos_message_t;

enum {
	  DJ_TOS_MESSAGE = 0x11,
};

#endif
