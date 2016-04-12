/**
 * Project 2   : CPU Scheduling
 * Programmers : Drew Rife & Alec Waddelow
 * Course      : CMPE320
 * Section     : 2(11-12:50pm)
 * Instructor  : S. Lee
 */

int main(int argc, char * argv[]) {

	/* a user must input how many threads they desire */
	if(argc == 2) {
		bool valid = Check_Num_Threads( atoi(argv[1]) );

		if(valid) {
			num_of_threads = atoi(argv[1]);
			Request_Execution_And_Period_Times();
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

/**
 * Requests the execution time for each thread from the user.
 * Requests the period time for each thread from the user.
 * Requests how long (sec) the user would like it to run.
 */
void Request_Execution_And_Period_Times() {
	list_of_threads = malloc(sizeof(THREAD *) * num_of_threads);
	int i = 0;
	int largest_period_time = 0;

	/* gets the execution time from the user */
	while(i < num_of_threads) {
		unsigned int this_execution_time = 0;
		printf("\nExecution time for Thread %02d : ", i);
		scanf("%d", &this_execution_time);
		
		if(this_execution_time >= 1) {
			list_of_threads[i] = malloc(sizeof(THREAD));
			list_of_threads[i]->execution_time = this_execution_time;
			list_of_threads[i]->thread_ID = i;
			list_of_threads[i]->is_done = false;
			i++;
		}
		else
			printf("\nError : You need to have an execution time of at least 1!\n");
	}

	i = 0;
	/* gets the period times for the threads */
	while(i < num_of_threads) {
		unsigned int this_period = 0;
		printf("Period for Thread %02d : ", i);
		scanf("%d", &this_period);

		if(this_period >= list_of_threads[i]->execution_time) {
			list_of_threads[i]->period_for_thread = this_period;
			printf("\n");
			i++;
		}
		else
			printf("\nError : The period needs to be at least the execution time!\n");
	}

	for(i = 0; i < num_of_threads; i++) {
		if(i == 0)
			largest_period_time = list_of_threads[i]->period_for_thread;
		else if(largest_period_time < list_of_threads[i]->period_for_thread)
			largest_period_time = list_of_threads[i]->period_for_thread;
	}

	bool acceptable_time = false;
	while(!acceptable_time) {
		printf("How long do you want to execute this program (sec) : ");
		scanf("%d", &sec_to_run);

		if(sec_to_run >= largest_period_time)
			acceptable_time = true;
		else
			printf("\nError: Your time needs to be at least the largest period time (%d)!\n", largest_period_time);
	}
}