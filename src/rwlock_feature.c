#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define read-write lock
pthread_rwlock_t rwlock;

int main(int argc, char *argv[])
{
	int err;

	if(argc < 3)
	{
		printf("-usage:%s [r|w] [r|w]\n", argv[0]);
		exit(1);
	}
	// rwlock init
	pthread_rwlock_init(&rwlock, NULL);
	if(!strcmp("r", argv[1]))
	{
		// Add read lock
		err = pthread_rwlock_rdlock(&rwlock);
		if(err)
		{
			printf("First read lock failure!\n");
		}
		else
		{
			printf("First read lock success.\n");
		}
	}
	else if(!strcmp("w", argv[1]))
	{
		// Add write lock
		err = pthread_rwlock_wrlock(&rwlock);
		if(err)
		{
			printf("First write lock failure!\n");
		}
		else
		{
			printf("First write lock success.\n");
		}
	}

	if(!strcmp("r", argv[2]))
	{
		// Add read lock
		err = pthread_rwlock_rdlock(&rwlock);
		if(err)
		{
			printf("Second read lock failure!\n");
		}
		else
		{
			printf("Second read lock success.\n");
		}
	}
	else if(!strcmp("w", argv[2]))
	{
		// Add write lock
		err = pthread_rwlock_wrlock(&rwlock);
		if(err)
		{
			printf("Second write lock failure!\n");
		}
		else
		{
			printf("Second write lock success.\n");
		}
	}

	return 0;
}
