#ifndef __darjeeling_h__
#define __darjeeling_h__

#include <stdint.h>

void dj_init();
uint32_t dj_run();
void dj_notifyRadioSendDone();
void dj_notifyRadioReceive();

#endif
