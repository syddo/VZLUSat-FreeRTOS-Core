/*
 * cspTask.c
 *
 * Created: 11.10.2014 20:33:09
 *  Author: Tomas Baca
 */ 

#include "cspTask.h"
#include "system.h"

xQueueHandle * xCSPEventQueue;

/* -------------------------------------------------------------------- */
/*	Task that handles CSP incoming packets								*/
/* -------------------------------------------------------------------- */
void cspTask(void *p) {
	
	/* Create socket without any socket options */
	csp_socket_t * sock = csp_socket(CSP_SO_NONE);

	/* Bind all ports to socket */
	csp_bind(sock, CSP_ANY);

	/* Create 10 connections backlog queue */
	csp_listen(sock, 10);

	/* Pointer to current connection and packet */
	csp_conn_t * conn;
	csp_packet_t * packet;
	
	xCSPEventQueue = xQueueCreate(10, (portBASE_TYPE) sizeof(xCSPStackEvent_t));
	
	xCSPStackEvent_t * newEvent;

	/* Process incoming connections */
	while (1) {
		
		/* Wait for connection, 10000 ms timeout */
		if ((conn = csp_accept(sock, 10000)) == NULL)
		continue;
		
		led_red_toggle();

		/* Read packets. Timout is 1000 ms */
		while ((packet = csp_read(conn, 10)) != NULL) {

			switch (csp_conn_dport(conn)) {
				
				// response to all except ping
				case 15:
				
					newEvent->eEventType = generalCommEvent;
					newEvent->pvData = packet;
					xQueueSend(xCSPEventQueue, newEvent, 10);
					
				break;
				
				// response to ping
				case 32:

					newEvent->eEventType = pingReceivedEvent;
					newEvent->pvData = packet;
					xQueueSend(xCSPEventQueue, newEvent, 10);
				
				break;
				
				/* Process packet here */
				default:
				
				/* Let the service handler reply pings, buffer use, etc. */
				csp_service_handler(conn, packet);
				break;
			}
		}

		/* Close current connection, and handle next */
		csp_close(conn);
	}
}