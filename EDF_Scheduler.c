/**
 * Project 2   : CPU Scheduling
 * Programmers : Drew Rife & Alec Waddelow
 * Course      : CMPE320
 * Section     : 2(11-12:50pm)
 * Instructor  : S. Lee
 */

#include "EDF_Scheduler.h"
 	int this_func_local_time = 0;

int main(int argc, char * argv[]) {

	/* a user must input how many threads they desire */
	if(argc == 2) {
		pthread_mutex_init(&mutex_timer, NULL);
		pthread_mutex_init(&mutex_threads, NULL);

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

/**
 * Creates all the threads and joins them upon program completion
 */
void controller() {
	int i;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	/* create all threads */
	for(i = 0; i < num_of_threads; i++) {
		
		if(i == 0) {
			/* create timer and scheduler thread */
			pthread_create(&timer_thread, &attr, timer, NULL);
			pthread_create(&scheduler_thread, &attr, scheduler, NULL);
		}
		pthread_create(&this_thread[i], &attr, runner, (void *)&list_of_threads[i]);
	}

	thread_is_ready = true; // threads are created, now they are ready

	/* join all threads */
	for(i = 0; i < num_of_threads; i++) {

		if(i == 0) {
			pthread_join(timer_thread, NULL);
			pthread_join(scheduler_thread, NULL);
		}
		pthread_join(this_thread[i], NULL);
	}			
}

/**
 * Keep track of time
 */
void * timer() {
	time_elapsed = 0;
	change_thread = false;
	timer_finished = false;
	int tmp_max_seconds = sec_to_run;

	while(tmp_max_seconds > 0) {
		sleep(1);
		time_elapsed++;
		if(time_elapsed == 5) {
			change_thread = true;
		}
		tmp_max_seconds--;
	}
	timer_finished = true;
	pthread_exit(0);
}

/**
 * Schedules which thread to run next 
 * based on earliest deadline
 */
void * scheduler() {
	int i;
	int shortest_period;

	while(!timer_finished) {
		if(change_thread) {
			thread_being_executed++;
			change_thread = false;
		}
	}
	pthread_exit(0);
}

/**
 * Runs the main threads created : prints time (sec)
 * @param  my_thread 
 */
void * runner(void * my_thread_info) {
	THREAD_INFO * tmp_thread = (THREAD_INFO *) my_thread_info;

	sem_init(&sem_ready, 0, 1);
	
	while(!timer_finished) {
		if(thread_is_ready && (tmp_thread->thread_ID == thread_being_executed)) {

			/* critical section : synchronizes threads with timer */ 
			if(this_func_local_time < time_elapsed) {
				this_func_local_time = time_elapsed;
				sem_wait(&sem_ready);
				printf("Thread being executed : %d ----- time : ", tmp_thread->thread_ID);
				printf("%02d\n", this_func_local_time);
				sem_post(&sem_ready);
			}

		}
	}

	pthread_exit(0);
}