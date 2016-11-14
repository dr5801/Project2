/**
 * Project 2   : CPU Scheduling
 * Programmers : Drew Rife
 * Course      : CMPE320
 * Section     : 2(11-12:50pm)
 * Instructor  : S. Lee
 */

#include "EDF_Scheduler.h"

/**
 * Checks whether the number of threads the user entered is valid or not
 * @param  num_of_threads 
 * @return true or false
 */
bool check_num_threads(int num_of_threads) {
	if(num_of_threads >= 1 && num_of_threads <= MAX_NUM_THREADS) {
		return true;
	}
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
void request_execution_and_period_times() {
	list_of_threads = malloc(sizeof(THREAD_INFO) * num_of_threads);
	int i;
	int largest_period_time;
	int this_execution_time;
	int this_period;

	/* gets the execution time from the user */
	for(i = 0; i < num_of_threads; i++) {
		printf("Execution time for Thread %02d : ", i);
		scanf("%d", &this_execution_time);

		/* initializes the array */
		if(this_execution_time >= 1) {
			list_of_threads[i].thread_ID = i;
			list_of_threads[i].deadlines_completed = 0;
			list_of_threads[i].is_idling = true;
			list_of_threads[i].deadline_list_length = 0;
			list_of_threads[i].execution_time = this_execution_time;
			list_of_threads[i].can_be_ran = true;
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
		if(i == 0) {
			largest_period_time = list_of_threads[i].period_for_thread;
		}
		else if(largest_period_time < list_of_threads[i].period_for_thread) {
			largest_period_time = list_of_threads[i].period_for_thread;
		}
	}

	/* gets the input for how many seconds the user wants it to run */
	bool acceptable_time = false;
	while(!acceptable_time) {
		printf("How long do you want to execute this program (sec) : ");
		scanf("%d", &sec_to_run);

		if(sec_to_run >= largest_period_time) {
			acceptable_time = true;
		}
		else {
			printf("\nError: Your time needs to be at least the largest period time (%d)!\n", largest_period_time);
		}
	}

	/* fill in the deadline_list of the calculated deadlines for each thread */
	for(i = 0; i < num_of_threads; i++) {
		list_of_threads[i].deadline_list = malloc(sizeof(list_of_threads[i].deadline_list) * ((sec_to_run/list_of_threads[i].period_for_thread)+1));
		int j = 0;
		int sum = 0;
		for(j = 0; j < ((sec_to_run/list_of_threads[i].period_for_thread) + 1); j++) {
			sum += list_of_threads[i].period_for_thread;
			list_of_threads[i].deadline_list[j] = sum;
			total_number_deadlines++;
		}		
	}

	int k = 0;
	/* fill the deadlines in the struct array of computed the computed deadlines for each thread */
	computed_deadline_order = malloc(sizeof(DEADLINE_LIST) * total_number_deadlines+1);
	for(i = 0; i < num_of_threads; i++) {
		int j;
		int sum = 0;
		for(j = 0; j < ((sec_to_run/list_of_threads[i].period_for_thread) + 1); j++) {
			sum += list_of_threads[i].period_for_thread;
			computed_deadline_order[k].deadline = sum;
			computed_deadline_order[k].thread_num = list_of_threads[i].thread_ID;
			computed_deadline_order[k].is_done = false;
			k++;
		}
	}
	
	/* sort the deadline struct list from shortest periods to greatest periods to run threads in order */
	for(i = 0; i < total_number_deadlines; i++) {
		int y;
		for(y = 0; y < total_number_deadlines-1; y++) {
			if(computed_deadline_order[y].deadline > computed_deadline_order[y+1].deadline) {
				int temp = computed_deadline_order[y+1].deadline;
				int temp1 = computed_deadline_order[y+1].thread_num;

				computed_deadline_order[y+1].deadline = computed_deadline_order[y].deadline;
				computed_deadline_order[y+1].thread_num = computed_deadline_order[y].thread_num;

				computed_deadline_order[y].deadline = temp;
				computed_deadline_order[y].thread_num = temp1;
			}
		}
	}
}

/**
 * computationally checks to see if the threads can be ran
 */
bool threads_meet_deadlines() {
	int i;
	double sumnation = 0.0;
	bool can_execute;

	for(i = 0; i < num_of_threads; i++) {
		sumnation += ((double)list_of_threads[i].execution_time/(double)list_of_threads[i].period_for_thread);
	}

	return (sumnation <= 1.0) ? true:false;
}

/**
 * deconstructs the lists
 */
void free_list() {
	int i;
	for(i = 0; i < num_of_threads; i++) {
		free(list_of_threads[i].deadline_list);
	}
	free(computed_deadline_order);

	free(list_of_threads);
}