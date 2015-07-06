/*
 * mainTask.c
 *
 * Created: 11.10.2014 20:18:00
 *  Author: Tomas Baca
 */

#include "mainTask.h"
#include "system.h"
#include "queue.h"
#include "usart_driver_RTOS.h"
#include "leds.h"

#define BUFFER_SIZE 512

/* -------------------------------------------------------------------- */
/*	The main task														*/
/* -------------------------------------------------------------------- */
void mainTask(void *p) {
	
	// character received from uart
	char inChar;

	// buffer for message from OBC
	uint8_t buffer[BUFFER_SIZE];
	
	// 0 = not receiving
	// 1 = received address
	// 2 = receiving message
	uint8_t receiverState = 0;
	
	uint8_t twiAddress = 0;
	
	int16_t bytesReceived = 0;
		
	// infinite while loop of the program 
	while (1) {
				
		// if there is something from the uart
		if (usartBufferGetByte(pc_usart_buffer, &inChar, 0)) {

			// start receiving
			if (receiverState == 0) {
				
				twiAddress = inChar - '0';
				receiverState = 1;
				
			// expecting |
			} else if (receiverState == 1) {
				
				if (inChar == '|') {
					
					receiverState = 2;
					bytesReceived = 0;
				} else {
					
					receiverState = 0;
				}
				
			// receiving message
			} else if (receiverState == 2) {
				
				// message received
				if (inChar == '\n') {
					
					// if the message length is ok
					if (bytesReceived > 0 && ((bytesReceived % 2) == 0))
						sendToTwi((uint8_t *) &buffer, bytesReceived, twiAddress);
						
					receiverState = 0;
					bytesReceived = 0;
				
				// message being received
				} else {
					
					if (bytesReceived == (BUFFER_SIZE-1)) {
						
						receiverState = 0;	
					}
					
					buffer[bytesReceived++] = inChar;
				}
			}
		}
	}
}

// send the received message to i2c
void sendToTwi(uint8_t * buffer, int16_t bytesReceived, uint8_t address) {
		
	bytesReceived = bytesReceived/2;
	
	uint16_t i;
	for (i = 0; i < bytesReceived; i++) {
		
		buffer[i] = hex2bin(&buffer[2*i]);
	}
	
	if (i2c_send(address, buffer, bytesReceived) == 0) {
		ledYellow = 1;
	}
}

// converts two characters in hex to one byte in binary
uint8_t hex2bin(const uint8_t * ptr) {
	
	uint8_t value = 0;
	uint8_t ch = *ptr;
	
	int i;
	for (i = 0; i < 2; i++) {
		
		if (ch >= '0' && ch <= '9')
			value = (value << 4) + (ch - '0');
        else if (ch >= 'A' && ch <= 'F')
			value = (value << 4) + (ch - 'A' + 10);
        else if (ch >= 'a' && ch <= 'f')
			value = (value << 4) + (ch - 'a' + 10);
        else
			return value;
        ch = *(++ptr);
	}
	
	return value;
}