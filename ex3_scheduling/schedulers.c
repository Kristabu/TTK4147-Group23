#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "scheduling.h"
#include "schedulers.h"

void set_task_state(struct Task *task, enum taskState taskNewState)
{
    pthread_mutex_lock(&taskStateMutex);
    task->state = taskNewState;
    pthread_mutex_unlock(&taskStateMutex);
}

void wait_for_rescheduling(int quantum, struct Task *task)
{
    int startTime;
    int waitTime;

    pthread_mutex_lock(&timeMutex);
    startTime = globalTime;
    pthread_mutex_unlock(&timeMutex);

    do
    {
        pthread_mutex_lock(&timeMutex);
        pthread_cond_wait(&timeCond, &timeMutex);
        waitTime = globalTime - startTime;
        pthread_mutex_unlock(&timeMutex);
    } while (task->state != finished && waitTime < quantum);

    usleep(timeUnitUs / 100);
}

void round_robin(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval
        wait_for_rescheduling(quantum, tasks[taskIndex]);

        //  Check if the task is finished
        if (tasks[taskIndex]->state == finished)
        {
        }
        else
        {
            set_task_state(tasks[taskIndex], preempted);
        }

        // Find the next task to run
        taskIndex = (taskIndex + 1) % taskCount;

    } while (globalTime < timeout);
}

// Implement your schedulers here!
void first_come_first_served(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for task to finish
        while (tasks[taskIndex]->state != finished)
        {
            wait_for_rescheduling(1, tasks[taskIndex]);
        }

        // Find the next task to run
        taskIndex = (taskIndex + 1) % taskCount;

    } while (globalTime < timeout);
}

void shortest_process_next(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
    int fallbackIdx = 0;
	printf("i have started");
    do
    {
        for (int taskIndex = 0; taskIndex < taskCount; taskIndex++)
        {
		printf("Task id: %d, Fallback id %d", taskIndex, fallbackIdx);
            // Skip finished tasks or those that have not arrived yet
            if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
            {
                // taskIndex = (taskIndex + 1) % taskCount;
                continue;
            }
	    //check runtime against fallback
	    if(fallbackIdx == -1)
	    {
	       	fallbackIdx = taskIndex;
	    }
	    if(tasks[taskIndex]->totalRuntime < tasks[fallbackIdx]->totalRuntime)
	    {
		fallbackIdx = taskIndex;
	    }
        }

	if(fallbackIdx == -1){
		continue;
	}

        // Set the task state to running
        if (tasks[fallbackIdx]->startTime == -1)
            tasks[fallbackIdx]->startTime = globalTime;
        set_task_state(tasks[fallbackIdx], running);
	printf("Task %d set to running", fallbackIdx);
        // Wait for task to finish
        while (tasks[fallbackIdx]->state != finished)
        {
            wait_for_rescheduling(1, tasks[fallbackIdx]);
        }
    } while (globalTime < timeout);
}
void highest_response_ratio_next(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
}
void shortest_remaining_time(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here
}
void feedback(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here
}

