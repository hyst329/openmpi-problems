#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct location {
  double v;
  int ind;
};

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
  int c = counts[rank], st = indices[rank];
  double *d = new double[c];
  location loc, gloc;
  MPI_Scatterv(a, counts, indices, MPI_DOUBLE, d, c, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  loc.v = d[0];
  loc.ind = 0;
  for (int i = 1; i < c; i++) {
    if (d[i] < loc.v) {
      loc.v = d[i];
      loc.ind = i;
    }
  }
  loc.ind += st;
  MPI_Reduce(&loc, &gloc, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
  if (!rank)
    printf("value = %7.4f index = %3d\n", gloc.v, gloc.ind);
  MPI_Finalize();
}
