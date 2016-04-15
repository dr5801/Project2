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
#endif  // MAX_PROG_LENGTH_SECONDS


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

void * timer();			// keeps track of time
void * scheduler();		// schedules what thread goes next
void * runner();		// runs the main threads 
void controller();		// creates the threads


pthread_t this_thread[MAX_NUM_THREADS];  // threads that run and print time
pthread_t timer_thread;					 // thread to keep track of time
pthread_t scheduler_thread;				 // thread to change which thread runs next

/* semaphore and mutex locks for threads */
pthread_mutex_t mutex_timer;
pthread_mutex_t mutex_threads;
sem_t sem_ready;

int num_of_threads;
int sec_to_run;
int time_elapsed;
int thread_being_executed;
bool thread_is_ready;

/* make flags volatile so compiler doesn't ignore them */
volatile bool change_thread;
volatile bool timer_finished;
volatile static bool print;

#endif // EDF_SCHEDULER_H