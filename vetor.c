#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

double *le_vetor(int n, int resto)
{
    FILE *arq;
    char c[n];
    double *vetor  = malloc(sizeof(double) * (n+resto)); 
    char path[20] = "vetores/vet";
    char nome[20];
    sprintf(nome, "%d", n);
    strcat(nome, ".txt");
    strcat(path,nome);

    //printf("%d", resto);

    //printf("%s", path);
    if (arq = fopen(path, "r")) {
        for (int i =0; i<n ; i++){
            fscanf(arq, "%lf,", &vetor[i]); 
        }
        fclose(arq);
        if(resto != 0){
            for(int i = n; i < (n+1 + resto); i++){
                //printf("%d\n", i);
                vetor[i] = __INT_MAX__;
            }
        }
    }
    else {
        printf("Error! Arquivo não existe.");
        exit(1);
    }

    //printf("%d\n", n);
    //printf("%d", resto);


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

void imprime_vetor(double A[], int n)
{
	for (int i = 0; i < n; i++)
		printf("%lf ", A[i]);
	printf("\n");
}

//int main()
//{
//     int *vetor = le_vetor(10000);
//     imprimeVetor(vetor,10000);
    //gera_vetor(10, 1000);
//     // gera_vetor(20000, 1000);
//     // gera_vetor(40000, 1000);
//}