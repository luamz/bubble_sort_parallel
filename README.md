# Comparando o desempenho do Bubble Sort

Nesse trabalho nós comparamos o desempenho do bubble sort (método de ordenação da bolha) em quatro modalidades:
 - Sequêncial
 - Paralelizado com MPI
 - Paralelizado com OpenMP
 - Paralelizado com OpenMP e com vetorização

## Execução

 - Sequêncial
 - Paralelizado com MPI
Para compilar rode ``mpicc bubble_sort_mpi.c -o bubble_sort_mpi`` e para executar rode `` mpirun -n X bubble_sort_mpi" substituindo o "X" pelo número desejado de processos.
 - Paralelizado com OpenMP
 - Paralelizado com OpenMP e com vetorização

// mpirun -n 6 bubble_sort