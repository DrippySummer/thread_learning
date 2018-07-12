#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
	int value;

	pthread_cond_t rc;
	pthread_mutex_t rm;
	int r_wait;
	
	pthread_cond_t wc;
	pthread_mutex_t wm;
	int w_wait;
} Storage;

void set_data(Storage *s, int value)
{
	s->value = value;
}

int get_data(Storage *s)
{
	return s->value;
}

void* set_th(void *arg)
{
	Storage *s = (Storage*)arg;
	int i;
	for(i = 1; i <= 100; i++)
	{
		set_data(s, i + 100);
		printf("0x%lx write data: %d\n", pthread_self(), i);

		// Wait for reader
		pthread_mutex_lock(&s->rm);
		while(!s->r_wait)
		{
			pthread_mutex_unlock(&s->rm);
			sleep(1);
			pthread_mutex_lock(&s->rm);
		}
		s->r_wait = 0;
		pthread_mutex_unlock(&s->rm);
		pthread_cond_broadcast(&s->rc);

		// Block and then get informed, and write
		pthread_mutex_lock(&s->wm);
		s->w_wait = 1;
		pthread_cond_wait(&s->wc, &s->wm);
		pthread_mutex_unlock(&s->wm);
	}
	return (void*)0;
}

void* get_th(void *arg)
{
	Storage *s = (Storage*)arg;
	int i;
	for(i = 1; i <= 100; i++)
	{
		pthread_mutex_lock(&s->rm);
		s->r_wait = 1;
		pthread_cond_wait(&s->rc, &s->rm);
		pthread_mutex_unlock(&s->rm);
		
		// Read data
		int value = get_data(s);
		printf("0x%lx(%-5d) read data: %d\n", pthread_self(), i, value);
		
		//Judge whether writer is ready
		pthread_mutex_lock(&s->wm);
		while(!s->w_wait)
		{
			pthread_mutex_unlock(&s->wm);
			sleep(1);
			pthread_mutex_lock(&s->wm);
		}

		// Wake writer up
		s->w_wait = 0;
		pthread_mutex_unlock(&s->wm);
		pthread_cond_broadcast(&s->wc);
	}
	return (void*)0;
}

int main(void)
{
	int err;
	pthread_t rth, wth;

	Storage s;
	s.r_wait = 0;
	s.w_wait = 0;
	pthread_mutex_init(&s.rm, NULL);
	pthread_mutex_init(&s.wm, NULL);
	pthread_cond_init(&s.rc, NULL);
	pthread_cond_init(&s.wc, NULL);

	// Create writer and reader thread
	err = pthread_create(&rth, NULL, get_th, (void*)&s);
	if(err)
	{
		fprintf(stderr, "pthread create error!\n");
		exit(1);
	}
	err = pthread_create(&wth, NULL, set_th, (void*)&s);
	if(err)
	{
		fprintf(stderr, "pthread create error!\n");
	}

	pthread_join(rth, NULL);
	pthread_join(wth, NULL);

	pthread_mutex_destroy(&s.rm);
	pthread_mutex_destroy(&s.wm);
	pthread_cond_destroy(&s.rc);
	pthread_cond_destroy(&s.wc);

	return 0;
}
