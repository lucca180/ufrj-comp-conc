#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

long long int nSerie;
int nThreads;

void* somaSerie (void* arg){
	long int id = (long int) arg;

	double *somaLocal; //variavel local da soma de elementos
   	somaLocal = (double*) malloc(sizeof(double));
	if(somaLocal==NULL) {exit(1);}

	double serieV; // fracao da serie
	int isSoma = !(id%2); //boleana de somar/subtrair
	long int tamBloco = nSerie/nThreads;
	long int ini = id * tamBloco;
	long int fim; //elemento final(nao processado) do bloco da thread

  	if(id == nThreads-1) fim = nSerie; //trata o resto se houver
   	else fim = ini + tamBloco; 

	for(int i = ini; i < fim; i++){
		serieV = 1.0/(1 + 2 * i);

		if(isSoma) *somaLocal += serieV;
		else *somaLocal -= serieV;
		isSoma = !isSoma;
	}

	pthread_exit((void *) somaLocal);
}

int main(int argc, char *argv[]) {
	double start, finish; //TIMER
	double somaSeq = 0, somaConc = 0, serieV;
	double *retorno;
	int isSoma = 1; //boleana de somar/subtrair
	pthread_t *tid; //identificadores das threads no sistema

	if(argc < 3){
		printf("Digite: %s <elementos da serie> <numero de threads>\n", argv[0]);
		return 1;
	}

	nSerie = atoll(argv[1]);
	nThreads = atoll(argv[2]);


	// Solucao Sequencial
	GET_TIME(start);
	for(int i = 0; i < nSerie; i++){
		serieV = 1 + 2 * i;

		if(isSoma) somaSeq += (1/serieV);
		else somaSeq -= (1/serieV);
		isSoma = !isSoma;
	}

	GET_TIME(finish);
	printf("SEQ: %.3f segundos\n", finish-start);


	// Solucao Concorrente
	GET_TIME(start);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
	if(tid==NULL) {
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}
	//criar as threads
	for(long int i=0; i<nThreads; i++) {
		if(pthread_create(tid+i, NULL, somaSerie, (void*) i)){
			fprintf(stderr, "ERRO--pthread_create\n");
			return 3;
		}
	}

	 //aguardar o termino das threads
   	for(long int i=0; i<nThreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      //soma global
      somaConc += *retorno;
   	}
	GET_TIME(finish);
	printf("CONC: %.3f segundos\n\n", finish-start);

	somaConc = somaConc * 4;
	somaSeq = somaSeq * 4;
	printf("ResultadoSeq: 	%.15f \nDesvioSeq: 	%.15f\n\n", somaSeq, M_PI - somaSeq);
	printf("ResultadoConc:	%.15f \nDesvioConc: 	%.15f\n\n", somaConc, M_PI - somaConc);

	
	free(tid);
	return 0;
}


/*
Os resultados não coincidem devido às questões de arredondamento de numero flutuante... 

Curiosamente a solução sequencial parece ser mais precisa do que a solução concorrente e
se aproxima mais "rapidamente" (embora a concorrentemente seja de fato mais rápido) de Pi.

*/