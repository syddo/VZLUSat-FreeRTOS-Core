/*
 * cspTask.h
 *
 * Created: 11.10.2014 20:33:36
 *  Author: Tomas Baca
 */ 

#include "system.h"

#ifndef CSPTASK_H_
#define CSPTASK_H_

typedef enum {
	
	nothingEvent = 0,
	generalCommEvent = 1,
	pingReceivedEvent = 2,
	
} eCSPEvent_t;

typedef struct CSP_TASK_COMMANDS
{
	eCSPEvent_t eEventType; /* Tells the receiving task what the event is. */
	void *pvData; /* Holds or points to any data associated with the event. */

} xCSPStackEvent_t;

extern xQueueHandle * xCSPEventQueue;

/* -------------------------------------------------------------------- */
/*	Task that handles CSP incoming packets								*/
/* -------------------------------------------------------------------- */
void cspTask(void *p);


#endif /* CSPTASK_H_ */