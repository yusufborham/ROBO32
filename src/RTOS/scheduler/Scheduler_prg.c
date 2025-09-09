/*
 * Scheduler_prg.c
 *
 *  Created on: Aug 24, 2025
 *      Author: Koshok
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "Scheduler_int.h"
#include "Scheduler_cfg.h"
#include "Scheduler_prv.h"

Task Sys_Task[MAX_NO_TASKS] = { { 0 } };
u32 TimingTask[MAX_NO_TASKS];

static void OS_Scheduler(void);

void OS_vStart(void) {
    MSYSTICK_Config_t STK_cfg = {
        .InterruptEnable = INT_ENABLE,
        .CLK_SRC = CLK_SRC_AHB_8
    };

    MSYSTICK_vInit(&STK_cfg);

    MSYSTICK_vSetInterval_Multi(TICKTIME, OS_Scheduler);
}

static void OS_Scheduler(void) {
    for (u8 i = 0; i < MAX_NO_TASKS; i++) {
        if (Sys_Task[i].State == Running) {
            if (TimingTask[i] == 0 || TimingTask[i] >= Sys_Task[i].Periodicity) {
                if (Sys_Task[i].Fptr != NULL) {
                    Sys_Task[i].Fptr();
                }
                TimingTask[i] = Sys_Task[i].Periodicity;
            } else {
                TimingTask[i]--;
            }
        }
    }
}

ERR_STATE OS_CreateTask(void (*_xFptr)(void), u32 A_32Periodicity,
                        u32 FirstDelay, u8 Priority) {
    ERR_STATE Error_state = OK;

    if (Priority < MAX_NO_TASKS) {
        if (Sys_Task[Priority].Fptr == NULL) {
            Sys_Task[Priority].Fptr        = _xFptr;
            Sys_Task[Priority].Periodicity = A_32Periodicity;
            TimingTask[Priority]           = FirstDelay;
            Sys_Task[Priority].State       = Running;
        } else {
            Error_state = RESERVED_PRIORITY_ERROR;
        }
    } else {
        Error_state = OUT_OF_RANGE_ERROR;
    }
    return Error_state;
}

ERR_STATE OS_SuspendTask(u8 Priority) {
    ERR_STATE Error_state = OK;

    if (Sys_Task[Priority].State == Running) {
        Sys_Task[Priority].State = Suspended;
    } else {
        Error_state = NO_TASK_ERROR;
    }
    return Error_state;
}

ERR_STATE OS_ResumeTask(u8 Priority) {
    ERR_STATE Error_state = OK;

    if (Sys_Task[Priority].State == Suspended) {
        Sys_Task[Priority].State = Running;
    } else {
        Error_state = NO_TASK_ERROR;
    }
    return Error_state;
}

ERR_STATE OS_DeleteTask(u8 Priority) {
    ERR_STATE Error_state = 0;

    if (Sys_Task[Priority].Fptr != NULL) {
        Sys_Task[Priority].Fptr        = NULL;
        Sys_Task[Priority].Periodicity = 0;
        Sys_Task[Priority].State       = Suspended;
        TimingTask[Priority]           = 0;
    } else {
        Error_state = NO_TASK_ERROR;
    }
    return Error_state;
}
