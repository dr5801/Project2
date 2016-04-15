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
		pthread_mutex_init(&mutex_timer, NULL);
		pthread_mutex_init(&mutex_threads, NULL);

		bool valid = Check_Num_Threads( atoi(argv[1]) );

		if(valid) {
			num_of_threads = atoi(argv[1]);
			Request_Execution_And_Period_Times();
			
			sem_init(&sem_ready, 0, 1);
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
	int i;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	thread_being_executed = 0;


	for(i = 0; i < num_of_threads; i++) {
		
		if(i == 0) {
			/* create timer and scheduler thread */
			pthread_create(&timer_thread, &attr, timer, NULL);
			pthread_create(&scheduler_thread, &attr, scheduler, NULL);
		}
		pthread_create(&this_thread[i], &attr, runner, list_of_threads);
	}
	thread_is_ready = true;


	for(i = 0; i < num_of_threads; i++) {

		if(i == 0) {
			pthread_join(timer_thread, NULL);
			pthread_join(scheduler_thread, NULL);
		}
		pthread_join(this_thread[i], NULL);
	}
}

void * timer() {
	int tmp_max_seconds = sec_to_run;
	timer_finished = false;
	time_elapsed = 0;
	change_thread = false;

	while(tmp_max_seconds > 0) {
		if(time_elapsed == 5) {
			change_thread = true;
		}
		sleep(1);
		time_elapsed++;
		tmp_max_seconds--;
	}
	timer_finished = true;
}

void * scheduler() {
	while(!timer_finished) {
		if(change_thread) {
			printf("YOU MADE IT INTO SCHEDULER\n");
			change_thread = false;
		}
	}
}

void * runner(void *my_thread) {
	THREAD_INFO * tmp_thread = (THREAD_INFO *) my_thread;

	while(!timer_finished) {
		if(thread_is_ready && (tmp_thread->thread_ID == thread_being_executed)) {
			

			/**
			 * ALEC: this is how I was able to sync them.
			 * If you move the if statement then the output won't be correct. 
			 * Don't know if you wanna waste time to try some other method to work
			 * but at least now we have something that works.
			 */
			if(!timer_finished) {
				sem_wait(&sem_ready);
				sleep(1);
				printf("%02d\n", time_elapsed);
				sem_post(&sem_ready);
			}

		}
	}
}