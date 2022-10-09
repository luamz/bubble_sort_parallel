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

int main (){

	#define NUM 10

	double *vetor_elementos; 
	vetor_elementos = le_vetor(NUM, 0);
	
	time_t tempo_inicial;
     
	bubble_sort(vetor_elementos, NUM);

	imprime_vetor(vetor_elementos, NUM);

	time_t tempo_final;

	printf("Tempo de execução: %ld", (tempo_final-tempo_inicial));

}