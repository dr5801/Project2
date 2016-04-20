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
	int deadlines_completed;
	bool is_idling;
	bool can_be_ran;
	int deadline_list_length;
	int * deadline_list;
}THREAD_INFO;

typedef struct DEADLINE_LINK_THREAD {
	int thread_num;
	bool cpu_idle;
	bool is_done;
	int deadline;
}DEADLINE_LINK_THREAD;

THREAD_INFO * list_of_threads;
DEADLINE_LINK_THREAD * computed_deadline_order;

/* functions in User_Input file */
void free_list();

void * timer();						// keeps track of time
void * scheduler();					// schedules what thread goes next
void * runner();					// runs the main threads 
void * checker();
void controller();					// creates the threads
void predict_thread_executions(); 	// orders which sequence of threads should be ran
bool check_num_threads(int num_of_threads);
void request_exection_and_period_times();
bool threads_meet_deadlines();

pthread_t this_thread[MAX_NUM_THREADS];  // threads that run and print time
pthread_t timer_thread;					 // thread to keep track of time
pthread_t scheduler_thread;				 // thread to change which thread runs next
pthread_t checker_thread;

/* semaphore and mutex locks for threads */
pthread_mutex_t mutex_timer;
pthread_mutex_t mutex_threads;
sem_t sem_ready;

/* make all variables used between processes volatile so compiler doesn't ignore them */
volatile int num_of_threads;
volatile int sec_to_run;
volatile int time_elapsed;
volatile int thread_being_executed;
volatile int current_period;
volatile int deadline_times[1000];
volatile int predicted_executions[1000];
volatile bool thread_is_ready;
volatile int total_number_deadlines;
volatile bool change_thread;
volatile bool timer_finished;
volatile static bool print;
volatile int deadline_being_ran;

#endif // EDF_SCHEDULER_H