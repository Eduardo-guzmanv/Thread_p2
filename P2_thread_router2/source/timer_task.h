/*
 * timer_task.h
 *
 *  Created on: 2 oct. 2026
 *      Author: santiagosalcedo
 */

#ifndef TIMER_TASK_H_
#define TIMER_TASK_H_

/* Fwk */
#include "TimersManager.h"
#include "FunctionLib.h"

/* KSDK */
#include "fsl_common.h"
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"

/* Define the available Task's Events */
#define gMyNewTaskEvent1_c      (1 << 0)
#define gMyNewTaskEvent2_c      (1 << 1)
#define gMyNewTaskEvent3_c      (1 << 2)

/* Define Task priority and Task size */
#define gMyTaskPriority_c       3
#define gMyTaskStackSize_c      800

/* Timer interval */
#define gCounterRequestTime_c   2000U

/* Prototype definition */
void MyTaskTimer_Start(void);
void MyTaskTimer_Stop(void);
void MyTimer_Init(void);

#endif /* TIMER_TASK_H_ */
