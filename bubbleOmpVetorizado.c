#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

void bubblesort_odd_even(double *vetor_elementos, int num_elementos)
{
    #pragma omp parallel num_threads(omp_get_num_threads()) shared(vetor_elementos, num_elementos)
    {
        int tid = omp_get_thread_num();
        for (int iteracao = 0; iteracao < num_elementos; iteracao++)
        {
            if (iteracao % 2 == 1) // Odd iteration
            {
                #pragma omp simd
                for (int i = 1; i < num_elementos - 1; i += 2)
                {
                    if (vetor_elementos[i] > vetor_elementos[i + 1])
                    {
                        //printf("thread: %d - iteração %d, troca %g e %g \n", tid, iteracao, vetor_elementos[i], vetor_elementos[i+1]);
                        troca(vetor_elementos, i, i + 1);
                    }
                }
            }
            if (iteracao % 2 == 0) // Even iteration
            {
                #pragma omp simd
                for (int i = 1; i < num_elementos; i += 2)
                {
                    if (vetor_elementos[i - 1] > vetor_elementos[i])
                    {
                        //printf("thread: %d - iteração %d, troca %g e %g \n", tid, iteracao, vetor_elementos[i-1], vetor_elementos[i]);
                        troca(vetor_elementos, i, i - 1);
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Você deve passar o número de elementos a ordernar e o número de threads!\n");
        return 0;
    }

    int elementos = atoi(argv[1]);  // Lendo o tamanho do vetor
    int nthreads = atoi(argv[2]);   // Lendo o número de threads

    omp_set_num_threads(nthreads);  // Settando o Número de threads
    double *vetor_elementos;        // Elementos do vetor a ser ordenado
    int num_elementos = elementos;  // Número total de elementos do vetor a ser ordenado
    double inicio, fim, duracao;    // Tempos

    vetor_elementos = le_vetor(num_elementos, 0);

    // imprime_vetor(vetor_elementos, num_elementos);

    inicio = omp_get_wtime();
    bubblesort_odd_even(vetor_elementos, num_elementos);
    duracao = omp_get_wtime() - inicio;

    // imprime_vetor(vetor_elementos, num_elementos);
    printf("\nTempo de Execução: %f segundos \n", duracao);

    free(vetor_elementos);

    return 0;
}
