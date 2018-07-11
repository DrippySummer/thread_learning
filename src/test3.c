#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void clean_fun(void *arg)
{
	char *s = (char*)arg;
	printf("clean_func: %s\n", s);
}

void *th_fn(void *arg)
{
	/*
	 * arg 0 do not execute the cleanup function.
	 * arg 1 execute the cleanup function
	 * */
	int execute = (int)arg;
	pthread_cleanup_push(clean_fun, "first clean func");
	pthread_cleanup_push(clean_fun, "second clean func");
	printf("thread running %lx\n", pthread_self());
	pthread_cleanup_pop(execute);
	pthread_cleanup_pop(execute);

	return (void*)0;
}

int main(void)
{
	int err;
	pthread_t th1, th2;
	err = pthread_create(&th1, NULL, th_fn, (void*)1);
	if(err)
	{
		perror("pthread create error");
	}
	pthread_join(th1, NULL);
	printf("th1(%lx) finished\n", th1);

	err = pthread_create(&th2, NULL, th_fn, (void*)1);
	if(err)
	{
		perror("pthread create error");
	}
	pthread_join(th2, NULL);
	printf("th2(%lx) finished\n", th2);

	return 0;
}
