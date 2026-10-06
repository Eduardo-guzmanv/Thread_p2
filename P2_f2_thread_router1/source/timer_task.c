/*
 * MyNewTask.c
 *
 *  Created on: 7 sep. 2026
 *      Author: santiagosalcedo
 */

#include "timer_task.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "PhyInterface.h"
#include "MacInterface.h"

#define LED_BLUE    (1 << 3)   // 0x01
#define LED_RED  (1 << 1)   // 0x02
#define LED_GREEN   (1 << 2)   // 0x04

osaEventId_t mMyEvents;
/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;

/* Information about the PAN we are part of */
/* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;

static volatile uint16_t mTeamCounter = 1;

/*
        #define gRedLedIdx_c                    0
        #define gGreenLedIdx_c                  1
        #define gBlueLedIdx_c                   2
 * */
typedef enum {
	GREEN,
	RED,
	BLUE,
	MAGENTA
}t_LED_color;

t_LED_color color;
/* Forward declarations */
void My_Task(osaTaskParam_t argument);
static void myTaskTimerCallback(void *param);

/* OSA Task Definition*/
OSA_TASK_DEFINE(My_Task, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE );

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

        /* =====================================================
         * EVENT 1 - START TIMER
         * ===================================================== */
        if(customEvent & gMyNewTaskEvent1_c)
        {
            mTeamCounter = 1;
            color = GREEN;
            TurnOffLeds();
            LED_TurnOnLed(LED_GREEN);

            if(myTimerID != gTmrInvalidTimerID_c)
            {
                TMR_StartIntervalTimer(
                    myTimerID,
                    1000,
                    myTaskTimerCallback,
                    NULL
                );
            }
        }

        if(customEvent & gMyNewTaskEvent2_c)
        {
        	if(mTeamCounter < 150){
				mTeamCounter++;
			}
        	else{
				mTeamCounter = 1;
			}

        }

        if(customEvent & gMyNewTaskEvent3_c)
        {
            TurnOffLeds();

            TMR_StopTimer(myTimerID);
        }

        if(customEvent & gMyNewTaskEvent4_c)
        {
            mTeamCounter = 0;

            if(myTimerID != gTmrInvalidTimerID_c)
            {
                TMR_StartIntervalTimer(
                    myTimerID,
                    1000,
                    myTaskTimerCallback,
                    NULL
                );
            }
        }

        if(customEvent & gMyNewTaskEvent5_c)
        {
            if(myTimerID != gTmrInvalidTimerID_c)
            {
                TMR_StartIntervalTimer(
                    myTimerID,
                    1000,
                    myTaskTimerCallback,
                    NULL
                );
            }
        }
    }
}

/* Function to init the task */
void MyTimer_Init(void)
{
    mMyEvents = OSA_EventCreate(TRUE);
    /* The instance of the MAC is passed at task creaton */
    gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(My_Task), NULL);
}

/* This is the function called by the Timer each time it expires */
static void myTaskTimerCallback(void *param)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);

}

/* Public function to send an event to stop the timer */
void MyTaskTimer_Stop(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEvent3_c);
}

/* Public function to send an event to start the timer */
void MyTaskTimer_Start(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEvent1_c);
}

uint16_t get_timer(void){
    return mTeamCounter;
}

void MyTaskTimer_StartFromZero(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEvent4_c);
}

void MyTaskTimer_Resume(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEvent5_c);
}
