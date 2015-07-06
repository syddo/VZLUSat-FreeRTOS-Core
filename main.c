/*
 * main.c
 *
 * Created: 24.8.2014 15:10:04
 *  Author: Tomas Baca
 */ 

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "system.h"
#include "mainTask.h"
#include "leds.h"

int main(void) {
	
	// initialize the xMega peripherals
	boardInit();
				
	/* -------------------------------------------------------------------- */
	/*	Starts blinking task - only for debug								*/
	/* -------------------------------------------------------------------- */
	xTaskCreate(leds, (signed char*) "blink", 512, NULL, configNORMAL_PRIORITY, NULL);
		
	/* -------------------------------------------------------------------- */
	/*	Starts task that handles outgoing communication		 				*/
	/* -------------------------------------------------------------------- */
	xTaskCreate(mainTask, (signed char*) "mainTask", 2048, NULL, configNORMAL_PRIORITY, NULL);
	
	/* -------------------------------------------------------------------- */
	/*	Starts the scheduler and all previously created tasks				*/
	/* -------------------------------------------------------------------- */
	vTaskStartScheduler();
	
	return 0;
}