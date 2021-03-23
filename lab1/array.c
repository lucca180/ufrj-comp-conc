#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2 
typedef struct {
   int start, end;
} t_Args;

int array[10000];

void* somaArr (void* arg) {
  	t_Args *args = (t_Args *) arg;

	for(int i = args->start; i < args->end; i++)
		array[i] = i+1;

  	free(arg); 

  	pthread_exit(NULL);
}

//funcao principal do programa
int main() {
	pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
	int thread; //variavel auxiliar
	t_Args *arg; //receberá os argumentos para a thread

	// inicializa array	
	for(int i = 0; i < 10000; i++)
		array[i] = i;

	for(thread = 0; thread<NTHREADS; thread++) {
		printf("--Aloca e preenche argumentos para thread %d\n", thread);
		
		arg = malloc(sizeof(t_Args));
		
		if (arg == NULL) {
		printf("--ERRO: malloc()\n"); exit(-1);
		}

		arg->start = 10000 / 2 * thread; 
		arg->end = 10000 / 2 * (thread + 1); 
		
		printf("--Cria a thread %d\n", thread);
		if (pthread_create(&tid_sistema[thread], NULL, somaArr, (void*) arg)) {
		printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}

	//--espera todas as threads terminarem
	for (thread = 0; thread<NTHREADS; thread++) {
		if (pthread_join(tid_sistema[thread], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
	}

	for(int i = 0; i < 10000; i++) {		
		if(array[i] != i+1){
			printf("ERROO!! VC É UM IDIOTA\n");
			break;
		}
	}

	printf("--Thread principal terminou\n");
	pthread_exit(NULL);
}
