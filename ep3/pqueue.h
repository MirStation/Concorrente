#ifndef _PQUEUE_H
#define _PQUEUE_H

#include <stdio.h>
#include <stdlib.h>

void insert_with_priority(int tid, int priority);
int highest_priority_element();
void pull_highest_priority_element();
void clean_queue();
void print_pqueue();

#endif
