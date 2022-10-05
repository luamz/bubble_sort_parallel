#include <stdio.h>
#include <mpi.h>
#include "vetor.c"

#define NUM 10000

char *odd_even(int i)
{

    if (i % 2 == 0)
        return "Even";
    else
        return "Odd ";
}

// Algorithm 10.3
//  Parallel algorithm of odd-even transposition

int main(argc, argv)
int argc;
char **argv;
{
    // ParallelOddEvenSort(double A[], int n) {
    int my_rank, np;
    int resto_elementos_processo_local;              // resto da divisao de numero de elementos por numero de processos
    int *vetor_elementos_processo_local;     // elementos a serem alocados a cada processo
    int *num_elementos_processo_local; // numero de elementos a serem alocados a cada processo
    int num_elementos;                 // numero de elementos total
    int *vetor_elementos;    // elementos do vetor a ser ordenado
    int *displacement;
    
    //Inicializa o ambiente de execucao
    MPI_Init(&argc, &argv);

    // Numero de processos
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    // Numero do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    vetor_elementos_processo_local = (int *)malloc(num_elementos/np * sizeof(int));

    if (my_rank == 0)
    {

        vetor_elementos = le_vetor(NUM);
        //imprime_vetor(vetor_elementos, NUM);

        num_elementos_processo_local = (int *)malloc(np * sizeof(int));
        //displacement = (int *)malloc(np * sizeof(int));
        resto_elementos_processo_local = num_elementos % np;

        for (int i = 0; i < np; i++)
        {
            if (resto_elementos_processo_local != 0)
            {
                num_elementos_processo_local[i] = num_elementos / np + 1;
                displacement[i] = 1;
                resto_elementos_processo_local--;
            }
            else
            {
                num_elementos_processo_local[i] = num_elementos / np;
                displacement[i] = 0;
            }
        }
    }
    else
    {
        MPI_Scatter(NULL, 0,
                MPI_INT, &vetor_elementos_processo_local, num_elementos/np,
                MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    MPI_Bcast(&vetor_elementos_processo_local,1,MPI_INT,0,MPI_COMM_WORLD);

    MPI_Scatter(&vetor_elementos, num_elementos/np,
                MPI_INT, &vetor_elementos_processo_local, num_elementos/np,
                MPI_INT, 0, MPI_COMM_WORLD);

    // MPI_Scatterv(vetor_elementos, num_elementos_processo_local, displacement,
    //             MPI_INT, &vetor_elementos_processo_local, 100,
    //             MPI_INT, 0, MPI_COMM_WORLD);

    
    imprime_vetor(vetor_elementos_processo_local, num_elementos_processo_local[my_rank]);

    MPI_Finalize();
    return 0;
}
