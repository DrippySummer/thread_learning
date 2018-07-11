#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct
{
	int code;
	double balance;
	// Define a mutex
	pthread_mutex_t mutex;
	// Suggest that every mutex is attached to a shared variable.
	// Do not init mutex in the global field in case of confusion!
} Account;

// Create account
extern Account *create_account(int code, double balance);
// Destroy account
extern void destroy_account(Account *a);
// Withdraw
extern double withdraw(Account *a, double amt);
// Store
extern double deposit(Account *a, double amt);
// Seek balance
extern double get_balance(Account *a);

#endif
