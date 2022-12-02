#include <stdio.h>
#include <omp.h>
#include "vetor.c"
#include "utils.c"
#define nthreads 4
#define NUM 40000

int main()
{
    omp_set_num_threads(nthreads);
    double *vetor_elementos;                // Elementos do vetor a ser ordenado
    double *vetor_elementos_processo_local; // Elementos a serem alocados a cada processo
    int num_elementos = NUM;                // Numero total de elementos do vetor a ser ordenado
    int num_elementos_processo_local;       // Numero de elementos a serem alocados a cada processo
    int tam_novo_vetor;
    double inicio, fim, duracao, elapsed;

    /* int resto_elementos_processo_local = num_elementos % omp_get_num_threads();

     if (resto_elementos_processo_local == 0)
     {
         num_elementos_processo_local = (num_elementos / omp_get_num_threads());
     } else {
         num_elementos_processo_local = (num_elementos / omp_get_num_threads()) + 1;
     }

     tam_novo_vetor = omp_get_num_threads() * num_elementos_processo_local; */
    vetor_elementos = le_vetor(num_elementos, 0);

    inicio = omp_get_wtime();
    
    // vetor_elementos_processo_local = (double *) malloc (num_elementos / np * sizeof(double));

    // imprime_vetor_processos(my_rank,np,vetor_elementos,tam_novo_vetor,vetor_elementos_processo_local,
    //                          num_elementos_processo_local);

    bubble_sort(vetor_elementos_processo_local, num_elementos_processo_local);

    bubblesort_odd_even(np, my_rank, vetor_elementos_processo_local, num_elementos_processo_local);

    duracao = omp_get_wtime() - inicio;

    printf("\n");
    imprime_vetor(vetor_elementos, tam_novo_vetor);
    printf("\nTempo de Execução: %f segundos \n", duracao);

    free(vetor_elementos);
    free(vetor_elementos_processo_local);

    return 0;
}
