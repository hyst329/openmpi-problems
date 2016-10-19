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
  double a[ARRAY_SIZE], b[ARRAY_SIZE];
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
    printf("b = ");
    for (double &x : b) {
      x = rand() % 100;
      printf("%4.0f ", x);
    }
    printf("\n");
  }
  int c = counts[rank];
  double *d = new double[c], *e = new double[c];
  double p, prod;
  MPI_Scatterv(a, counts, indices, MPI_DOUBLE, d, c, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  MPI_Scatterv(b, counts, indices, MPI_DOUBLE, e, c, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  p = prod = 0;
  for (int i = 0; i < c; i++)
    p += d[i] * e[i];
  printf("Sum in process %2d = %5.3f\n", rank + 1, p);
  MPI_Reduce(&p, &prod, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (!rank)
    printf("prod = %7.4f\n", prod);
  MPI_Finalize();
}
