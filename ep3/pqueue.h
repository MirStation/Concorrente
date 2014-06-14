#ifndef _PQUEUE_H
#define _PQUEUE_H

#include <stdlib.h>

void insert_with_priority(int tid, int priority);
int pull_highest_priority_element();
void clean_queue();

#endif
