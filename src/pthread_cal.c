#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
	int res;
	int is_wait;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
} Result;

// Define a thread proc function and put result into res
void* set_fn(void *arg)
{
	int i = 1, sum = 0;
	for(; i <= 100; i++)
	{
		sum += i;
	}
	Result *r = (Result*)arg;
	
	// Push result into res
	r->res = sum;

	// Lock
	pthread_mutex_lock(&r->mutex);

	// Judge whether thread is ready
	while(!r->is_wait)
	{
		// Firstly check released lock
		pthread_mutex_unlock(&r->mutex);
		usleep(100);
		// If not, lock
		pthread_mutex_lock(&r->mutex);
	}

	// Unlock
	pthread_mutex_unlock(&r->mutex);

	pthread_cond_broadcast(&r->cond);

	// Get res
	int res = r->res;
	printf("0x%lx get sum is %d\n", pthread_self(), res);

	return (void*)0;
}

// Get result proc
void* get_fn(void *arg)
{
	Result *r = (Result*)arg;
	
	// Protect shared src variables
	pthread_mutex_lock(&r->mutex);

	// Thread is ready
	r->is_wait = 1;

	pthread_cond_wait(&r->cond, &r->mutex);

	// Unlock 
	pthread_mutex_unlock(&r->mutex);
	
	return (void*)0;
}

// Get result proc
int main(void)
{
	int err;
	pthread_t cal, get;
	Result r;
	r.is_wait = 0;
	pthread_cond_init(&r.cond, NULL);
	pthread_mutex_init(&r.mutex, NULL);

	// Start thread quirying result
	err = pthread_create(&get, NULL, get_fn, (void*)&r);
	if(err)
	{
		fprintf(stderr, "pthread create error!\n");
	}
	// Start thread calculating
	err = pthread_create(&cal, NULL, set_fn, (void*)&r);
	if(err)
	{
		fprintf(stderr, "pthread create error!\n");
	}

	pthread_join(cal, NULL);
	pthread_join(get, NULL);

	pthread_cond_destroy(&r.cond);
	pthread_mutex_destroy(&r.mutex);

	return 0;
}
