#include <stdio.h>
#include <time.h>
#include "utils.c"


int main (){

	#define NUM 20000

	double *vetor_elementos; 
	vetor_elementos = le_vetor(NUM, 0);

	clock_t t;
    t = clock();
     
	bubble_sort(vetor_elementos, NUM);

	// imprime_vetor(vetor_elementos, NUM);

	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time

	printf("Tempo de execução: %f segundos\n", time_taken);
}