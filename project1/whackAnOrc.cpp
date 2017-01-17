#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h" // message passing interface
using namespace std;

// Do this ONCE when you start up thomas
// module load openmpi-x86_64

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -np 32 blah

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;			// message number
	char message[100];		// message itself
	MPI_Status status;		// return status for receive

  srand(1251);

	// Start MPI
	MPI_Init(&argc, &argv);

	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// THE REAL PROGRAM IS HERE

  // 0. Non-parallel solution:
  // double sum = 0;
  // for (int i = 0; i < 300000; i++) {
  //   sum += i;
  // }

  // 1. know the problem
  int a[1000];
  int n = (sizeof(a)/sizeof(*a));

  for (int x = 0; x < n; x++) {
    a[x] = rand();
  }

  // 2. Break the problem down
  double the_max = 0;
  double the_min = 0;
  double the_avg = 0;
  int local_start = my_rank;

  // 3. Perform local work
  double local_max = 0;
  double local_min = 0;
  double local_avg = 0;
  for (int x = local_start; x < n; x += p) {
    if (a[x] > local_max) {
      local_max = a[x];
    }

    if (a[x] < local_min) {
      local_min = a[x];
    }
  }


  // 4. Combine local results back together again
  // set overseer process to p0
  if (my_rank != 0) {
    double results[3];
    results[0] = local_max; results[1] = local_min; results[2] = local_avg;
    MPI_Send(&results, 3, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);

  } else {
    the_max = local_max;
    the_min = local_min;

    double temp;
    for (int x = 1; x < p; x++) {
      // get each set of results and parse
      MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);

      // max
      int max_index = (int) temp[0];
      if (a[max_index] > the_max) {
        the_max = a[max_index];
      }

      // min
      int min_index = (int) temp[1];
      if (a[min_index] > a[the_max]) {
        the_max = a[min_index];
      }

      the_avg += ((double) temp[2] / n);
    }
  }

	// Shut down MPI
	MPI_Finalize();

  cout << "Max: " << the_max << ", Min: " << the_min << ", Avg: " << the_avg << endl;

	return 0;
}
