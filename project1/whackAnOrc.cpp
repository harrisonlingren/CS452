#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> // message passing interface
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

	double the_max = 0; double the_min = 0; double the_avg = 0;

	// Start MPI
	MPI_Init(&argc, &argv);

	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// THE REAL PROGRAM IS HERE

	// create the dataset
	double a[20000];
  int n = 20000;
	cout << "Size: " << n << endl;

	if (my_rank == 0) {
	  for (int x = 0; x < n; x++) {
	    a[x] = rand() % 100;
	  }
	}

	// divide the problem
	int local_n = n / p;
	double * local_a = new double[local_n];

	MPI_Scatter(&a[0], local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// local work
	double local_max = 0;
	double local_min = 0;
	double local_avg = 0;

	for (size_t x = 0; x < local_n; x++) {
		if (local_a[x] > local_max) {
			local_max = local_a[x];
		}
		if (local_a[x] < local_min) {
			local_min = local_a[x];
		}
		local_avg += (local_a[x] / n);
	}

	// Collect results and reduce
	MPI_Allreduce(&local_max, &the_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
	MPI_Allreduce(&local_min, &the_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
	MPI_Allreduce(&local_avg, &the_avg, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	// Shut down MPI
	MPI_Finalize();

	the_avg = the_avg / p;
  cout << "Max: " << the_max << ", Min: " << the_min << ", Avg: " << the_avg << endl;

	return 0;
}
