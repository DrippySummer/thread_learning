#include "account.h"

typedef struct
{
	char name[36];
	Account *account;
	double amt;
} OperArg;

// Withdraw from the account
void* withdraw_fn(void *arg)
{
	OperArg *oa = (OperArg*)arg;
	double amt = withdraw(oa->account, oa->amt);

	printf("%8s(0x%lx) withdraw %lf from account %d\n", oa->name, pthread_self(),
			amt, oa->account->code);

	return (void*)0;
}

// Store balance into the account
void* deposit_fn(void *arg)
{
	OperArg *oa = (OperArg*)arg;
	double amt = deposit(oa->account, oa->amt);

	printf("%8s(0x%lx) deposit %lf into account %d\n", oa->name, pthread_self(),
			amt, oa->account->code);

	return (void*)0;
}

void* check_fn(void *arg)
{
	return (void*)0;
}

int main(void)
{
	int err;
	pthread_t boy, girl;
	Account *a = create_account(10000, 10000);
	OperArg o1, o2;

	strcpy(o1.name, "boy");
	o1.account = a;
	o1.amt = 10000;

	strcpy(o2.name, "girl");
	o2.account = a;
	o2.amt = 10000;

	// Begin two threads to operate the account simultaneously
	err = pthread_create(&boy, NULL, withdraw_fn, (void*)&o1);
	if(err)
	{
		perror("boy thread create error!\n");
	}
	err = pthread_create(&girl, NULL, withdraw_fn, (void*)&o2);
	if(err)
	{
		perror("girl thread create error!\n");
	}

	// Join the threads
	pthread_join(boy, NULL);
	pthread_join(girl, NULL);

	printf("Account balance: %lf\n", get_balance(a));
	destroy_account(a);
	
	return 0;
}
