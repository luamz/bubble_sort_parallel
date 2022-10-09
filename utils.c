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