#include <stdio.h>
#include <mpi.h>
#include "vetor.c"

#define NUM 10


/*void imprime_vetor_processos(int my_rank, int np, double *vetor_elementos, int tam_vetor, double *vetor_elementos_processo_local, int num_elementos_processo_local) {
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
}*/
 
void troca(double *vetor, int i, int j){
	int temp;
	temp = vetor[i];
	vetor[i] = vetor[j];
	vetor[j] = temp;
}

void bubble_sort(double *vetor, int n){
	int i,j;
	for(i = n-2; i >= 0; i--){
		for(j = 0; j <= i; j++){
			if(vetor[j] > vetor[j+1]){
				troca(vetor, j, j+1);
            }
        }
    }
}

double* merge(double* vetor_1, double* vetor_2, int num_elementos_processo_local){
    int i = 0, j = 0, k = 0;
    double *vetor_merge = malloc (sizeof (double) * (num_elementos_processo_local * 2));

    while(k < (2 * num_elementos_processo_local) - 1){
        if(vetor_1[i] <= vetor_2[j] || j == num_elementos_processo_local)
        {
            vetor_merge[k] = vetor_1[i];
            i++;
            k++;
        }
        else if (vetor_1[i] >= vetor_2[j] || i == num_elementos_processo_local)
        {
            vetor_merge[k] = vetor_2[j];
            j++;
            k++;
        }
    }
    return vetor_merge;
}

void split(double *vetor_1, double *vetor_2, double *vetor_merge, int num_elementos_processo_local) {
    int i = 0, j = 0, k = 0;

    while(k < (2 * num_elementos_processo_local) - 1){
        if (i < num_elementos_processo_local - 1)
        {
            vetor_1[i] = vetor_merge[k];
            i++;
            k++;
        }
        else if (j < num_elementos_processo_local - 1)
        {
            vetor_2[j] = vetor_merge[k];
            j++;
            k++;
        }
    }
}

void compare_split(double *vetor_elementos_processo_local, int num_elementos_processo_local, 
                   int my_rank, int vizinho, int primeiro_do_par, MPI_Status status) {
    double *vetor_processo_vizinho = malloc(sizeof(double) * (num_elementos_processo_local));
    double *vetor_merge_myRank_e_vizinho = malloc(sizeof(double) * (num_elementos_processo_local*2));

    // Recebe dados do vetor do vizinho
    MPI_Sendrecv(vetor_elementos_processo_local, num_elementos_processo_local, MPI_DOUBLE, vizinho, 0,
                 vetor_processo_vizinho, num_elementos_processo_local, MPI_DOUBLE, vizinho, 0,
                 MPI_COMM_WORLD, &status);

    // Faz o merge do seu vetor com o do vizinho
    vetor_merge_myRank_e_vizinho = merge(vetor_elementos_processo_local, vetor_processo_vizinho,
                                         num_elementos_processo_local);

    // separa os dados em cada 
    if(primeiro_do_par == 1){
        split(vetor_elementos_processo_local, vetor_processo_vizinho, vetor_merge_myRank_e_vizinho, num_elementos_processo_local);
    }
    else {
        split(vetor_processo_vizinho, vetor_elementos_processo_local, vetor_merge_myRank_e_vizinho, num_elementos_processo_local);
    }

    MPI_Send(vetor_processo_vizinho, num_elementos_processo_local, MPI_DOUBLE, vizinho, 0, MPI_COMM_WORLD);
    
}

/*void bubblesort_odd_even(int np, int my_rank, double *vetor_elementos_processo_local, int num_elementos_processo_local, MPI_Status status)
{
    printf("entrouu");
    for (int i = 1; i < np; i++)
    {
        // Odd iteration
        if (i % 2 == 1)
        {
            // Odd process number
            if (my_rank % 2 == 1)
            {
                // Compare-exchange com o proximo vizinho
                if (my_rank < np - 1)
                {   
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank + 1, 1, status);
                    printf("Rank: %d \nVizinho: %d", my_rank, my_rank+1);
                    //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador direita %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank + 1);
                }
            }

            else
            {
                // Compare-exchange com o vizinho anterior
                if (my_rank > 0)
                {   
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank - 1, 0, status);
                    printf("Rank: %d \nVizinho: %d", my_rank, my_rank+1);
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
                // Compare-exchange com o proximo vizinho
                if (my_rank < np - 1)
                {   
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank + 1, 1, status);
                    printf("Rank: %d \nVizinho: %d", my_rank, my_rank+1);
                    //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador direita %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank + 1);
                }
            }
            // Compare-exchange com o vizinho anterior
            else
            {   
                compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank - 1, 0, status);
                printf("Rank: %d \nVizinho: %d", my_rank, my_rank+1);
                //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador esquerda %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank - 1);
            }
        }
    }
}
*/
 
int main(int argc, char *argv[])
{   
    int my_rank, np;
    double *vetor_elementos;                    // Elementos do vetor a ser ordenado
    double *vetor_elementos_processo_local;     // Elementos a serem alocados a cada processo
    int num_elementos = NUM;                    // Numero total de elementos do vetor a ser ordenado
    int num_elementos_processo_local;           // Numero de elementos a serem alocados a cada processo
    int tam_novo_vetor;

    MPI_Init(&argc, &argv);                     // Inicializa o ambiente de execucao
    MPI_Comm_size(MPI_COMM_WORLD, &np);         // Numero de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    // Numero do processo
    MPI_Status status;


    if (my_rank == 0)
    {
        int resto_elementos_processo_local = num_elementos % np;

        if (resto_elementos_processo_local == 0)
        {
            num_elementos_processo_local = (num_elementos / np);
        } else {
            num_elementos_processo_local = (num_elementos / np) + 1;
        }

        tam_novo_vetor = np * num_elementos_processo_local; 
        vetor_elementos = le_vetor(num_elementos, (tam_novo_vetor - num_elementos));
    }
      
    vetor_elementos_processo_local = (double *) malloc (num_elementos / np * sizeof(double));

    MPI_Bcast(&num_elementos_processo_local, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vetor_elementos, num_elementos_processo_local, MPI_DOUBLE,
                vetor_elementos_processo_local, num_elementos_processo_local, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    bubble_sort(vetor_elementos_processo_local, num_elementos_processo_local);
    
    //imprime_vetor_processos(my_rank, np, vetor_elementos, tam_novo_vetor, 
    //                      vetor_elementos_processo_local, num_elementos_processo_local);

    if (my_rank == 0)
    {   
        printf("liberou");
        free(vetor_elementos);
    }

    printf("comeca bubblesort");
    //odd-even
    for (int i = 1; i < np; i++)
    {
        // Odd iteration
        if (i % 2 == 1)
        {
            // Odd process number
            if (my_rank % 2 == 1)
            {
                // Compare-exchange com o proximo vizinho
                if (my_rank < np - 1)
                {   
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank + 1, 1, status);
                    printf("Rank: %d \nVizinho: %d", my_rank, my_rank+1);
                    //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador direita %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank + 1);
                }
            }

            else
            {
                // Compare-exchange com o vizinho anterior
                if (my_rank > 0)
                {   
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank - 1, 0, status);
                    printf("Rank: %d \nVizinho: %d", my_rank, my_rank+1);
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
                // Compare-exchange com o proximo vizinho
                if (my_rank < np - 1)
                {   
                    compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank + 1, 1, status);
                    printf("Rank: %d \nVizinho: %d", my_rank, my_rank+1);
                    //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador direita %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank + 1);
                }
            }
            // Compare-exchange com o vizinho anterior
            else
            {   
                compare_split(vetor_elementos_processo_local, num_elementos_processo_local, my_rank, my_rank - 1, 0, status);
                printf("Rank: %d \nVizinho: %d", my_rank, my_rank+1);
                //printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador esquerda %d\n", i, odd_even(i), my_rank, odd_even(my_rank), my_rank, my_rank - 1);
            }
        }
    }

    if (my_rank == 0)
    {
        double *vetor_elementos = (double *)malloc(tam_novo_vetor * sizeof(double));
    }

    MPI_Gather(vetor_elementos_processo_local, num_elementos_processo_local, MPI_DOUBLE, vetor_elementos, 
                num_elementos_processo_local, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    imprime_vetor(vetor_elementos, tam_novo_vetor);
    
    MPI_Finalize();
    return 0;
}
