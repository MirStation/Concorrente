#include "pqueue.h"

typedef struct elem_s {
  int tid;
  int priority;
  struct elem_s *next;
} elem;
typedef elem* element;

element pqueue;

element create_element(int tid, int priority, element next) {
	element e;
	e = malloc(sizeof(elem));
	e->tid = tid;
	e->priority = priority;
	e->next = next;
	return e;
}

void insert_with_priority(int tid, int priority) {
	element e;
	/** Lista vazia **/
	if(pqueue == NULL) {
		pqueue = create_element(tid,priority,NULL);
		return;
	}	
	
	/** Primeiro da lista **/
	if(pqueue->priority < priority) {
		pqueue = create_element(tid,priority,pqueue);
		return;
	} else if (pqueue->priority == priority && pqueue->tid == tid){
	  /*As tid presentes na  pqueue são únicas, ou seja, não insere tids repetidos*/
	  return;
	}

	/** No meio da lista **/
	for(e = pqueue; e->next != NULL; e = e->next) {
	  if(e->next->priority < priority) {
	    e->next = create_element(tid,priority,e->next);
	    return;				
	  } else if (e->next->priority == priority && e->next->tid == tid){
	    /*As tid presentes na  pqueue são únicas, ou seja, não insere tids repetidos*/
	    return;
	  }
	}
	
	/** Último da lista **/
	e->next = create_element(tid,priority,NULL);
}

int highest_priority_element() {
  element first = pqueue;
  if(first == NULL){
    return -1;
  }
  return first->tid;
}

void pull_highest_priority_element() {
  element first = pqueue;
  if(first == NULL)
    return ;
  pqueue = first->next;
  first->next = NULL;
  free(first);
}

void print_pqueue(){
  element e = pqueue;
  puts("PQueue elements:");
  for(e=pqueue; e != NULL; e=e->next){
    printf("tid:%d p:%d\n",e->tid,e->priority);
  }
}

void clean_queue_R(element e) {
	if(e == NULL)
		return;
	clean_queue_R(e->next);
	e->next = NULL;
	free(e);
}

void clean_queue() {
	clean_queue_R(pqueue);
}
