#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "monitor.h"

#define MAXLINE 80

int *weights;
Food c;

void *savage(void *args) {
  Food f;
  int tid = *((int*)args);
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  while (get_repetitions() >= 0) {
    get_food_from_pot(&f,tid,0);
    /*Just a delay*/
    tim.tv_nsec = rand()%1000;
    nanosleep(&tim,&tim2);
  } 
  return NULL;
}

void *chef(void *args) {
  int tid = *((int*)args);
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  while (get_repetitions() >= 0) {
    put_food_in_pot(c,tid);
    /*Just a delay*/
    tim.tv_nsec = rand()%1000;
    nanosleep(&tim,&tim2);
  }
  return NULL;
}

int main (int argc, char *argv[]) {
	FILE *file;
	char *filepath;
	char line[MAXLINE];

	int n, m;
	int i, retval, repetitions;

	pthread_t *threads;
	int *thread_args;

	char weight_option;

	assert(argc == 4);

	filepath = argv[1];
	file = fopen(filepath,"r");
	assert(file);

	repetitions = atoi(argv[2]);
	assert(repetitions > 0);

	weight_option = argv[3][0];
	assert(strlen(argv[3]) == 1);
	assert(weight_option == 'U' || weight_option == 'P');

	fgets(line,MAXLINE,file);
	n = atoi(line);

	weights = malloc(sizeof(*weights) * n);
	assert(weights);
	for(i = 0; i < n; ++i) {
		fscanf(file,"%d",&weights[i]);
	}
	fgets(line,MAXLINE,file); /*Pega o \n da linha acima*/
	fgets(line,MAXLINE,file);
	c = atoi(line);
	fgets(line,MAXLINE,file);
	m = atoi(line);

	threads = malloc(sizeof(*threads) * (n+m));
	assert(threads);

	thread_args = (int*)malloc(sizeof(int)*(n+m));
	assert(thread_args);
	
	monitor_init(c,weight_option,repetitions);
	for(i = 0; i < n; ++i) {
	  thread_args[i]= i;
	  retval = pthread_create(&threads[i],NULL,savage,(void *)&thread_args[i]);
	  assert(retval == 0);
	}
	for(; i < n+m; ++i) {
	  thread_args[i] = i;
	  retval = pthread_create(&threads[i],NULL,chef,(void *)&thread_args[i]);
	  assert(retval == 0);
	}
	/*Waiting for the end of all the chefs*/
	for(i = n; i < n+m; ++i) {
	  retval = pthread_join(threads[i],NULL);
	  assert(retval == 0);
	}
	/*Waiting for the end of all the savages*/
	for(i = 0; i < n; ++i) {
	  retval = pthread_join(threads[i],NULL);
	  assert(retval == 0);
	}
	monitor_finish();
	fclose(file);
	free(weights);
	free(threads);
	free(thread_args);
	return 0;
}
