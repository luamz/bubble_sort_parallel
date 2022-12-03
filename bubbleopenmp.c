#include <stdio.h>
#include <omp.h>
#include "vetor.c"
#include "utils.c"

#define nthreads 4
#define NUM 10

void imprime_vetor(double *vetor_elementos, int num_elementos) {
    for (int i = 0; i < num_elementos; i++){
        printf("%f", vetor_elementos[i]);
    }
}

void bubblesort_odd_even(int n_threads, double *vetor_elementos, int num_elementos)
{
    int i, k;
    double aux;

    #pragma omp parallel num_threads(n_threads) shared(vetor_elementos, num_elementos) private(i, aux, k)
    for (k = 0; k < num_elementos; k++)
    {   
        if (k % 2 == 1) // Odd iteration
        {
            #pragma omp for
            for(i = 1; i < num_elementos - 1; i += 2){
                if(vetor_elementos[i] > vetor_elementos[i + 1]){
                    aux = vetor_elementos[i + 1];
                    vetor_elementos[i + 1] = vetor_elementos[i];
                    vetor_elementos[i] = aux;
                }
            }
        }
        if (k % 2 == 0) // Even iteration
        {
            #pragma omp for
            for(i = 1; i < num_elementos; i += 2){
                if(vetor_elementos[i-1] > vetor_elementos[i]){
                    aux = vetor_elementos[i - 1];
                    vetor_elementos[i - 1] = vetor_elementos[i];
                    vetor_elementos[i] = aux;
                }
            }
        
        }
    
    }
}
 
int main()
{
    omp_set_num_threads(nthreads);
    double *vetor_elementos;                // Elementos do vetor a ser ordenado
    //double *vetor_elementos_processo_local; // Elementos a serem alocados a cada processo
    int num_elementos = NUM;                // Numero total de elementos do vetor a ser ordenado
    //int num_elementos_processo_local;       // Numero de elementos a serem alocados a cada processo
    //int tam_novo_vetor;
    double inicio, fim, duracao;

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

    //bubble_sort(vetor_elementos_processo_local, num_elementos_processo_local);

    bubblesort_odd_even(omp_get_num_threads(), vetor_elementos, num_elementos);

    duracao = omp_get_wtime() - inicio;

    printf("\n");
    imprime_vetor(vetor_elementos, num_elementos);
    
    printf("\nTempo de Execução: %f segundos \n", duracao);

    free(vetor_elementos);
    //free(vetor_elementos_processo_local);

    return 0;
}
