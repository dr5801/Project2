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
		

		bool valid = check_num_threads( atoi(argv[1]) );
		bool can_execute = false;

		if(valid) {
			num_of_threads = atoi(argv[1]);
			request_exection_and_period_times();
		
			if(threads_meet_deadlines()) {
				thread_being_executed = computed_deadline_order[0].thread_num;
				list_of_threads[thread_being_executed].is_idling = false;
				deadline_being_ran = 0;
				cpu_idle = false;
				controller();
			}
			else
				printf("\nThese threads can't be scheduled. Program will exit.");
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
			// pthread_create(&checker_thread, &attr, scheduler, NULL);
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
	int local_time = 0;
	change_thread = false;
	timer_finished = false;
	int tmp_max_seconds = sec_to_run;

	sleep(1);
	while(tmp_max_seconds > 0) {

		time_elapsed++;
		local_time++;
		sleep(1);
		if(local_time == list_of_threads[thread_being_executed].execution_time && !cpu_idle) {
			list_of_threads[thread_being_executed].can_be_ran = false;
			list_of_threads[thread_being_executed].deadlines_completed++;
			list_of_threads[thread_being_executed].is_idling = true;
			computed_deadline_order[deadline_being_ran].is_done = true;
			change_thread = true;
			local_time = 0;
		}
		else if(cpu_idle) {
			if(!list_of_threads[computed_deadline_order[deadline_being_ran + 1].thread_num].can_be_ran) {
				cpu_idle = false;
			}
			printf("PRINT!!!!");
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
	int i, j, distance;
	int shortest_period;
	int thread_deadlines_met[num_of_threads];
	bool previous_thread_needs_ran = false;
	bool just_idle = false;
	bool found = false;

	printf("\nThread being executed : %d\n", thread_being_executed);

	i = 0;
	while(!timer_finished) {
		if(change_thread) {

			/* look left of current thread being executed */
			j = i;
			while(j > 0 && !previous_thread_needs_ran) {
				if(!computed_deadline_order[j].is_done) {
					previous_thread_needs_ran = true;
					distance = i-j;
				}

				if(!previous_thread_needs_ran) {
					j--;
				}
			}

			int tmp = i;
			if(!previous_thread_needs_ran) {
				while (i < total_number_deadlines && !found) {
					
					if(list_of_threads[computed_deadline_order[i+1].thread_num].deadlines_completed == 0) {

						thread_being_executed = computed_deadline_order[i+1].thread_num;
						found = true;
						i++;

						printf("first if");
					}
					/* checking if total time >= the next thread's previous deadline */
					else if(list_of_threads[computed_deadline_order[i+1].thread_num].can_be_ran
						&& !computed_deadline_order[i+1].is_done) {
						thread_being_executed = computed_deadline_order[i+1].thread_num;
						found = true;
						i++;
						printf("DID YOU NOT MAKEIT HERE?!?!?!!?");
					}
					
					if(!found) {
						i++;
					}
				}
				

				if(found) {
					printf("\nThread being executed : %d\n", thread_being_executed);
					list_of_threads[thread_being_executed].is_idling = false;
					deadline_being_ran = i;
					found = false;
					change_thread = false;
				}
				else {
					printf("NOTHING FOUND!!!!!");
					i = tmp;
					cpu_idle = true;
					change_thread = false;					
				}
			} 
			else {
				i = j;
				thread_being_executed = computed_deadline_order[i].thread_num;
				list_of_threads[thread_being_executed].is_idling = false;
				printf("\nThread being executed : %d\n", thread_being_executed);
				deadline_being_ran = i;
				change_thread = false;
			}
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
	static int this_func_local_time = 0;

	pthread_mutex_init(&mutex_threads, NULL);
	sem_init(&sem_ready, 0, 1);
	while(!timer_finished) {

			if(tmp_thread->deadlines_completed > 0) {
				pthread_mutex_lock(&mutex_threads);
				if(time_elapsed >= tmp_thread->deadline_list[tmp_thread->deadlines_completed - 1] && !tmp_thread->can_be_ran) {
					tmp_thread->can_be_ran = true;
				}
				pthread_mutex_unlock(&mutex_threads);
			}
			
			if((tmp_thread->thread_ID == thread_being_executed) && (tmp_thread->can_be_ran) && (!tmp_thread->is_idling) ) {

				/* critical section : synchronizes threads with timer */ 
				if(this_func_local_time < time_elapsed && !change_thread) {
					sem_wait(&sem_ready);
					printf("%02d aaaaaand its deadline time -> %02d\n", time_elapsed, tmp_thread->deadline_list[tmp_thread->deadlines_completed]);
					sem_post(&sem_ready);
					this_func_local_time = time_elapsed;
				}

			}
	}

	pthread_exit(0);
}