/**
 * Project 2   : CPU Scheduling
 * Programmers : Drew Rife
 * Course      : CMPE320
 * Section     : 2(11-12:50pm)
 * Instructor  : S. Lee
 */

#include "EDF_Scheduler.h"

int main(int argc, char * argv[]) {

	/* a user must input how many threads they desire */
	if(argc == 2) {
		bool valid = check_num_threads( atoi(argv[1]) );

		/* check to see if the user entered in valid numbers */
		if(valid) {
			num_of_threads = atoi(argv[1]);
			request_execution_and_period_times();
		
			/* check to see if the threads can meet their deadlines */
			if(threads_meet_deadlines()) {
				thread_being_executed = computed_deadline_order[0].thread_num;
				list_of_threads[thread_being_executed].is_idling = false;
				deadline_being_ran = 0;
				cpu_idle = false;

				controller();
				printf("killed\n");
			}
			else
				printf("\nThese threads can't be scheduled. Program will exit.\n\n");
		}
		
		free_list();  // deconstruct the list
	}
	else if(argc < 2){
		printf("Error: You didn't enter how many threads you wanted. TERMINATING...\n");
	}
	else {
		printf("Error: You only need to enter how many threads you want. Nothing else. TERMINATING...\n");
	}
	return 0;
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
	int local_time = 0;
	change_thread = false;
	timer_finished = false;

	while(time_elapsed < (sec_to_run)) {

		sleep(1);
		time_elapsed++;
		local_time++;
		printf("%02d\n", time_elapsed);

		/* change threads if local time = the current thread running's execution time */
		if(local_time == list_of_threads[thread_being_executed].execution_time && !cpu_idle) {
			list_of_threads[thread_being_executed].can_be_ran = false;
			list_of_threads[thread_being_executed].deadlines_completed++;
			list_of_threads[thread_being_executed].is_idling = true;
			computed_deadline_order[deadline_being_ran].is_done = true;
			change_thread = true;
			local_time = 0;
		}

		/* while the cpu is idling */
		while(cpu_idle) {
			sleep(1);
			time_elapsed++;
			local_time = 0;
			printf("%02d\n", time_elapsed);
		}
	}
	timer_finished = true;
	pthread_exit(0);  // exit the thread when fully done
}

/**
 * Schedules which thread to run next
 */
void * scheduler() {
	int i, j;
	bool previous_thread_needs_ran = false;

	printf("\nThread being executed : %d\n", thread_being_executed);

	i = 0;
	while(time_elapsed < sec_to_run) {
		if(change_thread) {

			j = i;
			/* look left of current thread being executed to make sure we didn't skip a thread*/
			while(j > 0 && !previous_thread_needs_ran) {
				if(!computed_deadline_order[j].is_done) {
					if(list_of_threads[computed_deadline_order[j].thread_num].can_be_ran) {
						previous_thread_needs_ran = true;
					}
				}

				if(!previous_thread_needs_ran) {
					j--;
				}
			}

			int tmp = i;
			/* if previous thread doesn't need ran -> look to the right */
			if(!previous_thread_needs_ran) {
				bool found = false;
				while (i < total_number_deadlines && !found) {
					
					if(list_of_threads[computed_deadline_order[i+1].thread_num].deadlines_completed == 0) {
						
						thread_being_executed = computed_deadline_order[i+1].thread_num;
						found = true;
					}
					/* checking if total time >= the next thread's previous deadline */
					else if(list_of_threads[computed_deadline_order[i+1].thread_num].can_be_ran
						&& !computed_deadline_order[i+1].is_done
						&& list_of_threads[computed_deadline_order[i+1].thread_num].is_idling) {
						
						thread_being_executed = computed_deadline_order[i+1].thread_num;
						found = true;
					}
					
					i++;
				}
				
				/* if something was found - run it */
				if(found) {
					printf("Thread being executed : %d\n", thread_being_executed);
					list_of_threads[thread_being_executed].is_idling = false;
					deadline_being_ran = i;
					found = false;
					change_thread = false;
				}
				/* if nothing can be ran then CPU Idling */
				else {
					printf("CPU is idling now\n");
					i = tmp;
					cpu_idle = true;
					change_thread = false;					
				}
			} 
			/* a previous thread needs ran now that time_elapsed is now >= to that thread's last period time */
			else {
				i = j;
				thread_being_executed = computed_deadline_order[i].thread_num;
				list_of_threads[thread_being_executed].is_idling = false;
				printf("Thread being executed : %d\n", thread_being_executed);
				deadline_being_ran = i;
				previous_thread_needs_ran = false;
				change_thread = false;
			}
		}
	}
	pthread_exit(0);  // exit the thread when fully done
}

/**
 * Runs the main threads created
 * @param  my_thread_info
 */
void * runner(void * my_thread_info) {
	THREAD_INFO * tmp_thread = (THREAD_INFO *) my_thread_info;

	pthread_mutex_init(&mutex_threads, NULL);
	sem_init(&sem_ready, 0, 1);
	while(!timer_finished) {

		/* if CPU isn't idling */
		if(!cpu_idle) {
			if(tmp_thread->deadlines_completed > 0) {
				pthread_mutex_lock(&mutex_threads);
				if(time_elapsed+1 >= tmp_thread->deadline_list[tmp_thread->deadlines_completed - 1] && !tmp_thread->can_be_ran) {
					tmp_thread->can_be_ran = true;
				}
				pthread_mutex_unlock(&mutex_threads);
			}
		}
		/* cpu is idling */
		else {
			if(tmp_thread->deadlines_completed > 0) {
				sem_wait(&sem_ready);
				if(time_elapsed >= tmp_thread->deadline_list[tmp_thread->deadlines_completed - 1] && !tmp_thread->can_be_ran) {
					tmp_thread->can_be_ran = true;
					cpu_idle = false;
					change_thread = true;
				}
				sem_post(&sem_ready);
			}
		}
	}
	pthread_exit(0); // exit the thread when finished
}