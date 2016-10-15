#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int ARRAY_SIZE = 8;
  int a[ARRAY_SIZE * ARRAY_SIZE];
  int x[ARRAY_SIZE];
  if (!rank) {
    srand(time(0));
    printf("a = ");
    for (int i = 0; i < ARRAY_SIZE * ARRAY_SIZE; i++) {
      a[i] = rand() % 1000;
      printf(i % ARRAY_SIZE ? "%3d " : "\n%3d ", a[i]);
    }
    printf("\n");
  }
  int *indices = new int[size], *counts = new int[size];
  for (int i = 0; i < size; i++) {
    counts[i] = (ARRAY_SIZE / size) + (i < (ARRAY_SIZE % size));
    indices[i] = i ? indices[i - 1] + counts[i - 1] : 0;
  }
  for (int i = 0; i < size; i++) {
    MPI_Send(a + indices[i] * ARRAY_SIZE, counts[i] * ARRAY_SIZE, MPI_INT, i, 0,
             MPI_COMM_WORLD);
  }
  int c = counts[rank];
  int idx = indices[rank];
  int *diag = new int[c];
  int *data = new int[c * ARRAY_SIZE];
  MPI_Recv(data, c * ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, nullptr);
  for (int i = 0; i < c; i++)
    diag[i] = data[i * ARRAY_SIZE + idx + i];
  MPI_Send(diag, c, MPI_INT, 0, 1, MPI_COMM_WORLD);
  if (!rank) {
    for (int i = 0; i < size; i++) {
      MPI_Recv(x + indices[i], counts[i], MPI_INT, i, 1, MPI_COMM_WORLD,
               nullptr);
    }
    printf("x = ");
    for (int &i : x) {
      printf("%d ", i);
    }
    printf("\n");
  }
  MPI_Finalize();
}
