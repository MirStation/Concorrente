#ifndef _MONITOR_H
#define _MONITOR_H

#include <stdio.h>
#include <pthread.h>

#include "pqueue.h"
#include "food.h"

void monitor_init(Food capacity);
void monitor_finish();
void get_food_from_pot(Food *f, int tid);
void put_food_in_pot(Food f, int tid);
void set_end(int end2);
void wait(pthread_cond_t *cond);

#endif
