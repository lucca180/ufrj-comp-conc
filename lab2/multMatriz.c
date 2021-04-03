#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *mat1;
float *mat2;
float *saida;

int dim;

typedef struct tArgs
{
	int start;
	int step;
} tArgs;

void* multiArr (void* arg) {
  	tArgs *args = (tArgs *) arg;

	printf("--Thread começando na linha %d com step %d\n", args->start, args->step);

	for(int i = args->start; i < dim; i+= args->step){
		for(int j = 0; j < dim; j++){
			for (int k = 0; k < dim; k++)
				saida[i*dim + j] += mat1[i*dim + k] * mat2[k*dim + j];
		}
	}

  	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	double start, finish, elapsed, total = 0; //TIMER

	int thread, NTHREADS;
	tArgs *arg;

	if(argc < 3){
		printf("Digite: %s <dimenção da matriz> <n de threads>\n", argv[0]);
		return 1;
	}

	dim = atoi(argv[1]);
	NTHREADS = atoi(argv[2]);
	pthread_t tid_sistema[NTHREADS];

	// alocação de memoria
	mat1 = (float *) malloc(sizeof(float) * dim * dim);
	mat2 = (float *) malloc(sizeof(float) * dim * dim);
	saida = (float *) malloc(sizeof(float) * dim * dim);

	if(mat1 == NULL || mat2 == NULL || saida == NULL){
		printf("ERRO MALOC\n");
		return 2;
	}

	// inicialização das matrizes
	GET_TIME(start);
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++){
			mat1[i*dim + j] = 1;
			mat2[i*dim + j] = 1;
			saida[i*dim + j] = 0;
		}																																
	}
	GET_TIME(finish);
	elapsed = finish - start;
	total += elapsed;
 	printf("INICIALIZAÇÃO: %.3f segundos\n", elapsed);

	// multiplicacao das matrizes
	GET_TIME(start);
	for(thread = 0; thread<NTHREADS; thread++) {
		//printf("--Aloca e preenche argumentos para thread %d\n", thread);
		
		arg = malloc(sizeof(tArgs));
		
		if (arg == NULL) {
		printf("--ERRO: malloc()\n"); exit(-1);
		}

		arg->start = thread;
		arg->step = NTHREADS;
		
		printf("--Cria a thread %d\n", thread);
		if (pthread_create(&tid_sistema[thread], NULL, multiArr, (void*) arg)) {
		printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}

	for (thread = 0; thread<NTHREADS; thread++) {
		if (pthread_join(tid_sistema[thread], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
	}

	GET_TIME(finish);
	elapsed = finish - start;
	total += elapsed;
 	printf("MULTIPLICAÇÃO: %.3f segundos\n", elapsed);

	// verificação do resultado
	GET_TIME(start);
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++){
			if(saida[i*dim + j] != dim){
				printf("ERRADO -> LINHA: %d COLUNA: %d", i, j);
				return 1;
			}
			
			//printf("%.1f ", saida[i*dim + j]);
		}
		//printf("\n");
	}

	free(mat1);
	free(mat2);
	free(saida);

	GET_TIME(finish);
	elapsed = finish - start;
	total += elapsed;
 	printf("FINALIZAÇÃO: %.3f segundos\n", elapsed);
	printf("TOTAL: %.3f segundos\n", total);
	return 0;
}