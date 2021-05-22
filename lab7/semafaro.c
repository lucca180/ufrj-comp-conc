#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

// Variaveis globais
sem_t condt2, condt3, condt4;     //semaforos para sincronizar a ordem de execucao das threads

//funcao executada pela thread 1
void *t1 (void *threadid) {
	sem_wait(&condt4); //espera T1 executar
	sem_wait(&condt3); //espera T1 executar
  	printf("Volte Sempre!\n");
  	pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2 (void *threadid) {
  	sem_wait(&condt2); //espera T1 executar
  	printf("Fique a vontade.\n");
	sem_post(&condt2); //permite que T2/T3 execute
  	sem_post(&condt3); // Libera 1 Semaforo da T4

  	pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3 (void *threadid) {
  	sem_wait(&condt2); //espera T2 executar
    printf("Sente-se por favor.\n");
	sem_post(&condt2); //permite que T2/T3 execute
	sem_post(&condt4); // Libera 1 Semaforo da T4

  	pthread_exit(NULL);
}

void *t4 (void *threadid) {
	printf("Seja bem-vindo!\n");
	sem_post(&condt2); //permite que T2/T3 execute

	pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
	pthread_t tid[NTHREADS];
	int *id[4], t;

	for (t=0; t<NTHREADS; t++) {
		if ((id[t] = malloc(sizeof(int))) == NULL) {
		pthread_exit(NULL); return 1;
		}
		*id[t] = t+1;
	}

	//inicia os semaforos
	sem_init(&condt2, 0, 0);
	sem_init(&condt3, 0, 0);
	sem_init(&condt4, 0, 0);

	//cria as tres threads
    if (pthread_create(&tid[3], NULL, t4, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  	if (pthread_create(&tid[1], NULL, t2, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  	if (pthread_create(&tid[2], NULL, t3, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  	if (pthread_create(&tid[0], NULL, t1, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }

	//--espera todas as threads terminarem
	for (t=0; t<NTHREADS; t++) {
		if (pthread_join(tid[t], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
		free(id[t]);
	} 

  	pthread_exit(NULL);
}