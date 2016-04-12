/**
 * Project 2   : CPU Scheduling
 * Programmers : Drew Rife & Alec Waddelow
 * Course      : CMPE320
 * Section     : 2(11-12:50pm)
 * Instructor  : S. Lee
 */

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

/* Contains all the info related to each thread */
typedef struct THREAD_INFO {
	int thread_ID;
	int execution_time;
	int period_for_thread;
	bool is_done;
}THREAD;

THREAD **list_of_threads;
pthread_t this_thread[MAX_NUM_THREADS];

int num_of_threads;

bool Check_Num_Threads(int num_of_threads);

int main(int argc, char * argv[]) {

	/* a user must input how many threads they desire */
	if(argc == 2) {
		bool valid = Check_Num_Threads( atoi(argv[1]) );

		if(valid) {
			num_of_threads = atoi(argv[1]);

		}
		
	}
	else if(argc < 2){
		printf("Error: You didn't enter how many threads you wanted. TERMINATING...\n");
	}
	else {
		printf("Error: You only need to enter how many threads you want. Nothing else. TERMINATING...\n");
	}
}

/**
 * Checks whether the number of threads the user entered is valid or not
 * @param  num_of_threads 
 * @return true or false
 */
bool Check_Num_Threads(int num_of_threads) {
	if(num_of_threads >= 1 && num_of_threads <= MAX_NUM_THREADS)
		return true;
	else if(num_of_threads < 1) {
		printf("ERROR: You entered too few threads.  You need at least 1. TERMINATING...\n");
		return false;
	}
	else {
		printf("ERROR: You entered too many threads.  Max is 10. TERMINATING...\n");
		return false;
	}
}