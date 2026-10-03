/*
 * timer_task.c
 *
 *  Created on: 2 oct. 2026
 *      Author: santiagosalcedo
 */

#include "timer_task.h"

/* Function implemented in router_eligible_device_app.c */
extern void APP_TriggerCounterRequest(void);

/* Event ID */
osaEventId_t mMyEvents;

/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;

/* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;

/* Forward declarations */
void My_Task(osaTaskParam_t argument);
static void myTaskTimerCallback(void *param);

/* OSA Task Definition */
OSA_TASK_DEFINE(My_Task, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE);

/* Main custom task */
void My_Task(osaTaskParam_t argument)
{
    osaEventFlags_t customEvent;

    myTimerID = TMR_AllocateTimer();

    while(1)
    {
        OSA_EventWait(
            mMyEvents,
            osaEventFlagsAll_c,
            FALSE,
            osaWaitForever_c,
            &customEvent
        );

        if(!gUseRtos_c && !customEvent)
        {
            break;
        }

        /* Start Timer */
        if(customEvent & gMyNewTaskEvent1_c)
        {
            if(myTimerID != gTmrInvalidTimerID_c)
            {
                TMR_StartIntervalTimer(
                    myTimerID,
                    gCounterRequestTime_c,
                    myTaskTimerCallback,
                    NULL
                );
            }
        }

        /* Event called from myTaskTimerCallback */
        if(customEvent & gMyNewTaskEvent2_c)
        {
            APP_TriggerCounterRequest();
        }

        /* Event to stop the timer */
        if(customEvent & gMyNewTaskEvent3_c)
        {
            if(myTimerID != gTmrInvalidTimerID_c)
            {
                TMR_StopTimer(myTimerID);
            }
        }
    }
}

/* Function to init the task */
void MyTimer_Init(void)
{
    mMyEvents = OSA_EventCreate(TRUE);

    /* Create Task */
    gMyTaskHandler_ID = OSA_TaskCreate(
        OSA_TASK(My_Task),
        NULL
    );
}

/* This is the function called by the Timer each time it expires */
static void myTaskTimerCallback(void *param)
{
    OSA_EventSet(
        mMyEvents,
        gMyNewTaskEvent2_c
    );
}

/* Public function to send an event to stop the timer */
void MyTaskTimer_Stop(void)
{
    OSA_EventSet(
        mMyEvents,
        gMyNewTaskEvent3_c
    );
}

/* Public function to send an event to start the timer */
void MyTaskTimer_Start(void)
{
    OSA_EventSet(
        mMyEvents,
        gMyNewTaskEvent1_c
    );
}
