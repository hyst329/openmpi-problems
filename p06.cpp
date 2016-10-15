#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size, rank;
  const int ARRAY_SIZE = 1000;
  int a[ARRAY_SIZE], b[ARRAY_SIZE];
  timeval start, end;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (size != 2) {
    printf("The program must be run on 2 processes (instead of %d)\n", size);
    exit(1);
  }
  if (!rank) {
    srand(time(0));
    printf("PingPong\n");
  }
  for (int s = 0; s <= ARRAY_SIZE; s += ARRAY_SIZE / 10) {
    if (!rank) {
      printf("Sending %i\n", s);
      for (int i = 0; i < s; i++) {
        a[i] = rand() % 1000;
      }
      gettimeofday(&start, nullptr);
      MPI_Send(a, s, MPI_INT, 1, s, MPI_COMM_WORLD);
      MPI_Recv(a, s, MPI_INT, 1, ARRAY_SIZE + s, MPI_COMM_WORLD, nullptr);
      gettimeofday(&end, nullptr);
      int64_t diff = (end.tv_sec - start.tv_sec) * 1000000LL +
                     (end.tv_usec - start.tv_usec);
      printf("Time in usec for %i: %lli\n", s, diff);
    } else {
      MPI_Recv(b, s, MPI_INT, 0, s, MPI_COMM_WORLD, nullptr);
      printf("Received %i\n", s);
      MPI_Send(b, s, MPI_INT, 0, ARRAY_SIZE + s, MPI_COMM_WORLD);
    }
  }
  MPI_Finalize();
}
