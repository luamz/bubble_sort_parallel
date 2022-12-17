# Comparando o desempenho do Bubble Sort

Nesse trabalho nós comparamos o desempenho do bubble sort (método de ordenação da bolha) em quatro modalidades:
 - Sequêncial
 - Paralelizado com MPI
 - Paralelizado com OpenMP
 - Paralelizado com OpenMP e com vetorização

## Execução

 - Sequêncial
 
 - Paralelizado com MPI
 
Para compilar rode ``mpicc bubble_sort_mpi.c -o bubble_sort_mpi`` e para executar rode `` mpirun -n X bubble_sort_mpi`` substituindo o "X" pelo número desejado de processos.

 - Paralelizado com OpenMP
 
Para compilar rode ``gcc bubble_sort_omp.c -o bubble_sort_omp -fopenmp`` e para executar rode ``./bubble_sort_omp X Y`` onde "X" é o número de elementos a serem ordenados e "Y" o número de Threads desejado.
 
 - Paralelizado com OpenMP e com vetorização

Para compilar rode ``gcc bubble_sort_omp.c -o bubble_sort_omp_vetorizado -fopenmp`` e para executar rode ``./bubble_sort_omp_vetorizado X Y`` onde "X" é o número de elementos a serem ordenados e "Y" o número de Threads desejado.
