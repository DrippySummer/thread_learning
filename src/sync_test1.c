#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
	int result;
	long usleep;
	pthread_cond_t cond;
} Result;

pthread_mutex_t mutex;

void init_res(Result *res, int *err)
{
	res->result = 0;
	res->usleep = 100000;
	*err = pthread_cond_init(&res->cond, NULL);
	if(*err)
	{
		fprintf(stderr, "Err in pthread_cond_init\n");
		exit(1);
	}
}

void* thread_proc1(void *arg)
{
	while(((Result*)arg)[0].result < 30)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&((Result*)arg)[0].cond, &mutex);
		pthread_mutex_unlock(&mutex);
		((Result*)arg)[0].result++;
		printf("Thread1 result: %d\n", ((Result*)arg)[0].result);
		usleep(((Result*)arg)[0].usleep);
		pthread_cond_signal(&((Result*)arg)[1].cond);
	}
	return (void*)0;
}

void* thread_proc2(void *arg)
{
	while(((Result*)arg)[1].result < 30)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&((Result*)arg)[1].cond, &mutex);
		pthread_mutex_unlock(&mutex);
		((Result*)arg)[1].result++;
		printf("Thread2 result: %d\n", ((Result*)arg)[1].result);
		usleep(((Result*)arg)[1].usleep);
		pthread_cond_signal(&((Result*)arg)[0].cond);
	}
	return (void*)0;
}

int main(void)
{
	int err;
	pthread_t thread1, thread2;
	Result res[2];
	init_res(&res[0], &err);
	init_res(&res[1], &err);

	pthread_mutex_init(&mutex, NULL);

	err = pthread_create(&thread1, NULL, thread_proc1, (void*)res);
	if(err)
	{
		fprintf(stderr, "Thread1 create error!\n");
		exit(1);
	}

	err = pthread_create(&thread2, NULL, thread_proc2, (void*)res);
	if(err)
	{
		fprintf(stderr, "Thread2 create error!\n");
		exit(1);
	}

	usleep(10000);
	pthread_cond_broadcast(&res[0].cond);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_cond_destroy(&res[0].cond);
	pthread_cond_destroy(&res[1].cond);
	pthread_mutex_destroy(&mutex);

	return 0;
}
