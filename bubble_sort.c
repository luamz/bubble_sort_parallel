#include <stdio.h>
#include <mpi.h>
#include "vetor.c"

#define NUM 10

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
    int num_elementos_processo_local; // numero de elementos a serem alocados a cada processo
    int num_elementos = NUM;                 // numero de elementos total
    int *vetor_elementos;    // elementos do vetor a ser ordenado
    int tam_novo_vetor; //novo tamanho de vetor para numero de processos impares
   
    //Inicializa o ambiente de execucao
    MPI_Init(&argc, &argv);

    // Numero de processos
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    // Numero do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    vetor_elementos_processo_local = (int *)malloc(num_elementos/np * sizeof(int));

    if (my_rank == 0)
    {
       
        resto_elementos_processo_local = num_elementos % np;

        if(resto_elementos_processo_local == 0){
            num_elementos_processo_local = (num_elementos / np);
        }else{
            num_elementos_processo_local = (num_elementos / np) + 1;

        }

        tam_novo_vetor = np * num_elementos_processo_local; 

        vetor_elementos = le_vetor(num_elementos, (tam_novo_vetor-num_elementos));
        imprime_vetor(vetor_elementos, tam_novo_vetor);
       /* int MPI_Bcast( void *buffer, int count, MPI_Datatype datatype, int root, 
               MPI_Comm comm )*/
            
    }
      
    MPI_Bcast(&num_elementos_processo_local, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
    MPI_Scatter(vetor_elementos, num_elementos_processo_local,
                        MPI_INT, vetor_elementos_processo_local, num_elementos_processo_local,
                        MPI_INT, 0, MPI_COMM_WORLD);

    // MPI_Scatterv(vetor_elementos, num_elementos_processo_local, displacement,
    //             MPI_INT, &vetor_elementos_processo_local, 100,
    //             MPI_INT, 0, MPI_COMM_WORLD);

      printf("RANK: %d\n", my_rank);
    //imprime_vetor(vetor_elementos_processo_local, num_elementos_processo_local[my_rank]);
     imprime_vetor(vetor_elementos_processo_local, num_elementos_processo_local);
    
 
    /*for (int i = 0; i < num_elementos_processo_local; i++) {
        printf("%d : %d ", i, vetor_elementos_processo_local[i]);
    }
    printf("\n");*/

    MPI_Finalize();
    return 0;
}
