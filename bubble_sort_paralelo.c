#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include "util.c"
#include "bubble_sort_sequencial.c"
#include <sys/time.h>
#include <time.h>

#define NUM 20000

float calcula_tempo_gasto(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

void imprime_vetor_processos(int my_rank, int np, double *vetor_elementos, int tam_vetor,
                             double *vetor_elementos_processo_local, int num_elementos_processo_local)
{
    MPI_Barrier(MPI_COMM_WORLD);
    if (my_rank == 0)
    {
        printf("\nVetor inicial:\n");
        imprime_vetor_global(vetor_elementos, tam_vetor);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < np; i++)
    {
        if (my_rank == i)
        {
            printf("\nRank = %d\n", my_rank);
            printf("Vetor do processo:\n");
            imprime_vetor(vetor_elementos_processo_local, num_elementos_processo_local);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

double *merge(double *vetor_1, double *vetor_2, int num_elementos_processo_local)
{
    int counter = 0;
    int merge_size = num_elementos_processo_local * 2;
    double *vetor_merge = malloc(sizeof(double) * merge_size);

    for (int i = 0; i < num_elementos_processo_local; i++)
    {
        vetor_merge[counter] = vetor_1[i];
        counter++;
    }
    for (int j = 0; j < num_elementos_processo_local; j++)
    {
        vetor_merge[counter] = vetor_2[j];
        counter++;
    }

    bubble_sort(vetor_merge, merge_size);

    return vetor_merge;
}

void split(double *vetor_1, double *vetor_2, double *vetor_merge, int num_elementos_processo_local)
{
    int i = 0, j = 0, k = 0;
    int merge_size = num_elementos_processo_local * 2;

    while (k < merge_size)
    {
        if (i < num_elementos_processo_local)
        {
            vetor_1[i] = vetor_merge[k];
            i++;
            k++;
        }
        else if (j < num_elementos_processo_local)
        {
            vetor_2[j] = vetor_merge[k];
            j++;
            k++;
        }
    }
}

void compare_split(double *vetor_elementos_processo_local, int num_elementos_processo_local,
                   int vizinho, int primeiro_do_par, MPI_Status status)
{

    double *vetor_processo_vizinho = malloc(sizeof(double) * (num_elementos_processo_local));
    double *vetor_merge = malloc(sizeof(double) * (num_elementos_processo_local * 2));

    MPI_Sendrecv(vetor_elementos_processo_local, num_elementos_processo_local, MPI_DOUBLE, vizinho, 0,
                 vetor_processo_vizinho, num_elementos_processo_local, MPI_DOUBLE, vizinho, 0,
                 MPI_COMM_WORLD, &status);

    vetor_merge = merge(vetor_elementos_processo_local, vetor_processo_vizinho, num_elementos_processo_local);

    if (primeiro_do_par == 1)
    {
        split(vetor_elementos_processo_local, vetor_processo_vizinho, vetor_merge, num_elementos_processo_local);
    }
    else
    {
        split(vetor_processo_vizinho, vetor_elementos_processo_local, vetor_merge, num_elementos_processo_local);
    }
}

void bubblesort_odd_even(int my_rank, int np, double *vetor_elementos_processo_local,
                         int num_elementos_processo_local)
{
    MPI_Status status;

    for (int i = 0; i < np; i++)
    {
        if (i % 2 == 1) // Odd iteration
        {
            if (my_rank % 2 == 1) // Odd process number
            {
                if (my_rank < np - 1) // Compare-exchange com o proximo vizinho
                {
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local,
                                  my_rank + 1, 1, status);
                }
            }
            else
            {
                if (my_rank > 0) // Compare-exchange com o vizinho anterior
                {
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local,
                                  my_rank - 1, 0, status);
                }
            }
        }

        if (i % 2 == 0) // Even iteration
        {
            if (my_rank % 2 == 0) // Even process number
            {
                if (my_rank < np - 1) // Compare-exchange com o proximo vizinho
                {
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local,
                                  my_rank + 1, 1, status);
                }
            }
            else
            { // Compare-exchange com o vizinho anterior
                compare_split(vetor_elementos_processo_local, num_elementos_processo_local,
                              my_rank - 1, 0, status);
            }
        }
    }
}

int calcula_novo_tamanho_vetor(int np, int num_elementos)
{
    int num_elementos_processo_local;
    int resto_elementos_processo_local = num_elementos % np;

    if (resto_elementos_processo_local == 0)
    {
        num_elementos_processo_local = (num_elementos / np);
    }
    else
    {
        num_elementos_processo_local = (num_elementos / np) + 1;
    }

    return (np * num_elementos_processo_local);
}

int main(int argc, char *argv[])
{
    int my_rank, np;
    double *vetor_elementos;                // Elementos do vetor a ser ordenado
    double *vetor_elementos_processo_local; // Elementos a serem alocados a cada processo
    int num_elementos = NUM;                // Numero total de elementos do vetor a ser ordenado
    int num_elementos_processo_local;       // Numero de elementos a serem alocados a cada processo
    int tam_novo_vetor;
    
    struct timeval tempo_inicial, tempo_final;
    
    MPI_Init(&argc, &argv);                  // Inicializa o ambiente de execucao
    MPI_Comm_size(MPI_COMM_WORLD, &np);      // Numero de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // Numero do processo

    if (my_rank == 0)
    {
        //clock_gettime(CLOCK_REALTIME, &comeco);
        gettimeofday(&tempo_inicial, NULL);
        tam_novo_vetor = calcula_novo_tamanho_vetor(np, num_elementos);
        vetor_elementos = le_vetor(num_elementos, (tam_novo_vetor - num_elementos));
    }

    vetor_elementos_processo_local = (double *)malloc(num_elementos / np * sizeof(double));

    MPI_Bcast(&num_elementos_processo_local, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vetor_elementos, num_elementos_processo_local, MPI_DOUBLE,
                vetor_elementos_processo_local, num_elementos_processo_local, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    imprime_vetor_processos(my_rank, np, vetor_elementos, tam_novo_vetor, vetor_elementos_processo_local,
                            num_elementos_processo_local);

    MPI_Barrier(MPI_COMM_WORLD);

    bubble_sort(vetor_elementos_processo_local, num_elementos_processo_local);
    bubblesort_odd_even(my_rank, np, vetor_elementos_processo_local, num_elementos_processo_local);

    MPI_Gather(vetor_elementos_processo_local, num_elementos_processo_local, MPI_DOUBLE,
               vetor_elementos, num_elementos_processo_local, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(vetor_elementos_processo_local);

    if (my_rank == 0)
    {
        printf("\nVetor ordenado: ");
        imprime_vetor_global(vetor_elementos, tam_novo_vetor);
        
        free(vetor_elementos);

        gettimeofday(&tempo_final, NULL);
       
        printf("Tempo de execução gasto: %0.8f sec\n", calcula_tempo_gasto(&tempo_inicial, &tempo_final));
    }

    MPI_Finalize();

    return 0;
}
