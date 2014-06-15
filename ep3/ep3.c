#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "monitor.h"

#define MAXLINE 80

int *weights;
char weight_option;
Food c;

int n, m;
int **thread_args;

/* void insert_with_priority(int tid, int priority); */
/* int highest_priority_element(); */
/* void pull_highest_priority_element(); */

void *savage(void *args) {
  Food f;
  int tid = (((int**)args)[0][0]);
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  while (get_repetitions() >= 0) {
    if(weight_option == 'U'){
      get_food_from_pot(&f,tid,1,&(((int**)args)[0][1]));
    }else{
      get_food_from_pot(&f,tid,weights[tid],&(((int**)args)[0][1]));
    }
    /*Just a delay*/
    tim.tv_nsec = rand()%1000;
    nanosleep(&tim,&tim2);
  } 
  return NULL;
}

void *chef(void *args) {
  int tid = (((int**)args)[0][0]);
  while (get_repetitions() >= 0) {
    put_food_in_pot(c,tid,&(((int**)args)[0][1]));
  }
  return NULL;
}

void print() {
  int i;
  for(i=0;i<n;i++){
    printf("O selvagem %d já comeu %d vezes.\n",thread_args[i][0],thread_args[i][1]);
  }
  for(i=n;i<n+m;i++){
    printf("O cozinheiro %d já encheu o pote %d vezes.\n",thread_args[i][0],thread_args[i][1]);
  }
  for(i=0;i<80;i++)
	printf("-");
  printf("\n");
}


int main (int argc, char *argv[]) {
	FILE *file;
	char *filepath;
	char line[MAXLINE];

	int i, retval, repetitions;

	pthread_t *threads;

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

	thread_args = (int**)malloc(sizeof(int*)*(n+m));
	assert(thread_args);
	for(i=0;i<n+m;i++){
	  thread_args[i] = (int*)malloc(sizeof(int)*2);
	  assert(thread_args[i]);
	}
	
	monitor_init(c,repetitions,m);
	for(i=0;i<80;i++)
	printf("-");
  printf("\n");
	for(i = 0; i < n; ++i) {
	  thread_args[i][0]= i;
	  thread_args[i][1]= 0;
	  retval = pthread_create(&threads[i],NULL,savage,(void *)&thread_args[i]);
	  assert(retval == 0);
	}
	for(; i < n+m; ++i) {
	  thread_args[i][0]= i-n;
	  thread_args[i][1]= 0;
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
