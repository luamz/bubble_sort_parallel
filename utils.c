#include <stdio.h>
#include "vetor.c"
#include <time.h>

void troca(double *vetor, int i, int j){
	int temp;
	temp = vetor[i];
	vetor[i] = vetor[j];
	vetor[j] = temp;
}

void bubble_sort(double *vetor, int n){

    int i,j;
	for(i = n-2; i >= 0; i--){
		for(j = 0; j <= i; j++){
			if(vetor[j] > vetor[j+1]){
				troca(vetor, j, j+1);
            }
        }
    }
}

/*int main (){

	#define NUM 20000

	double *vetor_elementos; 
	vetor_elementos = le_vetor(NUM, 0);

	clock_t t;
    t = clock();
     
	bubble_sort(vetor_elementos, NUM);

	imprime_vetor(vetor_elementos, NUM);

	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time

	printf("Tempo de execução: %f segundos\n", time_taken);

}*/