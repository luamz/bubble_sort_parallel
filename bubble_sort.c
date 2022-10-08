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

/*char *odd_even(int i)
{

    if (i % 2 == 0)
        return "Even";
    else
        return "Odd ";
}*/

// Algorithm 10.3
//  Parallel algorithm of odd-even transposition

 
int main(int argc, char *argv[])
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
        //imprime_vetor(vetor_elementos, tam_novo_vetor);
            
    }
      
    MPI_Bcast(&num_elementos_processo_local, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
    MPI_Scatter(vetor_elementos, num_elementos_processo_local,
                        MPI_INT, vetor_elementos_processo_local, num_elementos_processo_local,
                        MPI_INT, 0, MPI_COMM_WORLD);
    
    bubblesort(vetor_elementos_processo_local, num_elementos_processo_local);
   

    printf("RANK: %d\n", my_rank);
    imprime_vetor(vetor_elementos_processo_local, num_elementos_processo_local);
    
    MPI_Finalize();
    return 0;
}
