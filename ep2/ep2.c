#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include "gmp.h"

/*
 * VARIABLES AND CONSTANTS
 */

/*Constants used in the semaphores*/
#define SHARED 1
#define LOCAL 0
/*Keeps the number of threads to be used in e's calculation*/
int num_threads;
/*Variables used in the determiantion of the stop condition of e's calculation*/
int stop = 0;
int stop_all = 0;
char stop_setup;
mpf_t stop_value;
/*Variables used in the computation of e*/
mpf_t e;
mpf_t *factorials;
/*Producer and Consumer variables*/
int front=0, rear=0;
sem_t empty, full, mutexF, mutex;
/*Barrier arrays*/
int *arrive;
int *go;
int *arrive_order;
int order = 0;
/*Iterations*/
long iter = 0;
/*Terms*/
long k = 1;
/*Variables used to setup the output of the program*/
char output_setup = '\0'; /*The default outup of the program is indicated by the char \0*/

/*
 * UTILITIES
 */

/*Function that checks if a string is a valid natural number. 
  If 'str' is a valid natural number (including zero) the function 
  returns the value in 'natural', otherwise the it returns -1.
  
  OBS: If 'str' is a valid real number then 'natural' will be the truncation 
  of 'str' without the decimals.*/
int is_natural(char* str) {
  int natural = atoi(str);
  if (natural == 0 && str[0] != '0') {
    /*Not a natural number*/
    return -1;
  }
  /*Checks if natural is positive before returning.*/
  return natural >= 0 ? natural : -1;
}

/*Function that checks if a string is a valid positive real number. 
  If 'str' is a valid POSITIVE REAL number (including zero) the function 
  returns the value in 'preal', otherwise the it returns -1.*/
int is_preal(char* str) {
  int preal = atof(str);
  if (preal == 0 && str[0] != '0') {
    /*Not a real number*/
    return -1;
  }
  /*Checks if preal is a positive real number before returning.*/
  return preal >= 0 ? preal : -1;
}

/*
 * MULTITHREAD
 */

/* Computation of the constant e for multithreads  */
void* compute_e_multi_t (void* argument) {
  int tid;
  /*int evalue, fvalue;*/
  mpf_t diff;
  mpf_t past_e;
  struct timespec tim, tim2;
  tid = *((int *)argument);
  mpf_init(diff);
  mpf_init(past_e);
  mpf_set(past_e, e);
  tim.tv_sec = 0;
  while (!stop_all) {
    /*sem_getvalue(&full,&fvalue);
    printf("fvalue:%d\n",fvalue);
    sem_getvalue(&empty,&evalue);
    printf("evalue:%d\n",evalue);
    printf("1ESC tid:%d\n",tid);*/
    sem_wait(&full);
    /*sem_getvalue(&full,&fvalue);
    printf("fvalue:%d\n",fvalue);
    sem_getvalue(&empty,&evalue);
    printf("evalue:%d\n",evalue);
    printf("2ESC tid:%d\n",tid);*/
    sem_wait(&mutexF);
    /*sem_getvalue(&full,&fvalue);
    printf("fvalue:%d\n",fvalue);
    sem_getvalue(&empty,&evalue);
    printf("evalue:%d\n",evalue);
    printf("3ESC tid:%d\n",tid);*/
    mpf_add(e, e, factorials[front]);
    if (stop_setup == 'f') {
      mpf_sub(diff,e,past_e);
      if (mpf_cmp(diff,stop_value) < 0){
	stop = 1;
      }
      mpf_set(past_e, e);
    } else {
      if (mpf_cmp(factorials[front],stop_value) < 0) {
	stop = 1;
      }
    }
    front= (front+1)%(num_threads-1);
    /*printf("front:%d\n",front);
    printf("SSC tid:%d\n",tid);*/
    sem_post(&mutexF);
    sem_post(&empty);
    /*sem_getvalue(&full,&fvalue);
    printf("fvalue:%d\n",fvalue);
    sem_getvalue(&empty,&evalue);
    printf("evalue:%d\n",evalue);*/
    sem_wait(&mutex);
    arrive[tid] = 1;
    arrive_order[order++] = tid;
    if (order == (num_threads-1)) order = 0;
    sem_post(&mutex);
    while (go[tid] == 0) {
      tim.tv_nsec = rand()%1000;
      nanosleep(&tim,&tim2);
    }
    go[tid] = 0;
  }
  mpf_clear(diff);
  mpf_clear(past_e);
  return NULL;
}
/* Function executed by the thread observer */
void* observer() {
  int i;
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  while (!stop_all) {
    for (i=0;i<(num_threads-1);i++) {
      while (arrive[i] == 0) {
	tim.tv_nsec = rand()%1000;
	nanosleep(&tim,&tim2);
      }
      arrive[i] = 0;
    }
    iter++;
    if (output_setup == 'd') {
      printf("Iteration: %ld\n", iter);
      printf("-> Arrived order at the barrier:\n");
      for (i=0;i<(num_threads-1);i++) {
	printf("%d ",arrive_order[i]);
      }
      putchar('\n');
      printf("-> Partial value of e:\n");
      mpf_out_str(NULL,10,0,e);
      putchar('\n');
    }
    if(stop){
      stop_all = 1;
    }
    for (i=0;i<(num_threads-1);i++) {
      go[i] = 1;
    }
  }
  for(i=0;i<(num_threads-1);i++){ /*is this necessary?*/
    go[i] = 1;
  }
  return NULL;
}

/* Computation of factorials */
void* compute_factorials () {
  mpf_t uk;
  mpf_init(uk);
  mpf_set_d(uk,1);
  while(1){
    mpf_div_ui(uk,uk,k);
    sem_wait(&empty);
    mpf_set(factorials[rear],uk);
    rear = (rear+1)%(num_threads-1);
    sem_post(&full);
    k++;
    if ((k-1)%(num_threads-1) == 0) { /*New stop condition - solves the loop bug - tested for 5000 calculations of e and 30 threads*/
      if (stop) break;
    }
  }
  mpf_clear(uk);
  return NULL;
}

/*
 * SINGLE THREAD
 */

/* Computation of the constant e for one thread */
void* compute_e_single_t (void* argument) {
  int tid, i;
  mpf_t past_e;
  mpf_t diff;
  mpf_t uk;
  tid = *((int *)argument);
  mpf_init(uk);
  mpf_set_d(uk,1);
  mpf_init(past_e);
  mpf_init(diff);
  mpf_set(past_e, e);
  while (1) {
    mpf_div_ui(uk,uk,k);
    mpf_add(e, e, uk);
    if (stop_setup == 'f') {
      mpf_sub(diff,e,past_e);
      if (mpf_cmp(diff,stop_value) < 0){
	break;
      }
      mpf_set(past_e, e);
    } else {
      if (mpf_cmp(uk,stop_value) < 0) {
	break;
      }
    }
    k++;
    if (output_setup == 's') {
      printf("Partial value of e:\n");
      mpf_out_str(NULL,10,0,e);
      putchar('\n');
    } else if (output_setup == 'd') {
      printf("Iteration: %ld\n", k);
      printf("-> Arrived order at the barrier:\n");
      for (i=0;i<num_threads;i++) {
	printf("%d ",tid);
      }
      putchar('\n');
      printf("-> Partial value of e:\n");
      mpf_out_str(NULL,10,0,e);
      putchar('\n');
    }
  }
  mpf_clear(uk);
  mpf_clear(past_e);
  mpf_clear(diff);
  return NULL;
}

/*
 * MAIN
 */

int main(int argc, char** argv) {
  pthread_t *threads;
  int *threads_args;
  int i, rc;
  struct timespec tstart={0,0}, tend={0,0};
  double timelapse;

  mpf_init(e);  /* Inicialization of e*/
  mpf_init(stop_value); /* Inicialization of stop_value*/
  mpf_set_d(e,1); /* Setting e with with one*/

  /* timing */
  clock_gettime(CLOCK_MONOTONIC, &tstart);

  /*Input processing*/
  if (argc >= 4) {
    /*Processing the 1th argument*/
    if (is_natural(argv[1]) > -1) {
      num_threads = atoi(argv[1]);
    } else {
      printf("%s is not a valid natural number!\n", argv[1]);
      exit(1);
    }
    /*Processing the 2th argument*/
    if (argv[2][0] == 'm' || argv[2][0] == 'f') {
      stop_setup = argv[2][0];
    } else {
      printf("%c is an invalid stop condition setup!\n", argv[2][0]);
      exit(1);
    }
    /*Processing the 3th argument*/
    if (is_preal(argv[3]) > -1) {
      mpf_set_str(stop_value, argv[3], 10);
    } else {
      printf("%s is not a valid positive real number!\n", argv[3]);
      exit(1);
    }
    /*Processing the 4th argument (optinal)*/
    if (argc == 5) {
      if (argv[4][0] == 'd' || argv[4][0] == 's' || argv[4][0] == 't') {
	output_setup = argv[4][0]; 
      } else {
	printf("%c is an invalid output setup!Output setup maintained default!\n", argv[4][0]);
      }
    } else if (argc > 5) {
      printf("Too many arguments!\n");
      exit(1);
    }
  } else {
    printf("Too few arguments!\n");
    exit(1);
  }
  if (num_threads == 0) {
    /*Get the number of cpu cores in a linux OS*/
    num_threads = sysconf(_SC_NPROCESSORS_CONF);
  }
  
  if (num_threads > 1 && output_setup != 's') {
    threads = (pthread_t *)malloc((num_threads + 1)*sizeof(pthread_t));
    assert(threads);
    threads_args = (int *)malloc((num_threads - 1)*sizeof(int));
    assert(threads_args);
    /*Multithread settings*/
    srand(time(NULL));
    arrive = (int *)calloc(num_threads-1,sizeof(int));
    assert(arrive);
    go = (int *)calloc(num_threads-1,sizeof(int));
    assert(go);
    arrive_order = (int *)malloc((num_threads-1)*sizeof(int));
    assert(arrive_order);
    factorials = (mpf_t *)malloc((num_threads-1)*sizeof(mpf_t));
    assert(factorials);
    for(i=0;i<(num_threads-1);i++) {
      mpf_init(factorials[i]);
    }
    rc = sem_init(&empty,LOCAL,num_threads-1);
    assert(rc == 0);
    rc = sem_init(&full,LOCAL,0);
    assert(rc == 0);
    rc = sem_init(&mutexF,LOCAL,1);
    assert(rc == 0);
    rc = sem_init(&mutex,LOCAL,1);
    assert(rc == 0);
    for(i=0;i<(num_threads-1);i++) {
      threads_args[i]=i;
      rc = pthread_create(&threads[i],NULL,compute_e_multi_t,(void*)&threads_args[i]);
      assert(rc == 0);
    }
    rc = pthread_create(&threads[i],NULL,compute_factorials,NULL);
    assert(rc == 0);
    i++;
    rc = pthread_create(&threads[i],NULL,observer,NULL);
    assert(rc == 0);
    for(i=0;i<(num_threads+1);i++) {
      rc = pthread_join(threads[i],NULL);
      assert(rc == 0);
    }
    rc = sem_destroy(&empty);
    assert(rc == 0);
    rc = sem_destroy(&full);
    assert(rc == 0);
    rc = sem_destroy(&mutexF);
    assert(rc == 0);
    rc = sem_destroy(&mutex);
    assert(rc == 0);
    for(i=0;i<(num_threads-1);i++) {
      mpf_clear(factorials[i]);
    }
    free(factorials);
    free(arrive);
    free(go);
    free(arrive_order);
  } else {
    /*Singlethread settings*/
    threads = (pthread_t *)malloc(num_threads*sizeof(pthread_t));
    assert(threads);
    threads_args = (int *)malloc(num_threads*sizeof(int));
    assert(threads_args);
    threads_args[0] = 0;
    rc = pthread_create(&threads[0],NULL,compute_e_single_t,(void*)&threads_args[0]);
    assert(rc == 0);
    rc = pthread_join(threads[0],NULL);
    assert(rc == 0);
    
  }
  if (output_setup != 't') {
    if (output_setup == 's') {
      printf("Terms found: %ld\n",k);
    } else {
      printf("Total iterations: %ld\n",num_threads > 1 ? iter : k);
    }
    printf("Value of e:\n");
    mpf_out_str(stdout,10,0,e);
    putchar('\n');
  } else {
    clock_gettime(CLOCK_MONOTONIC, &tend);
    timelapse = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
    printf("%.6f\n",timelapse * 1000);
  }
  free(threads);
  free(threads_args);
  mpf_clear(e);
  mpf_clear(stop_value);
  return 0;
}
