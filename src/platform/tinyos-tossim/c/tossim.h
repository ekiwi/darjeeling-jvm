#ifndef __tossim_h__
#define __tossim_h__
#include "program_mem.h"

int tossim_printf(char *);
uint32_t tossim_getTime();

uint16_t tossim_getMaxPayloadLength();
int tossim_send(const char * message, int16_t receiverId, uint16_t length);
int tossim_wasAcked();

uint16_t tossim_peekMessageLength();
void * tossim_popMessageBuffer();
void tossim_setBufferIsLocked(char lock);
int tossim_getNrMessages();

#endif
