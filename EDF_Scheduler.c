/**
 * Project 2   : CPU Scheduling
 * Programmers : Drew Rife & Alec Waddelow
 * Course      : CMPE320
 * Section     : 2(11-12:50pm)
 * Instructor  : S. Lee
 */

#include "EDF_Scheduler.h"

int main(int argc, char * argv[]) {

	/* a user must input how many threads they desire */
	if(argc == 2) {
		pthread_mutex_init(&mutex, NULL);

		bool valid = Check_Num_Threads( atoi(argv[1]) );

		if(valid) {
			num_of_threads = atoi(argv[1]);
			Request_Execution_And_Period_Times();
			controller();

			
		}

		
		free_list();  // deconstruct the list
	}
	else if(argc < 2){
		printf("Error: You didn't enter how many threads you wanted. TERMINATING...\n");
	}
	else {
		printf("Error: You only need to enter how many threads you want. Nothing else. TERMINATING...\n");
	}
}

void controller() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&timer_thread, &attr, timer, NULL);
	pthread_create(&scheduler_thread, &attr, scheduler, NULL);

	

	pthread_join(timer_thread, NULL);
	pthread_join(scheduler_thread, NULL);
}

void * timer() {
	int tmp_max_seconds = sec_to_run;
	time_elapsed = 0;
	change_thread = false;

	while(tmp_max_seconds > 0) {

		if(time_elapsed == 5) {
			change_thread = true;
		}
		sleep(1);
		time_elapsed++;
		printf("%02d\n", time_elapsed);
		tmp_max_seconds--;
	}
}

void * scheduler() {
	while(true) {
		if(change_thread) {
			printf("\nYOU MADE IT INTO SCHEDULER\n");
			change_thread = false;
		}
	}
}