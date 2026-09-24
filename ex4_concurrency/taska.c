#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

// Barrier code taken from:
// Source - https://stackoverflow.com/a/12907534
// Posted by nneonneo
// Retrieved 2026-09-24, License - CC BY-SA 3.0


//time ./a.out with sem took longer and had more time in user than sys
//user time is approx doulbe the real time because there are two threads being processed

pthread_barrier_t barr;
int global_var = 0;

sem_t sem;

void thread_worker() {
    // do work

    // now make all the threads sync up
    int res = pthread_barrier_wait(&barr);
    if(res == PTHREAD_BARRIER_SERIAL_THREAD) {
        // this is the unique "serial thread"; you can e.g. combine some results here
    } else if(res != 0) {
        // error occurred
    } else {
        // non-serial thread released
    }
}


    // Note the argument and return types: void
void* increment(void* args){
	thread_worker();
	int self = 0;
	pthread_t thisThread = pthread_self();

	printf("Pthread %lu started \n", (unsigned long)thisThread);
	for(int i = 0; i < 100000; i ++) {
		self += 1;
		sem_wait(&sem);
		global_var += 1;
		sem_post(&sem);

	}
	printf("Pthread %lu self: %d \n", (unsigned long)thisThread, self);
	

	return NULL;
}

int main() {
    pthread_t threadHandle[2];
    sem_init(&sem, 0, 1);
    int nthreads = 2;
    pthread_barrier_init(&barr, NULL, nthreads);

    int i;
    for(i=0; i<nthreads; i++) {
        // create threads
	printf("Thread %d created \n", i);
        pthread_create(&threadHandle[i], NULL, increment, NULL);    
    }

    for(i=0; i<nthreads; i++) 
    {
	 pthread_join(threadHandle[i], NULL);
    }
    sem_destroy(&sem);
    pthread_barrier_destroy(&barr);
    printf("Global variable is: %d \n", global_var);

}

