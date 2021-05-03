#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int nThreads;
int* vector;
int bloqueadasW = 0;
int bloqueadasR = 0;
pthread_mutex_t x_mutex;
pthread_cond_t cond_leit, cond_escr;

//Barreira de Leitura
void barreiraR() {
    pthread_mutex_lock(&x_mutex); //inicio secao critica

	bloqueadasR++;
	if(bloqueadasR != nThreads){
      	pthread_cond_wait(&cond_leit, &x_mutex);
	}

	pthread_cond_broadcast(&cond_leit);
    bloqueadasR=0;

    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

// Barreira de Escrita
void barreiraW() {
    pthread_mutex_lock(&x_mutex); //inicio secao critica

	bloqueadasW++;
	if(bloqueadasW != nThreads){
      	pthread_cond_wait(&cond_escr, &x_mutex);
	}

	pthread_cond_broadcast(&cond_escr);
    bloqueadasW = 0;

    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

void* soma (void* arg){
  	long int id = (long int) arg;
	int step, aux, pos;
	
	for(int i = 0; i < nThreads; i++){
		step = pow(2, i);

		pos = id - step;

		if(pos < 0) {
			barreiraR();
			barreiraW();
			continue;
		}

		aux = vector[pos];

		barreiraR();

		vector[id] += aux;
		barreiraW();
	}

	pthread_exit(NULL);
}	

int main(int argc, char *argv[]) {
	//Variáveis
	int p;
	pthread_t *tid;
	pthread_mutex_init(&x_mutex, NULL);
  	pthread_cond_init(&cond_leit, NULL);
  	pthread_cond_init(&cond_escr, NULL);

	if(argc < 2){
		printf("Digite: %s <um numero P>\n", argv[0]);
		return 1;
	}

	p = atoll(argv[1]);
	nThreads = pow(2, p);

	// Inicia o Vetor
	vector = malloc(sizeof (int) * nThreads);

	for(int i = 0; i < nThreads; i++)
		vector[i] = i + 1;


	// Cria as Threads
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
	if(tid==NULL) {
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}
	// Inicia as threads
	for(long int i=0; i<nThreads; i++) {
		if(pthread_create(tid+i, NULL, soma, (void*) i)){
			fprintf(stderr, "ERRO--pthread_create\n");
			return 3;
		}
	}

	 //aguardar o termino das threads
   	for(long int i=0; i<nThreads; i++) {
      if(pthread_join(*(tid+i), NULL)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   	}

	// verifica resultado
	int* copy;
	int isError = 0;
	copy = malloc(sizeof (int) * nThreads);
	for(int i = 0; i < nThreads; i++)
		copy[i] = i + 1;

	for(int i = 0; i < nThreads; i++){
		copy[i] += copy[i-1];
		printf("%d ", vector[i]);
		if(copy[i] != vector[i]){
			printf("\n\n\nERRO RESULTADO INVÁLIDO \n\n\n");
			isError = 1;
			break;
		}
	}

	if(!isError) printf("\n\n\nRESULTADO CORRETO \n\n\n");

	free(tid);
	free(vector);
	free(copy);

	return 0;
}
