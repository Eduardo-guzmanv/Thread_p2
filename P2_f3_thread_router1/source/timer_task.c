#include "timer_task.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "PhyInterface.h"
#include "MacInterface.h"

osaEventId_t mMyEvents;
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;
osaTaskId_t gMyTaskHandler_ID;

typedef enum
{
    GREEN,
    RED,
    BLUE
} t_LED_color;

t_LED_color color;

void My_Task(osaTaskParam_t argument);
static void myTaskTimerCallback(void *param);

OSA_TASK_DEFINE(My_Task, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE);

void My_Task(osaTaskParam_t argument)
{
    osaEventFlags_t customEvent;

    (void)argument;

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

        if(customEvent & gMyNewTaskEvent1_c)
        {
            mTeamCounter = 1;
            color = GREEN;

            LED_StopFlashingAllLeds();

            LED_SetRgbLed(
                LED_RGB,
                0,
                LED_MAX_RGB_VALUE_c,
                0
            );

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
            if(mTeamCounter < 150)
            {
                mTeamCounter++;
            }
            else
            {
                mTeamCounter = 1;
            }
        }

        if(customEvent & gMyNewTaskEvent3_c)
        {
            LED_SetRgbLed(
                LED_RGB,
                0,
                0,
                0
            );

            if(myTimerID != gTmrInvalidTimerID_c)
            {
                TMR_StopTimer(myTimerID);
            }
        }

        if(customEvent & gMyNewTaskEventSW3_c)
        {
            color = GREEN;

            LED_SetRgbLed(
                LED_RGB,
                0,
                LED_MAX_RGB_VALUE_c,
                0
            );

            if(myTimerID != gTmrInvalidTimerID_c)
            {
                TMR_StopTimer(myTimerID);

                TMR_StartIntervalTimer(
                    myTimerID,
                    1000,
                    myTaskTimerCallback,
                    NULL
                );
            }
        }

        if(customEvent & gMyNewTaskEventSW4_c)
        {
            color = BLUE;

            LED_SetRgbLed(
                LED_RGB,
                0,
                0,
                LED_MAX_RGB_VALUE_c
            );

            if(myTimerID != gTmrInvalidTimerID_c)
            {
                TMR_StopTimer(myTimerID);

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

void MyTask_SW3_Pressed(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEventSW3_c);
}

void MyTask_SW4_Pressed(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEventSW4_c);
}

void MyTimer_Init(void)
{
    mMyEvents = OSA_EventCreate(TRUE);
    gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(My_Task), NULL);
}

static void myTaskTimerCallback(void *param)
{
    (void)param;

    OSA_EventSet(
        mMyEvents,
        gMyNewTaskEvent2_c
    );
}

void MyTaskTimer_Stop(void)
{
    OSA_EventSet(
        mMyEvents,
        gMyNewTaskEvent3_c
    );
}

void MyTaskTimer_Start(void)
{
    OSA_EventSet(
        mMyEvents,
        gMyNewTaskEvent1_c
    );
}

uint16_t get_timer(void)
{
    return mTeamCounter;
}
