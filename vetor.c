#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int *le_vetor(int n)
{
    FILE *arq;
    char c[n];
    int *vetor  = malloc(sizeof(int) * n); 
    char path[20] = "vetores/vet";
    char nome[20];
    sprintf(nome, "%d", n);
    strcat(nome, ".txt");
    strcat(path,nome);

    printf("%s", path);
    if (arq = fopen(path, "r")) {
        for (int i =0; i<n ; i++){
            fscanf(arq, "%d,", &vetor[i] ); 
        }
        fclose(arq);
    }

    else {
        printf("Error! Arquivo não existe.");
        exit(1);
    }

    return vetor;
}



void gera_vetor(int n, int semente)
{
    char nome[20];
    char caminho[20] = "vetores/vet";

    srand(time(NULL));

    FILE *arquivo;
    sprintf(nome, "%d", n);
    strcat(nome, ".txt");
    strcat(caminho, nome);

    arquivo = fopen(caminho, "w");

    for (int k = 0; k < n; k++)
    {
        fprintf(arquivo, "%d,", rand() % semente);
    }

    fprintf(arquivo, "\n");
    fclose(arquivo);
}

void imprime_vetor(int A[], int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", A[i]);
	printf("\n");
}

// int main()
// {
//     int *vetor = le_vetor(10000);
//     imprimeVetor(vetor,10000);
//     // gera_vetor(10000, 1000);
//     // gera_vetor(20000, 1000);
//     // gera_vetor(40000, 1000);
// }