#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "food.h"
#include "monitor.h"


#define MAXLINE 80

char weight_option = '\0';
int *weights;
int end_of_eating = 0;
Food c;

void *eater(void *args) {
	Food f;
	int tid = *((int*)args);
	int i;
	if(weight_option == 'U') 
		get_food_from_pot(&f,tid);
	else
		for (i = 0; i < weights[tid]; i++)
			get_food_from_pot(&f,tid);
	return NULL;
}
void *chef(void *args) {
	int tid = *((int*)args);
	while (!end_of_eating) {
		put_food_in_pot(c,tid);
	}
	return NULL;
}

int main (int argc, char *argv[]) {
	FILE *file;
	char *filepath;
	char line[MAXLINE];
	int repetitions = 0;

	int n, m;
	int i, j, retval;

	pthread_t *threads;
	int *thread_args;	

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
	thread_args = malloc(sizeof(*threads) * (n+m));
	assert(thread_args);

	for(j = 0; j < repetitions; j++) {
		monitor_init(c);
		end_of_eating = 0;
		for(i = 0; i < n; ++i) {
			thread_args[i] = i;
			retval = pthread_create(&threads[i],NULL,eater,(void *)&thread_args[i]);
			assert(retval == 0);
		}
		for(; i < n+m; ++i) {
			thread_args[i] = i-n;
			retval = pthread_create(&threads[i],NULL,chef,(void *)&thread_args[i]);
			assert(retval == 0);
		}
		for(i = 0; i < n; ++i) {
			retval = pthread_join(threads[i],NULL);
			assert(retval == 0);
		}
		end_of_eating = 1;
		set_end(end_of_eating);
		for(; i < n+m; ++i) {
			retval = pthread_join(threads[i],NULL);
			assert(retval == 0);
		}
		monitor_finish();
	}
	fclose(file);
	free(weights);
	free(threads);
	free(thread_args);
	return 0;
}

