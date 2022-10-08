#include <stdio.h>
#include <mpi.h>
#include "vetor.c"

#define NUM 10

int my_rank, np;

void imprimeVetorProcessos(double *vetor_elementos, int tam_vetor, double *vetor_elementos_processo_local, int num_elementos_processo_local) {
    MPI_Barrier(MPI_COMM_WORLD);
    if (my_rank == 0) {
        printf("\nVetor inicial:\n");
        imprime_vetor(vetor_elementos, tam_vetor);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < np; i++) {
        if (my_rank == i) {
            printf("\nRank = %d\n", my_rank);
            printf("Vetor do processo:\n");
            imprime_vetor(vetor_elementos_processo_local, num_elementos_processo_local);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}
 
void troca(double *vetor, int i, int j){
	int temp;
	temp = vetor[i];
	vetor[i] = vetor[j];
	vetor[j] = temp;
}

void bubblesort(double *vetor, int n){
	int i,j;
	for(i = n-2; i >= 0; i--){
		for(j = 0; j <= i; j++){
			if(vetor[j] > vetor[j+1]){
				troca(vetor, j, j+1);
            }
        }
    }
}

void compare_exchange_split(double *vetor_elementos_processo_local, int num_elementos_processo_local, int distanciaVizinho, bool crescente) {
    double *vetorProcessoVizinho = malloc(sizeof(double) * (num_elementos_processo_local));
    double *vetorMergeMyRankEVizinho = malloc(sizeof(double) * (num_elementos_processo_local*2)); 

    MPI_Status status;

    int vizinho = my_rank + distanciaVizinho;

    // faz o exchange de dados do vetor do vizinho a direita
    MPI_Sendrecv(vetor_elementos_processo_local, num_elementos_processo_local, MPI_DOUBLE, vizinho, 0,
    vetorProcessoVizinho, num_elementos_processo_local, MPI_DOUBLE, vizinho, 0,
    MPI_COMM_WORLD, &status);

    // faz o merge do seu vetor com o do vizinho
    //merge(vetor_elementos_processo_local, vetorProcessoVizinho, vetorMergeMyRankEVizinho);

    // separa os dados em cada 
    
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
    int resto_elementos_processo_local;              // resto da divisao de numero de elementos por numero de processos
    double *vetor_elementos_processo_local;     // elementos a serem alocados a cada processo
    int num_elementos_processo_local; // numero de elementos a serem alocados a cada processo
    int num_elementos = NUM;                 // numero de elementos total
    double *vetor_elementos;    // elementos do vetor a ser ordenado
    int tam_novo_vetor; //novo tamanho de vetor para numero de processos impares
   
    //Inicializa o ambiente de execucao
    MPI_Init(&argc, &argv);

    // Numero de processos
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    // Numero do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    vetor_elementos_processo_local = (double *)malloc(num_elementos/np * sizeof(double));

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
    }
      
    MPI_Bcast(&num_elementos_processo_local, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
    MPI_Scatter(vetor_elementos, num_elementos_processo_local,
                        MPI_DOUBLE, vetor_elementos_processo_local, num_elementos_processo_local,
                        MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    bubblesort(vetor_elementos_processo_local, num_elementos_processo_local);
    
    imprimeVetorProcessos(vetor_elementos, tam_novo_vetor, vetor_elementos_processo_local, num_elementos_processo_local);
    
    MPI_Finalize();
    return 0;
}
