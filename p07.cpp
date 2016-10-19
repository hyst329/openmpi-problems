#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int ARRAY_SIZE = 50;
  double a[ARRAY_SIZE];
  int *indices = new int[size], *counts = new int[size];
  for (int i = 0; i < size; i++) {
    counts[i] = (ARRAY_SIZE / size) + (i < (ARRAY_SIZE % size));
    indices[i] = i ? indices[i - 1] + counts[i - 1] : 0;
  }
  srand(time(0));
  if (!rank) {
    printf("a = ");
    for (double &x : a) {
      x = rand() % 100;
      printf("%4.0f ", x);
    }
    printf("\n");
  }
  int c = counts[rank];
  double *d = new double[c];
  double n, norm;
  MPI_Scatterv(a, counts, indices, MPI_DOUBLE, d, c, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  n = norm = 0;
  for (int i = 0; i < c; i++)
    n += sqrt(d[i]);
  printf("Sum in process %2d = %5.3f\n", rank + 1, n);
  MPI_Reduce(&n, &norm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (!rank)
    printf("norm = %7.4f\n", norm);
  MPI_Finalize();
}
