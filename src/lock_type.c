#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int err;
	pthread_mutex_t mutex;

	if(argc < 2)
	{
		fprintf(stderr, "-usage:%s [error|normal|recursive]\n", argv[0]);
		exit(1);
	}
	// Define mutex attr
	pthread_mutexattr_t mutexattr;
	// Init mutex attr
	pthread_mutexattr_init(&mutexattr);

	if(!strcmp(argv[1], "error"))
	{
		pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
	}
	else if(!strcmp(argv[1], "normal"))
	{
		pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);
	}
	else if(!strcmp(argv[1], "recursive"))
	{
		pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
	}
	else
	{
		fprintf(stderr, "Not a type!\n");
		exit(1);
	}

	pthread_mutex_init(&mutex, &mutexattr);
	err = pthread_mutex_lock(&mutex);
	if(err)
	{
		printf("First lock failure!\n");
	}
	else
	{
		printf("First lock success.\n");
	}

	err = pthread_mutex_lock(&mutex);
	if(err)
	{
		printf("Second lock failure!\n");
	}
	else
	{
		printf("Second lock success.\n");
	}

	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex);

	pthread_mutexattr_destroy(&mutexattr);
	pthread_mutex_destroy(&mutex);
}
