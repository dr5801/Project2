/**
 * Lab 5      : Thread Scheduling
 * Programmer : Drew Rife
 * Course     : CMPE 320
 * Section    : 2(11-12:50pm)
 * Instructor : S.Lee
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

#ifndef POLICY
#define POLICY SCHED_RR
#endif // POLICY

typedef struct THREAD_INFO {
	int threadID;
	int burstTime;
	bool done;
}THREAD;

THREAD **listOfThreads;
pthread_t thisThread[MAX_NUM_THREADS];

void requestBurstTimes();
void *scheduler();
void changeThread();

int numOfThreads;
int timeQuantum;
bool isReady;
int totalAmountOfTime;
int threadBeingExecuted;

sem_t ready;

/**
 * Main method to run program
 * @param  argc number of command line arguments
 * @param  argv what the arguments are
 * @return 0 program executed successfully
 */
int main(int argc, char *argv[]) {
	numOfThreads = 0;
	timeQuantum = 0;
	totalAmountOfTime = 0;
	threadBeingExecuted = 0;
	isReady = false;
	sem_init(&ready, 0, 1);

	/* check to see if the user entered in the correct amount of arguments */
	if (argc == 3) {
		numOfThreads = atoi(argv[1]);
		timeQuantum = atoi(argv[2]);

		/* user did enter the correct number */
		if(numOfThreads >= 1 && numOfThreads <= MAX_NUM_THREADS
			&& timeQuantum > 0) {

			listOfThreads = malloc(sizeof(THREAD *)*numOfThreads);
			
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
			pthread_attr_setschedpolicy(&attr, POLICY);

			int i = 0;
			for( i = 0; i < numOfThreads; i++) {
				unsigned long long int thisBurstTime = 0;
				printf("Burst time for Thread %02d : ", i);
				scanf("%llu", &thisBurstTime);
				if(thisBurstTime <= 0) {
					printf("***WARNING*** : You need to have a burst time of at least 1!\n");
					i--;
				}
				else {
					listOfThreads[i] = malloc(sizeof(THREAD));
					listOfThreads[i]->burstTime = thisBurstTime;
					listOfThreads[i]->threadID = i;
					listOfThreads[i]->done = false;
				}
				printf("\n");				
			}

			/* create threads */
			for(i = 0; i < numOfThreads; i++) {
				pthread_create(&thisThread[i], &attr, scheduler, listOfThreads[i]);
			}			

			isReady = true;  // threads are ready

			/* join threads */
			for(i = 0; i < numOfThreads; i++) {
				pthread_join(thisThread[i], NULL);
			}

			printf("\tAll threads have done their work. The scheduler thread exited\n%d\n", ++totalAmountOfTime);
		}
	}
	else if(argc < 3) 
		printf("***WARNING***: Too few parameters entered. Exiting\n");
	else if(argc > 3)
		printf("***WARNING***: Too many parameters entered. Exiting\n");
	else if(numOfThreads < 1 || numOfThreads > MAX_NUM_THREADS)
		printf("***WARNING***: You need to enter a number between 1 - 10 for number of threads. Exiting.\n");
	else if(timeQuantum < 0)
		printf("***WARNING***: You need to enter a time quantum of at least 1. Exiting.\n");
	return 0;
}

/**
 * Scheduler function that prints the time and what thread is being executed
 * @param  myThread thread being passed in
 */
void *scheduler(void *myThread) {
	THREAD * tmpThread = (THREAD *) myThread;
	int timeLeft = tmpThread->burstTime;
	int runNow = 0;

	while(timeLeft > 0) {
		runNow = timeLeft;

		if(runNow > timeQuantum) 
			runNow = timeQuantum;

		/* lock critical section */
		sem_wait(&ready);
		if(isReady && (tmpThread->threadID == threadBeingExecuted)) {
			printf("\tThread %02d is being executed :\n", threadBeingExecuted);
			while(runNow > 0) {
				sleep(1);				
				totalAmountOfTime++;
				printf("%02d\n", totalAmountOfTime);
				timeLeft--;
				runNow--;
			}
			changeThread();			
		}
		sem_post(&ready);
		/* unlock critical section */
	}
	listOfThreads[tmpThread->threadID]->done = true;
	printf("\tThread %02d finishes its job. It will be terminated\n", tmpThread->threadID);
	pthread_exit(0);  // exit the thread
}

/**
 * changes the thread
 */
void changeThread() {
	threadBeingExecuted++;

	/* if current thread is last thread, automatically go back to thread 0 */
	if(threadBeingExecuted == numOfThreads)
		threadBeingExecuted = 0;

	/* if that thread is done then change to another thread */
	if(listOfThreads[threadBeingExecuted]->done) 
		changeThread();
}