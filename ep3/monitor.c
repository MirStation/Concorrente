#include "monitor.h"

Food c;				/* copacidade do pote 	 */
Food pot_food;			/* pote de comida 	 */
pthread_mutex_t mutex;		
pthread_cond_t potfull; 	/* sinaliza se pot_food == C */
pthread_cond_t potempty;	/* sinaliza se pot_food == 0 */

void monitor_init(Food capacity) {
	c = capacity;
	pot_food = 0;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&potfull,NULL);
	pthread_cond_init(&potempty,NULL);
}
void monitor_finish() {
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&potfull);
	pthread_cond_destroy(&potempty);
}
void wait(pthread_cond_t *cond) {
	pthread_cond_wait(cond,&mutex);
}
void signal(pthread_cond_t *cond) {
	pthread_cond_signal(cond);
}
void get_food_from_pot(Food *f, int weight) {
	/*ignorando o peso por enquanto*/
	pthread_mutex_lock(&mutex);
	if(pot_food == 0) { 
		signal(&potempty);
		wait(&potfull);
	}
	*f = 1;
	pot_food -= *f;
	pthread_mutex_unlock(&mutex);
}
void put_food_in_pot(Food f) {
	pthread_mutex_lock(&mutex);
	if(pot_food > 0) { 
		wait(&potempty);
	}
	pot_food = f;
	signal(&potfull);
	pthread_mutex_unlock(&mutex);
}



