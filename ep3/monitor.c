#include "monitor.h"

Food c;				/* copacidade do pote 	 */
Food pot_food;			/* pote de comida 	 */
pthread_mutex_t mutex;		
pthread_cond_t potfull; 	/* sinaliza se pot_food == C */
pthread_cond_t potempty;	/* sinaliza se pot_food == 0 */
int end;

void monitor_init(Food capacity) {
	c = capacity;
	pot_food = 0;
	end = 0;
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
void signal_all(pthread_cond_t *cond) {
	pthread_cond_broadcast(cond);
}
void set_end(int end2) {
	pthread_mutex_lock(&mutex);
	end = end2;
	signal_all(&potempty);
	printf("fim!\n");
	pthread_mutex_unlock(&mutex);
}
void get_food_from_pot(Food *f) {
	/*ignorando o peso por enquanto*/
	pthread_mutex_lock(&mutex);
	while (pot_food == 0) { 
		signal_all(&potempty);
		wait(&potfull);
	}
	*f = 1;
	pot_food -= *f;
	pthread_mutex_unlock(&mutex);
}
void put_food_in_pot(Food f) {
	pthread_mutex_lock(&mutex);
	if(end)	{
		pthread_mutex_unlock(&mutex);
		return;
	}
	while (pot_food > 0) {
		wait(&potempty);
		if(end)	{
			pthread_mutex_unlock(&mutex);
			return;
		}
	}
	pot_food = f;
	signal_all(&potfull);
	pthread_mutex_unlock(&mutex);
}



