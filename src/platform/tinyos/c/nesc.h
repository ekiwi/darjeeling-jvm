#ifndef __nesc_h__
#define __nesc_h__

int nesc_printf(char * msg);
uint32_t nesc_getTime();
void nesc_setLed(int nr, int on);

uint16_t nesc_getMaxPayloadLength();
int nesc_send(const char * message, int16_t receiverId, uint16_t length);
int nesc_wasAcked();

uint16_t nesc_peekMessageLength();
void * nesc_popMessageBuffer();
int nesc_getNrMessages();

#endif
