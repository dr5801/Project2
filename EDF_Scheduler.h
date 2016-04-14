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
	int * deadline_list;
}THREAD_INFO;

THREAD_INFO * list_of_threads;


/* functions in User_Input file */
bool Check_Num_Threads(int num_of_threads);
void Request_Execution_And_Period_Times();
void free_list();

void * timer();
void controller();
void * scheduler();
void * thread_execution();

int num_of_threads;
int sec_to_run;
int time_elapsed;

bool change_thread;
bool program_finished;

pthread_t this_thread[MAX_NUM_THREADS];
pthread_t timer_thread;
pthread_t scheduler_thread;

pthread_mutex_t mutex;

#endif // EDF_SCHEDULER_H