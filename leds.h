/*
 * leds.h
 *
 * Created: 9.5.2015 22:20:10
 *  Author: klaxalk
 */ 

#ifndef LEDS_H_
#define LEDS_H_

volatile uint8_t ledRed;
volatile uint8_t ledYellow;

void leds(void *p);

#endif /* LEDS_H_ */