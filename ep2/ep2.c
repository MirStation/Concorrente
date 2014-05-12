#include <pthread.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <ctype.h>

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

/*MAIN*/
int main(int argc, char** argv) {
  /*Keeps the number of threads to be used in e's calculation*/
  int num_threads;
  
  /*Variables used in the determiantion of the stop condition of e's calculation*/
  char stop_setup;
  double stop_value;

  /*Variables used to setup the output of the program*/
  char output_setup = '\0'; /*The default outup of the program is indicated by the char \0*/
  
  /*Input processing*/
  if (argc >= 4) {
    /*Processing the 1th argument*/
    if (is_natural(argv[1]) > -1){
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
    if (is_preal(argv[3]) > -1){
      stop_value = atof(argv[3]);
    } else {
      printf("%s is not a valid positive real number!\n", argv[3]);
      exit(1);
    }
    /*Processing the 4th argument (optinal)*/
    if (argc == 5) {
      if (argv[4][0] == 'd' || argv[4][0] == 's') {
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
  
  /*DEBUG: checking input values*/
  printf("DEBUG -> num_threads: %d\nstop_type:%c\nstop_value:%f\noutput_setup:%s\n",num_threads,stop_setup,stop_value,(output_setup == '\0' ? "default" : (output_setup == 'd' ? "d":"s")));

  if (num_threads == 0) {
    /*Get the number of cpu cores in a linux OS*/
    num_threads = sysconf(_SC_NPROCESSORS_CONF);
  }

  /*DEBUG: num_threads must be equal to the number of cpu cores in this (linux) machine*/
  printf("DEBUG -> num_threads: %d\ncpu_cores:%ld\n",num_threads, sysconf(_SC_NPROCESSORS_CONF));

  return 0;
}
