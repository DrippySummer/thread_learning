#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

typedef struct
{
	char name[20];
	int time;
	int start;
	int end;
} RaceArg;

void *th_fn(void *arg)
{
	RaceArg *r = (RaceArg*)arg;
	int i;
	for(i = r->start; i <= r->end; i++)
	{
		printf("%s(%lx) running %d\n", r->name, pthread_self(), i);
		usleep((int)(drand48() * r->time));
	}
	return (void*)0;
}

int main(void)
{
	int err;
	pthread_t rabbit, turtle;
	
	RaceArg r_a = 
	{
		"rabbit",
		900000,
		20,
		50
	};
	
	RaceArg t_a = 
	{
		"turtle",
		1200000,
		10,
		50
	};

	err = pthread_create(&rabbit, NULL, th_fn, (void*)&r_a);
	if(err)
	{
		perror("pthread_create error!\n");
	}
	err = pthread_create(&turtle, NULL, th_fn, (void*)&t_a);
	if(err)
	{
		perror("pthread_create error!\n");
	}

	pthread_join(rabbit, NULL);
	pthread_join(turtle, NULL);
	printf("Race complete!\n");
}
