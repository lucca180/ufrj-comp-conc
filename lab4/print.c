#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void *Thread1 (void *t) {
	while(x != 3){}
	
	printf("Volte Sempre!\n");

	pthread_exit(NULL);
}
void *Thread2 (void *t) {
	pthread_mutex_lock(&x_mutex);
	
	if(x == 0) pthread_cond_wait(&x_cond, &x_mutex);
	
	printf("Fique a vontade.\n");
	x++;
	
	pthread_mutex_unlock(&x_mutex);

	pthread_exit(NULL);
}

void *Thread3 (void *t) {
	pthread_mutex_lock(&x_mutex);
	
	if(x == 0) pthread_cond_wait(&x_cond, &x_mutex);
	
	printf("Sente-se por favor.\n");
	x++;
	
	pthread_mutex_unlock(&x_mutex);

	pthread_exit(NULL);
}

void *Thread4 (void *t) {
	pthread_mutex_lock(&x_mutex);
	
	printf("Seja Bem-Vindo.\n");
	x++;
	
	pthread_cond_broadcast(&x_cond);
	
	pthread_mutex_unlock(&x_mutex);

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[4];
  //aloca espaco para os identificadores das threads

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, Thread1, NULL);
  pthread_create(&threads[1], NULL, Thread2, NULL);
  pthread_create(&threads[2], NULL, Thread3, NULL);
  pthread_create(&threads[3], NULL, Thread4, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("FIM.\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}