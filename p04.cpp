#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int ARRAY_SIZE = 20;
  int x[ARRAY_SIZE], y[ARRAY_SIZE], z1[ARRAY_SIZE], z2[ARRAY_SIZE];
  int *indices = new int[size], *counts = new int[size];
  if (!rank) {
    srand(time(0));
    printf("x = ");
    for (int &i : x) {
      i = rand() % 1000;
      printf("%d ", i);
    }
    printf("\n");
    printf("y = ");
    for (int &i : y) {
      i = rand() % 1000;
      printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
      counts[i] = (ARRAY_SIZE / size) + (i < (ARRAY_SIZE % size));
      indices[i] = i ? indices[i - 1] + counts[i - 1] : 0;
      MPI_Send(x + indices[i], counts[i], MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(y + indices[i], counts[i], MPI_INT, i, 1, MPI_COMM_WORLD);
    }
    // printf("counts = ");
    // for (int i = 0; i < size; i++) {
    //   printf("%d ", counts[i]);
    // }
    // printf("\n");
    // printf("indices = ");
    // for (int i = 0; i < size; i++) {
    //   printf("%d ", indices[i]);
    // }
    // printf("\n");
  }
  int c = counts[rank];
  int *buf1 = new int[c], *buf2 = new int[c];
  int *buf3 = new int[c], *buf4 = new int[c];
  MPI_Recv(buf1, c, MPI_INT, 0, 0, MPI_COMM_WORLD, nullptr);
  MPI_Recv(buf2, c, MPI_INT, 0, 1, MPI_COMM_WORLD, nullptr);
  for (int i = 0; i < c; i++) {
    buf3[i] = buf1[i] + buf2[i];
    buf4[i] = buf1[i] * buf2[i];
  }
  MPI_Send(buf3, c, MPI_INT, 0, 2, MPI_COMM_WORLD);
  MPI_Send(buf4, c, MPI_INT, 0, 3, MPI_COMM_WORLD);
  if (!rank) {
    for (int i = 0; i < size; i++) {
      MPI_Recv(z1 + indices[i], counts[i], MPI_INT, i, 2, MPI_COMM_WORLD,
               nullptr);
      MPI_Recv(z2 + indices[i], counts[i], MPI_INT, i, 3, MPI_COMM_WORLD,
               nullptr);
    }
    printf("z1 = ");
    for (int i : z1) {
      printf("%d ", i);
    }
    printf("\n");
    printf("z2 = ");
    for (int i : z2) {
      printf("%d ", i);
    }
    printf("\n");
  }
  MPI_Finalize();
}
