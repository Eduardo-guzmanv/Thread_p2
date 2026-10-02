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
            color = GREEN;

            TurnOffLeds();
            LED_TurnOnLed(LED_GREEN);

            TMR_StartIntervalTimer(
                myTimerID,
                4000,
                myTaskTimerCallback,
                NULL
            );
        }


        /* =====================================================
         * EVENT 2 - TIMER EXPIRED
         * ===================================================== */
        if(customEvent & gMyNewTaskEvent2_c)
        {
            /* Increment counter 0 -> 1 -> 2 -> 3 -> 0 */
            if(color != MAGENTA)
            {
                color += 1;
            }
            else
            {
                color = GREEN;
            }



            switch(color)
            {
                case GREEN:

                    TurnOffLeds();
                    LED_TurnOnLed(LED_GREEN);

                    break;


                case RED:

                	TurnOffLeds();
                    LED_TurnOnLed(LED_RED);

                    break;


                case BLUE:

                	TurnOffLeds();
                    LED_TurnOnLed(LED_BLUE);

                    break;


                case MAGENTA:

                	TurnOffLeds();
                    LED_TurnOnLed(LED_RED);
                    LED_TurnOnLed(LED_BLUE);

                    break;


                default:

                    break;
            }

        }


        /* =====================================================
         * EVENT 3 - STOP TIMER
         * ===================================================== */
        if(customEvent & gMyNewTaskEvent3_c)
        {
            TurnOffLeds();

            TMR_StopTimer(myTimerID);
        }


        /* =====================================================
         * SW3 PRESSED
         * Counter = 0
         * ===================================================== */
        if(customEvent & gMyNewTaskEventSW3_c)
        {
            color = GREEN;

            TurnOffLeds();
            LED_TurnOnLed(LED_GREEN);


            /* Restart the 4 second timer */
            TMR_StopTimer(myTimerID);

            TMR_StartIntervalTimer(
                myTimerID,
                4000,
                myTaskTimerCallback,
                NULL
            );
        }


        /* =====================================================
         * SW4 PRESSED
         * Counter = 2
         * ===================================================== */
        if(customEvent & gMyNewTaskEventSW4_c)
        {
            color = BLUE;

            TurnOffLeds();
            LED_TurnOnLed(LED_BLUE);

            /* Restart the 4 second timer */
            TMR_StopTimer(myTimerID);

            TMR_StartIntervalTimer(
                myTimerID,
                4000,
                myTaskTimerCallback,
                NULL
            );
        }
    }
}
void MyTask_SW3_Pressed(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEventSW3_c);
}

void MyTask_SW4_Pressed(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEventSW4_c);
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

