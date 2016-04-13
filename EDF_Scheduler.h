/**
 * Project 2   : CPU Scheduling
 * Programmers : Drew Rife & Alec Waddelow
 * Course      : CMPE320
 * Section     : 2(11-12:50pm)
 * Instructor  : S. Lee
 */

#ifndef EDF_SCHEDULER_H
#define EDF_SCHEDULER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#ifndef MAX_NUM_THREADS
#define MAX_NUM_THREADS 10
#endif // MAX_NUM_THREADS


#ifndef MAX_PROG_LENGTH_SECONDS
#define MAX_PROG_LENGTH_SECONDS 1000
#endif 


/* Contains all the info related to each thread */
typedef struct THREAD_INFO {
	int thread_ID;
	int execution_time;
	int period_for_thread;
	bool is_done;
	int deadline_list[];
}THREAD_INFO;

THREAD_INFO **list_of_threads;
pthread_t this_thread[MAX_NUM_THREADS];

int num_of_threads;
int sec_to_run;

bool Check_Num_Threads(int num_of_threads);
void Request_Execution_And_Period_Times();

#endif // EDF_SCHEDULER_H
