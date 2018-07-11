#include "account.h"

Account* create_account(int code, double balance)
{
	Account *account_ptr = (Account*)malloc(sizeof(Account));
	assert(account_ptr != NULL);
	account_ptr->code = code;
	account_ptr->balance = balance;

	// Init mutex
	pthread_mutex_init(&account_ptr->mutex, NULL);
	return account_ptr;
}

void destroy_account(Account *account_ptr)
{
	assert(account_ptr != NULL);
	// Free mutex
	pthread_mutex_destroy(&account_ptr->mutex);
	free(account_ptr);
}

double withdraw(Account *account_ptr, double amt)
{
	assert(account_ptr != NULL);
	// Lock
	pthread_mutex_lock(&account_ptr->mutex);
	if(amt < 0 || amt > account_ptr->balance)
	{
		// Release lock
		pthread_mutex_unlock(&account_ptr->mutex);
		return 0.0;
	}
	double balance = account_ptr->balance;
	sleep(1);
	balance -= amt;
	account_ptr->balance = balance;
	// Unlock
	pthread_mutex_unlock(&account_ptr->mutex);
	return amt;
}

double deposit(Account *account_ptr, double amt)
{
	assert(account_ptr != NULL);
	pthread_mutex_lock(&account_ptr->mutex);
	if(amt < 0)
	{
		pthread_mutex_unlock(&account_ptr->mutex);
		return 0.0;
	}
	double balance = account_ptr->balance;
	sleep(1);
	balance += amt;
	account_ptr->balance = balance;
	pthread_mutex_unlock(&account_ptr->mutex);
	return amt;
}

double get_balance(Account *account_ptr)
{
	assert(account_ptr != NULL);
	pthread_mutex_lock(&account_ptr->mutex);
	double balance = account_ptr->balance;
	pthread_mutex_unlock(&account_ptr->mutex);
	return balance;
}
