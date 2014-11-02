/*
 * mainTask.c
 *
 * Created: 11.10.2014 20:18:00
 *  Author: Tomas Baca
 */

#include "mainTask.h"
#include "cspTask.h"
#include "system.h"
#include "queue.h"
#include "usart_driver_RTOS.h"

// to save outcomming time of ping
uint32_t pingSent;

// for ping time difference
int16_t timediff;

/* -------------------------------------------------------------------- */
/*	The main task														*/
/* -------------------------------------------------------------------- */
void mainTask(void *p) {
	
	// packet used to handle incoming communication
	csp_packet_t * outcomingPacket = csp_buffer_get(CSP_PACKET_SIZE);

	// character received from uart
	char inChar;
	
	/* The variable used to receive from the queue. */
	xCSPStackEvent_t xReceivedEvent;
	
	int i; // iterator
	
	// text buffer for uart
	char text[20];
	
	// infinite while loop of the program 
	while (1) {
		
		if (xQueueReceive(xCSPEventQueue, &xReceivedEvent, 1)) {
			
			switch (xReceivedEvent.eEventType) {
			
				case generalCommEvent:
			
					// send its content to the uart
					for (i = 0; i < ((csp_packet_t *) xReceivedEvent.pvData)->length; i++) {
						usartBufferPutByte(pc_usart_buffer, ((csp_packet_t *) xReceivedEvent.pvData)->data[i], 10);
					}
			
				break;
				case pingReceivedEvent:
			
					// calculate the ping return time
					if ((int16_t) milisecondsTimer - (int16_t) pingSent > 0)
						timediff = milisecondsTimer - pingSent;
					else
						timediff = (int16_t) milisecondsTimer - (int16_t) pingSent + (int16_t) 1000;

					itoa(timediff, text, 10);
					usartBufferPutString(pc_usart_buffer, "ping received in ", 10);
					usartBufferPutString(pc_usart_buffer, text, 10);
					usartBufferPutString(pc_usart_buffer, "ms\n\r", 10);
			
				break;
			
				default:
					/* Should not get here. */
				break;
			}
		}
		
		// if there is something from the uart
		if (usartBufferGetByte(pc_usart_buffer, &inChar, 0)) {
		
			outcomingPacket->data[0] = inChar;
			outcomingPacket->length = 1;
		
			switch (inChar) {
			
				// ask board for free memory heap
				case 'm':
					csp_sendto(CSP_PRIO_NORM, CSP_BOARD_ADDRESS, 16, 15, CSP_O_NONE, outcomingPacket, 10);
				break;
			
				// ask board for status
				case 'h':
					csp_sendto(CSP_PRIO_NORM, CSP_BOARD_ADDRESS, 17, 15, CSP_O_NONE,  outcomingPacket, 10);
				break;
			
				// ask board for status
				case 'p':	
					outcomingPacket->data[0] = 0;
					outcomingPacket->length = 1;
					pingSent = milisecondsTimer;
					csp_sendto(CSP_PRIO_NORM, CSP_BOARD_ADDRESS, 1, 32, CSP_O_NONE, outcomingPacket, 10);
				break;
			
				// sends the char and is supposed to receive it back
				default:
					csp_sendto(CSP_PRIO_NORM, CSP_BOARD_ADDRESS, 15, 15, CSP_O_NONE,  outcomingPacket, 10);
				break;
			}
		}
	}
}