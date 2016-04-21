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


/*--------------- Contains all the info related to each thread ---------------*/
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

/*--------------- a sorted list of which deadlines to run next ---------------*/
typedef struct DEADLINE_LINK_THREAD {
	int thread_num;
	bool is_done;
	int deadline;
}DEADLINE_LINK_THREAD;

/*--------------- FUNCTIONS for threads and user input ---------------*/
void * timer();								// keeps track of time
void * scheduler();							// schedules what thread goes next
void * runner();							// runs the main threads 
void controller();							// creates the threads
void predict_thread_executions(); 			// orders which sequence of threads should be ran
bool check_num_threads(int num_of_threads); // checks if the number of threads the user enters is valid or not
void request_execution_and_period_times();	// user input for execution and period times
bool threads_meet_deadlines();				// checks to see if threads meet deadlines
void free_list();

/*--------------- All threads used ---------------*/
pthread_t this_thread[MAX_NUM_THREADS];  // threads that run and print time
pthread_t timer_thread;					 // thread to keep track of time
pthread_t scheduler_thread;				 // thread to change which thread runs next

/*--------------- semaphore and mutex locks for threads ---------------*/
pthread_mutex_t mutex_threads;
sem_t sem_ready;

/*--------------- make all variables shared between processes volatile so compiler doesn't ignore them ---------------*/
volatile int num_of_threads;
volatile int sec_to_run;
volatile int time_elapsed;
volatile int thread_being_executed;
volatile bool thread_is_ready;
volatile int total_number_deadlines;
volatile bool change_thread;
volatile bool timer_finished;
volatile int deadline_being_ran;
volatile static bool cpu_idle;

/*--------------- List of threads and the struct array of what deadlines need ran next ---------------*/
THREAD_INFO * list_of_threads;
DEADLINE_LINK_THREAD * computed_deadline_order;

#endif // EDF_SCHEDULER_H