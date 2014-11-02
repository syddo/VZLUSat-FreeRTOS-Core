/*
 * mainTask.c
 *
 * Created: 11.10.2014 20:18:00
 *  Author: Tomas Baca
 */

#include "mainTask.h"
#include "cspTask.h"
#include "system.h"

csp_packet_t * outcomingPacket;

/* -------------------------------------------------------------------- */
/*	Reply the free heap space in human readable form					*/
/* -------------------------------------------------------------------- */
int sendFreeHeapSpace(csp_packet_t * inPacket) {
	
	char msg[20];
	itoa(xPortGetFreeHeapSize(), msg, 10);
	
	/* Copy message to packet */
	strcpy(outcomingPacket->data, msg);
	outcomingPacket->length = strlen(msg);

	/* Send packet */
	if (!csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 1000)) {
		/* Send failed */
	} else {
		/* Send succeeded */
	}

	return 0;
}

/* -------------------------------------------------------------------- */
/*	Reply with some status info message									*/
/* -------------------------------------------------------------------- */
int houseKeeping(csp_packet_t * inPacket) {
	
	// put the info message into the packet
	char msg[64];
	sprintf(msg, "*** Board\n\rSoftware v1.0\n\rUptime: %id %ih %im %ds\n\r", (int16_t) hoursTimer/24, (int16_t) hoursTimer%24, (int16_t) secondsTimer/60, (int16_t) secondsTimer%60);

	strcpy(outcomingPacket->data, msg);
	outcomingPacket->length = strlen(msg);

	/* Send packet */
	if (!csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, outcomingPacket, 1000)) {
		/* Send failed */
	} else {
		/* Send succeeded */
	}

	return 0;
}

/* -------------------------------------------------------------------- */
/*	Sends back the incoming packet										*/
/* -------------------------------------------------------------------- */
int echoBack(csp_packet_t * inPacket) {

	/* Send packet */
	// reuses the incoming packet for the response
	if (!csp_sendto(CSP_PRIO_NORM, inPacket->id.src, inPacket->id.sport, inPacket->id.dport, CSP_O_NONE, inPacket, 1000)) {
		
		/* Send failed */
	} else {
		/* Send succeeded */
	}

	return 0;
}

/* -------------------------------------------------------------------- */
/*	The main task														*/
/* -------------------------------------------------------------------- */
void mainTask(void *p) {
	
	/* The variable used to receive from the queue. */
	xCSPStackEvent_t xReceivedEvent;
	
	outcomingPacket = csp_buffer_get(CSP_PACKET_SIZE);
	
	// infinite while loop of the program 
	while (1) {
		
		// the queue between cspTask and the main task
		// this is unblocking way how to read from the queue, the last parameter is "ticks to wait"
		if (xQueueReceive(xCSPEventQueue, &xReceivedEvent, 1)) {
		
			switch( xReceivedEvent.eEventType )
			{
				// Reply with RTOS free heap space
				// replies in Human Readable form
				case freeHeapEvent :
			
					sendFreeHeapSpace(xReceivedEvent.pvData);
			
				break;
			
				// Echo back the whole packet
				// incoming port => outcoming
				case echoBackEvent :
			
					echoBack(xReceivedEvent.pvData);
			
				break;
			
				// sends the info about the system
				case housKeepingEvent :
			
					houseKeeping(xReceivedEvent.pvData);
			
				break;
		
				default :
					/* Should not get here. */
				break;
			}
		}
	}
}