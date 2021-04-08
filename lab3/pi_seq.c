#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

int main(int argc, char *argv[]) {
	long long int nSerie;
	int isSoma = 1;
	double result, aprox, serieV;

	if(argc < 2){
		printf("Digite: %s <elementos da serie>\n", argv[0]);
		return 1;
	}

	nSerie = atoll(argv[1]);

	for(int i = 0; i < nSerie; i++){
		serieV = 1 + 2 * i;

		if(isSoma) result += (1/serieV);
		else result -= (1/serieV);
		isSoma = !isSoma;
	}

	result = result * 4;
	aprox = M_PI - result;

	printf("Resultado: %.15f \nDesvio: %.15f\n", result, aprox);

	return 0;
}