#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size, rank;
  const int ARRAY_SIZE = 10;
  int a[ARRAY_SIZE], b[ARRAY_SIZE];
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (size != 2) {
    printf("The program must be run on 2 processes (instead of %d)\n", size);
    exit(1);
  }
  if (!rank) {
    srand(time(0));
    printf("Sending a = ");
    for (int &i : a) {
      i = rand() % 1000;
      printf("%d ", i);
    }
    printf("\n");
    MPI_Send(a, ARRAY_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else {
    MPI_Recv(b, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, nullptr);
    printf("Received b = ");
    for (int i : b) {
      printf("%d ", i);
    }
    printf("\n");
  }
  MPI_Finalize();
}
