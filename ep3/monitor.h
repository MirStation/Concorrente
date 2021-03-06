#ifndef _MONITOR_H
#define _MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#include "pqueue.h"
#include "food.h"
#include "ep3.h"
#include "uniformDistribution.h"

void monitor_init(Food capacity, int r, int n);
void monitor_finish();
void get_food_from_pot(Food *f, int tid, int weight, int* e);
int get_repetitions();
void put_food_in_pot(Food f, int tid, int * c);

#endif
