/*
 *	EX: 8.
 *	FILE:ex8a.C
 *	CREATED BY: DAN NARKISSI	ID:301680435	USER:danna.
 *							ELI TSURI 		ID:201610672	USER:eliyhots.
 *
 *								# # 6 THREADS PROGRAM # #
 *	DESCRIOPTION: this program creates 3 threads that lottery numbers between
 *	-10 to 10 , then she puts them in the array.
 *	and calculate how many times she add a new number or erased a number.
 *	then the program create 3 other threads.
 *	the threads lottery unsigned number (1 - 10 ) and search for the number
 *	in the array.
 *	finnaly the main program will print the result.
 *
 */

//-------------------------INCLUDES--------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

//-------------------------CONSTANTS ------------------------------------------
#define SIZE 100
#define THREAD_NUM 6
#define LOTTERY 1000

//-------------------------CONSTANTS ------------------------------------------
struct thread_data
{
	int add_counter;
	int erased_counter;
};

//------------------------- GLOBAL ------------------------------------------
int arr[SIZE];

//-------------------------CONSTANTS ------------------------------------------
void array_reset();
void thread_update_create (pthread_t update[]);
void thread_search_create (pthread_t search[]);
void * search_mission (void *arg );
bool check_if_exist(int random_num);
void* update_mission(void* arg);
void erase_num(int num);
void add_num (int num);
//-------------------------MAIN ------------------------------------------
int main ()
{
	pthread_t threads[THREAD_NUM];

	array_reset();

	thread_update_create (threads);
	thread_search_create (threads);


	return EXIT_SUCCESS;


}


//-------------------------FUNCTIONS ------------------------------------------
// array reset = will reset the global scturct to zero.
void array_reset()
{
	for(int i = 0 ; i<SIZE ; i++ )
		arr[i] = 0;
}

//----------------------------------------------------------------------------f
//thread update create = create the 3 threads for the array update.
void thread_update_create (pthread_t threads[])
{
	srand((unsigned)time(NULL));

	int i;
	int status;
	struct thread_data data[3];

	for(i = 0 ; i < THREAD_NUM/2 ; i++)// this loop will create the thread.
	{
		status = pthread_create(threads + i,NULL,update_mission,
				(void*) &data[i]);
		if (status != 0 )
		{
			puts("cannot create thread\n");
			exit(EXIT_FAILURE);
		}
	}
	//wait for the threads and print the result.
	for (i = 0 ; i < THREAD_NUM/2 ; i++) {

		pthread_join(threads[i], NULL);

		printf("thread: %d add : %d erase: %d\n",(int)threads[i],
				data[i].add_counter,data[i].erased_counter);
	}
}

//----------------------------------------------------------------------------f
//thread search create = create the 3 threads for the search mission.
void thread_search_create (pthread_t threads[])
{
	int i;
	int found[3]={0,0,0};

	int status;

	for (i = 3 ; i < THREAD_NUM;i++)//create the search threads
	{
		status = pthread_create(threads + i ,NULL,search_mission,(void *)
				&found[i % 3]);
		if (status != 0 )
		{
			puts("cannot create thread\n");
			exit(1);
		}
	}

	for (i = 3 ; i < THREAD_NUM ; i++)//wait for search
		//and will print the results.
	{
		pthread_join(threads[i], NULL);
		printf ("thread: %d found: %d\n",(int)threads[i], found[i%3]);

	}

}

//----------------------------------------------------------------------------f
//search mission = define the search commands.
void * search_mission (void *arg )
{
    int *counter = (int *)arg;
    int i;
    int random_num;

    for (i = 0 ; i < LOTTERY ; i++)
    {
        random_num = rand() % 11;
        if (check_if_exist(random_num))
            (*counter)++;
    }
    pthread_exit(NULL);
}

//----------------------------------------------------------------------------f
// check if exist = check if the number is in the array.
bool check_if_exist(int random_num)
{
	int i;
	for (i= 0 ; i < SIZE ; i++)
	{
		if (arr[i] == random_num)
			return true;
		if (arr[i] == 0)
			continue;
	}
	return false;
}

//----------------------------------------------------------------------------f
//update mission = will define the update array commands.
void* update_mission(void* arg)
{
	int random_num;
	int index;
	struct thread_data* data = (struct thread_data*)arg;

	data->add_counter = 0;
	data->erased_counter = 0;


	for (index = 0 ; index < LOTTERY ; index++)
	{
		random_num = (rand()%21) - 10;

		if (random_num < 0)
		{
			random_num = -random_num;
			erase_num(random_num);
			data->erased_counter++;
		}

		else if (random_num > 0)
		{
			add_num(random_num);
			data->add_counter++;
		}
	}

	pthread_exit(NULL);
}

//----------------------------------------------------------------------------f
//erase num = will remove a number from the array.
void erase_num(int num)
{
	int index;

	for (index = 0 ; index < SIZE ; index++)
	{
		if (arr[index] == num)
		{
			arr[index] = 0;
			break;
		}
	}
}
//----------------------------------------------------------------------------f
//add num = will add a new number to the array.
void add_num (int num)
{
	int index;

	for (index = 0 ; index < SIZE ; index++)
	{
		if (arr[index] == 0)
		{
			arr[index] = num;
			break;
		}
	}
}
