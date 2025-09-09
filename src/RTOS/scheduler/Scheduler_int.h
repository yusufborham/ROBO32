/*
 * Scheduler_int.h
 *
 *  Created on: Aug 24, 2025
 *      Author: Koshok
 */

#ifndef RTOS_SCHEDULER_SCHEDULER_INT_H_
#define RTOS_SCHEDULER_SCHEDULER_INT_H_

#include "../../LIB/STD_TYPES.h"



/* Error states */
typedef enum {
    OK = 0,
    OUT_OF_RANGE_ERROR,
    RESERVED_PRIORITY_ERROR,
    NO_TASK_ERROR
} ERR_STATE;


/* Scheduler API */
void OS_vStart(void);
ERR_STATE OS_CreateTask(void (*Fptr)(void), u32 Periodicity, u32 FirstDelay, u8 Priority);
ERR_STATE OS_SuspendTask(u8 Priority);
ERR_STATE OS_ResumeTask(u8 Priority);
ERR_STATE OS_DeleteTask(u8 Priority);

#endif /* RTOS_SCHEDULER_SCHEDULER_INT_H_ */
