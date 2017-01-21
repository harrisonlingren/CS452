#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
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
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE

	srand(time(0));
/*
	int a[10], b[10];
	for (int x = 0; x < 10; x++) {
		a[x] = rand() % 10;
		b[x] = rand() % 10;
	}
	
	int dotprod = 0;
	for (int x = 0; x < 10; x++)
		dotprod += a[x]*b[x];
	cout << dotprod << endl;
*/

	// KNOW THE PROBLEM
	int a[300000], b[300000];
	int n = 300000;
	if (my_rank == 0) {
		for (int x = 0; x < n; x++) {
			a[x] = rand() % 8;
			b[x] = rand() % 8;
		}		
	}
	
	// DIVIDE THE PROBLEM UP
	int local_n = n/p;
	int * locala = new int[local_n];
	int * localb = new int[local_n];
	
	MPI_Scatter(&a[0], local_n, MPI_INT, locala, local_n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&b[0], local_n, MPI_INT, localb, local_n, MPI_INT, 0, MPI_COMM_WORLD);
	
	// After executing these lines of code, locala and localb all have the "right stuff"

	// DO THE LOCAL WORK
	double localdot = 0;
	for (int x = 0; x < local_n; x++)
		localdot += locala[x] * localb[x];
	
	// COLLECT THE RESULTS BACK IN THE OVERSEER
	double dotproduct = 0;
	for (int x = 0; x < p; x++)
		MPI_Reduce(&localdot, &dotproduct, 1, MPI_DOUBLE, MPI_SUM, x, MPI_COMM_WORLD);
	
	// Shut down MPI
	MPI_Finalize();

	return 0;
}