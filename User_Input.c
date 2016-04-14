#include "EDF_Scheduler.h"

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


/**
 * Requests the execution time for each thread from the user.
 * Requests the period time for each thread from the user.
 * Requests how long (sec) the user would like it to run.
 */
void Request_Execution_And_Period_Times() {
	list_of_threads = malloc(sizeof(THREAD_INFO) * num_of_threads);
	int i;
	int largest_period_time;
	int this_execution_time;
	int this_period;

	/* gets the execution time from the user */
	for(i = 0; i < num_of_threads; i++) {
		printf("Execution time for Thread %02d : ", i);
		scanf("%d", &this_execution_time);

		if(this_execution_time >= 1) {
			list_of_threads[i].execution_time = this_execution_time;
			list_of_threads[i].thread_ID = i;
			list_of_threads[i].is_done = false;
		}
		else {
			printf("\nError : You need to have an execution time of at least 1!\n");
			i--;
		}
	}

	/* gets the period times for the threads */
	for(i = 0; i < num_of_threads; i++) {
		this_period = 0;
		printf("Period for Thread %02d : ", i);
		scanf("%d", &this_period);

		if(this_period >= list_of_threads[i].execution_time) {
			list_of_threads[i].period_for_thread = this_period;
		}
		else {
			printf("\nError : The period needs to be at laest the execution time!\n");
			i--;
		}
	}

	/* find the largest period of time */
	for(i = 0; i < num_of_threads; i++) {
		if(i == 0)
			largest_period_time = list_of_threads[i].period_for_thread;
		else if(largest_period_time < list_of_threads[i].period_for_thread)
			largest_period_time = list_of_threads[i].period_for_thread;
	}

	/* gets the input for how many seconds the user wants it to run */
	bool acceptable_time = false;
	while(!acceptable_time) {
		printf("How long do you want to execute this program (sec) : ");
		scanf("%d", &sec_to_run);

		if(sec_to_run >= largest_period_time) {
			acceptable_time = true;
		}
		else
			printf("\nError: Your time needs to be at least the largest period time (%d)!\n", largest_period_time);
	}

	/* fill in the deadline_list of the calculated deadlines for each thread */
	for(i = 0; i < num_of_threads; i++) {
		list_of_threads[i].deadline_list = malloc(sizeof(list_of_threads[i].deadline_list) * ((sec_to_run/list_of_threads[i].period_for_thread)+1));
		int j = 0;
		int sum = 0;
		for(j = 0; j < ((sec_to_run/list_of_threads[i].period_for_thread) + 1); j++) {
			sum += list_of_threads[i].period_for_thread;
			list_of_threads[i].deadline_list[j] = sum;
		}		
	}
}

/**
 * free everything :) - LIBERATION!!!!!!
 *
 * (memory leaks are bad)
 */
void free_list() {
	int i;
	for(i = 0; i < num_of_threads; i++) {
		free(list_of_threads[i].deadline_list);
	}
	free(list_of_threads);
}