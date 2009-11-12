#ifndef __nesc_h__
#define __nesc_h__
#include "program_mem.h"

int tossim_printf(char *);
uint32_t nesc_getTime();

uint16_t nesc_getMaxPayloadLength();
int nesc_send(const char * message, int16_t receiverId, uint16_t length);
int nesc_wasAcked();

uint16_t nesc_peekMessageLength();
void * nesc_popMessageBuffer();
void nesc_setBufferIsLocked(char lock);
int nesc_getNrMessages();

#endif
