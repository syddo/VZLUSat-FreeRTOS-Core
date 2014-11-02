/*
 * system.c
 *
 * Created: 11.10.2014 18:40:47
 *  Author: Tomas Baca
 */

#include "sysclk.h"
#include "system.h"
#include "config.h"
#include "TC_driver.h"

UsartBuffer * pc_usart_buffer;
volatile uint32_t milisecondsTimer;
volatile uint32_t secondsTimer;
volatile uint32_t hoursTimer;
 
/* -------------------------------------------------------------------- */
/*	Initialize the xMega peripherals									*/
/* -------------------------------------------------------------------- */
void boardInit() {
	
	// prepare the i/o for LEDs
	ioport_init();
	
	// clock init & enable system clock to all peripheral modules
	sysclk_init();
	sysclk_enable_module(SYSCLK_PORT_GEN, 0xff);
	sysclk_enable_module(SYSCLK_PORT_A, 0xff);
	sysclk_enable_module(SYSCLK_PORT_B, 0xff);
	sysclk_enable_module(SYSCLK_PORT_C, 0xff);
	sysclk_enable_module(SYSCLK_PORT_D, 0xff);
	sysclk_enable_module(SYSCLK_PORT_E, 0xff);
	sysclk_enable_module(SYSCLK_PORT_F, 0xff);
	
	ioport_set_pin_dir(RED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(YELLOW, IOPORT_DIR_OUTPUT);
	
	led_yellow_off();
	led_red_off();
	
	/* -------------------------------------------------------------------- */
	/*	Timer for RTC														*/
	/* -------------------------------------------------------------------- */
	
	// select the clock source and pre-scaler by 8
	TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV64_gc);
	
	TC1_SetOverflowIntLevel(&TCC1, TC_OVFINTLVL_LO_gc);
	
	TC_SetPeriod(&TCC1, 499);
	
	milisecondsTimer = 0;
	secondsTimer = 0;
	hoursTimer = 0;
	
	pc_usart_buffer = usartBufferInitialize(&PC_USART, PC_USART_BAUDRATE, PC_USART_BUFFERSIZE);
}

/* -------------------------------------------------------------------- */
/*	Interrupt for timing the RTC										*/
/* -------------------------------------------------------------------- */
ISR(TCC1_OVF_vect) {
	
	// shut down the output PPM pulse
	
	if (milisecondsTimer++ == 1000) { // overflow to seconds
		
		milisecondsTimer = 0;
		
		if (secondsTimer++ == 3600) { // overflow to hours
			
			secondsTimer = 0;
			hoursTimer++;
		}
	}
}