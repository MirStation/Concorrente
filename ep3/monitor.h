#include <stdio.h>
#include <pthread.h>
#include "food.h"

void monitor_init(Food capacity);
void monitor_finish();
void get_food_from_pot(Food *f, int weight);
void put_food_in_pot(Food f);
void wait(pthread_cond_t *cond);
