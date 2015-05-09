/*
 * mainTask.h
 *
 * Created: 11.10.2014 20:19:18
 *  Author: Tomas Baca
 */ 

#include "system.h"

#ifndef MAINTASK_H_
#define MAINTASK_H_

void mainTask(void *p);

void sendToTwi(uint8_t * buffer, int16_t bytesReceived, uint8_t address);

uint8_t hex2bin(const uint8_t * ptr);

#endif /* MAINTASK_H_ */