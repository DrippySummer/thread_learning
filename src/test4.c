/*
 * pthread_attr_init(pthread_attr_t *attr)
 * pthread_attr_destroy(pthread_attr_t *attr)
 * typedef struct
 * {
 * 	int etachstate,			// Detach state of thread
 * 	int schedpolicy,		// Schedule strategy of thread
 * 	structsched_param schedparam,	// Schedule parameter
 * 	int inheritsched,		// Inheret property of thread
 * 	int scope,			// Scope of the thread
 * 	size_t guardsize,		// Critical buffer size
 * 	int stackaddr_set,		// Stack options
 * 	void* stackaddr,		// Position of the stack thread
 * 	size_t stacksize		// Size of the stack
 * } pthread_attr_t;
 *
 * PTHREAD_CREATE_JOINABLE uses normal method to set a thread, src must be released
 * by pthread_join(), yet PHTREAD_CREATE_DETACHED will auto release when thread 
 * exit.
 * Use function pthread_attr_getdetachstat(const pthread_attr_t *restrict_attr,
 * 		int *detachstate);
 * Or function pthread_attr_setdetachstat(const pthread_attr_t *attr, 
 * 		int detachstate); // The place "detachstate" sets the state use
 * 		// the macros above.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void out_state(pthread_attr_t *attr)
{
	int state;
	int err;
	err = pthread_attr_getdetachstate(attr, &state);
	if(err)
	{
		perror("Get detach state error!");
	}
	else
	{
		if(state == PTHREAD_CREATE_JOINABLE)
		{
			printf("Joinable state\n");
		}
		else if(state == PTHREAD_CREATE_DETACHED)
		{
			printf("Detached state\n");
		}
		else
		{
			printf("Unknown state!\n");
		}
	}
}

void *th_fn(void *arg)
{
	int i;
	int sum = 0;
	for(i = 1; i <= 100; i++)
	{
		sum += i;
	}
	return (void*)0;
}

int main(void)
{
	int err;
	pthread_t default_th, detach_th;
	// Define thread attr
	pthread_attr_t attr;
	// Init attr
	pthread_attr_init(&attr);
	// Print detach state
	out_state(&attr);

	// Default thread
	err = pthread_create(&default_th, &attr, th_fn, (void*)0);
	if(err)
	{
		perror("pthread create error!");
	}
	int res;
	err = pthread_join(default_th, (void*)&res);
	if(err)
	{
		perror("Thread join error!");
	}
	else
	{
		printf("Default return is %d\n", (int)res);
	}
	printf("--------------------------------------\n");

	// Detached thread
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	out_state(&attr);
	err = pthread_create(&detach_th, &attr, th_fn, (void*)0);
	if(err)
	{
		perror("pthread create error!");
	}

	// Destroy thread attr
	pthread_attr_destroy(&attr);

	printf("0x%lx finished\n", pthread_self());
	sleep(1);
	
	return 0;
}
