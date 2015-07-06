/*
 * leds.c
 *
 * Created: 9.5.2015 22:19:20
 *  Author: klaxalk
 */ 

#include "system.h"

volatile uint8_t ledRed = 1;
volatile uint8_t ledYellow = 1;

// Blinking RTOS task, just for debugging
void leds(void *p) {

	led_yellow_on();

	uint8_t i;
	for (i = 0; i < 10; i++) {

		led_red_toggle();
		led_yellow_toggle();
		vTaskDelay(200);
	}
	
	// takes care of rx/tx leds
	while (1) {
		
		if (ledRed < 20) {
			
			led_red_on();
			ledRed++;
		} else
			led_red_off();
		
		if (ledYellow < 20) {
			
			led_yellow_on();
			ledYellow++;
		} else
			led_yellow_off();
		
		vTaskDelay(10);
	}
	
}