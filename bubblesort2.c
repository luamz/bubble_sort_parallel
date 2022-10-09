#include <stdio.h>
#include <mpi.h>
#include "vetor.c"
 
#define NUM 10
 
void troca(int *vetor, int i, int j){
	int temp;
	temp = vetor[i];
	vetor[i] = vetor[j];
	vetor[j] = temp;
}

void bubblesort(int *vetor, int n){
	int i,j;
	for(i = n-2; i >= 0; i--){
		for(j = 0; j <= i; j++){
			if(vetor[j] > vetor[j+1]){
				troca(vetor, j, j+1);
            }
        }
    }
}


// Algorithm 10.3
//  Parallel algorithm of odd-even transposition
 
int main(int argc, char *argv[])
{   
    int sum = 0;
    int my_rank, np;
    int resto_elementos_processo_local;              // resto da divisao de numero de elementos por numero de processos
    int *vetor_elementos_processo_local;     // elementos a serem alocados a cada processo
    int num_elementos_processo_local; // numero de elementos a serem alocados a cada processo
    int num_elementos = NUM;                 // numero de elementos total
    int *vetor_elementos = (int *)malloc(NUM * sizeof(int));   // elementos do vetor a ser ordenado
   
    //Inicializa o ambiente de execucao
    MPI_Init(&argc, &argv);
 
    // Numero de processos
    MPI_Comm_size(MPI_COMM_WORLD, &np);
   
    // Numero do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
   
    vetor_elementos = le_vetor(NUM, 0);
    //imprime_vetor(vetor_elementos, NUM);
    
    resto_elementos_processo_local = (num_elementos % np);
   
    for (int i = 0; i < np; i++)
    {
        if (resto_elementos_processo_local != 0)
        {   
            num_elementos_processo_local = (num_elementos / np) + 1;
            resto_elementos_processo_local--;
        }
        else
        {   
            num_elementos_processo_local = (num_elementos / np);
        }
    }

    //MPI_Bcast(&num_elementos_processo_local ,1,MPI_INT,0,MPI_COMM_WORLD);

    vetor_elementos_processo_local = (int *)malloc((num_elementos_processo_local) * sizeof(int));

    MPI_Scatter(vetor_elementos, num_elementos_processo_local,
                MPI_INT, vetor_elementos_processo_local, num_elementos_processo_local,
                MPI_INT, 0, MPI_COMM_WORLD);
    
    bubblesort(vetor_elementos_processo_local, num_elementos_processo_local);
    
    for (int i = 1; i < np; i++)
    {

        // Odd iteration
        if (i % 2 == 1)
        {
            // Odd process number
            if (my_rank % 2 == 1)
            {
                // Compare-exchange with the right neighbor process
                if (my_rank < np - 1)
                {   
                    //compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank, my_rank + 1);
                    //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador direita %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank + 1);
                }
            }

            else
            {

                // Compare-exchange with the left neighbor process
                if (my_rank > 0)
                {   
                    //compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank, my_rank - 1);
                    //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador esquerda %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank - 1);
                }
            }
        }
        // Even iteration
        if (i % 2 == 0)
        {

            // Even process number
            if (my_rank % 2 == 0)
            {

                // Compare-exchange with the right neighbor process
                if (my_rank < np - 1)
                {   
                    //compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank, my_rank + 1);
                    //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador direita %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank + 1);
                }
            }

            // Compare-exchange with the left neighbor process
            else
            {   
                //compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank, my_rank - 1);
                //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador esquerda %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank - 1);
            }
        }
    }

    
    printf("RANK: %d\n", my_rank);
    for (int i = 0; i < num_elementos_processo_local; i++) {
        printf("%d : %d ", i, vetor_elementos_processo_local[i]);
    }
    printf("\n");

    MPI_Finalize();
    
    return 0;
}