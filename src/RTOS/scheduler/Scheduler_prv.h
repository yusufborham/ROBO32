/*
 * Scheduler_prv.h
 *
 *  Created on: Aug 24, 2025
 *      Author: Koshok
 */

#ifndef RTOS_SCHEDULER_SCHEDULER_PRV_H_
#define RTOS_SCHEDULER_SCHEDULER_PRV_H_

#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"
typedef enum{
	Running=1,
	Suspended,

}TaskState;

typedef struct{
	void (*Fptr)(void);
	u32 Periodicity;
	u32 FirstDealy;
	u8 Priority;
	TaskState State;

}Task;



#define OUT_OF_RANGE_ERROR		1
#define RESERVED_PRIORITY_ERROR	2
#define NO_TASK_ERROR			3

#endif /* RTOS_SCHEDULER_SCHEDULER_PRV_H_ */
