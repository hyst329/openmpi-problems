#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size, rank;
  const int ARRAY_SIZE = 10;
  int a[ARRAY_SIZE];
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (!rank) {
    srand(time(0));
    printf("a = ");
    for (int &i : a) {
      i = rand() % 1000;
      printf("%d ", i);
    }
    printf("\n");
    for (int p = 1; p < size; p++) {
      printf("Sending from 1 to %d\n", p + 1);
      MPI_Send(a, ARRAY_SIZE, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Status status;
    MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    int data_size;
    MPI_Get_count(&status, MPI_INT, &data_size);
    int *b = new int[data_size];
    MPI_Recv(b, data_size, MPI_INT, 0, 0, MPI_COMM_WORLD, nullptr);
    printf("b = ");
    for (int i = 0; i < data_size; i++) {
      printf("%d ", b[i]);
    }
    printf("\n");
  }
  MPI_Finalize();
}
