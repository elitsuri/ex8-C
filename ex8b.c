/*
* ex8b.c - Pthreads
*  Created on: Dec 27, 2017
*     Author: eliyho tsuri, Dan Narkisi
*       Login: eliyhots , danna
*		    Id: 201610672, 301680435
*
* The program create 5 pthread, Each Grill natural numbers
* and checks if the first number, Each of the processes
* until Grill gets a prime once received natural number is
* killing other processes and displays the number of play
*/
// ------------------- Include Section -----------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
// ----------------------- Define ----------------------------
#define NUM_THREADS 5
// ---------------------- Prototype --------------------------
void *my_funcs(void *);
unsigned set_number();
bool is_prime(unsigned number);
// -------------------- Global Section -----------------------
pthread_t thread_data[NUM_THREADS];
int arr[NUM_THREADS];
// ------------------------ Main -----------------------------
int main()
{
	int status, index;
	int *number;

	for (index = 0; index < NUM_THREADS; index++)
	{
		status = pthread_create(&thread_data[index],
								NULL,
								my_funcs,
								(void*)&arr[index]);
		if (status != 0)
		{
			fputs("pthread create failed", stderr);
			exit(EXIT_FAILURE);
		}
	}
	for (index = 0; index < NUM_THREADS; index++)
	{
		pthread_join(thread_data[index], (void**)&number);
		printf("I am thread, my thread pid :%d \n", (int)pthread_self());
		printf("Lottery prime number: %d\n", *number);
		free(number);
		pthread_exit(NULL);
	}
	return (EXIT_SUCCESS);
}
// ----------------------- my_funcs --------------------------
/*
* This function srand unsigned numbers and return the number if
* the number is prime number, the function return it to the main
*/
void *my_funcs(void *args)
{
	unsigned *number = (unsigned *)malloc(sizeof(unsigned));
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int *my_id = (int *)args;
	int id;
	bool if_prime;


	while (1)
	{
		*number = set_number();
		if_prime = is_prime(*number);
		if (if_prime)
		{
			for (id = 0; id < NUM_THREADS; id++)
			{
				if (*my_id != id)
					pthread_cancel(thread_data[id]);
			}
			break;
		}
	}
	pthread_exit((unsigned*)number);
}
// ---------------------- set_number -------------------------
/*
* This function srand unsigned numbers and return the number
*/
unsigned set_number()
{
	unsigned number;
	srand((unsigned)time(NULL));
	number = rand() % 100;
	return number;
}
//---------------------- is_prime ----------------------------
/*
* The function takes a number and checks whether it is prime.
* returns true if prime and false if the number is not prime
*/
bool is_prime(unsigned number)
{
	int div;
	if (number == 1)
		return false;
	for (div = 2; div <= number / 2; div++)
		if (number % div == 0)
			return false;
	return true;
}
