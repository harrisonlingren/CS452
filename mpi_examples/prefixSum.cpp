#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
	
	srand(time(0));
	
	// KNOW THE PROBLEM
	int n = 300000;
	double a[300000], psum[300000] = {0};
	
	if (my_rank == 0)
		for (int x = 0; x < n; x++)
			a[x] = rand() % 8;
/*	
	psum[0] = a[0];
	for (int x = 1; x < n; x++)
		psum[x] = psum[x-1] + a[x];
*/

	// DIVIDE THE PROBLEM UP
	double * locala = new double[n/p];
	double * localpsum = new double[n/p];
	
	MPI_Scatter(&a[0], n/p, MPI_DOUBLE, locala, n/p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	// DO THE LOCAL WORK
	
	// Each process is going to calculate its "wrong" prefix sum array
	localpsum[0] = locala[0];
	for (int x = 1; x < n/p; x++)
		localpsum[x] = localpsum[x-1] + locala[x];
	
	// COMBINE THE LOCAL WORK TOGETHER
	
	// Send my last partial prefix sum answer to all LATER processors
	for (int x = my_rank + 1; x < p; x++)
		MPI_Send(&localpsum[n/p-1], 1, MPI_DOUBLE, x, tag, MPI_COMM_WORLD);
	
	// Receive all the answers I was sent
	double temp_no = 0;
	int messages_received = 0;
	double localprefixadditive = 0;
	while (messages_received < my_rank) {
		MPI_Recv(&temp_no, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
		localprefixadditive += temp_no;
		messages_received++;
	}
	
	// Add my localprefixadditive to every element of the local array
	for (int x = 0; x < n/p; x++)
		localpsum[x] += localprefixadditive;
	
	// Finally, collect all the partial local prefix sum arrays into one 
	// glorious answer array on process 0
	MPI_Gather(&localpsum[0], n/p, MPI_DOUBLE, psum, n/p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	// Shut down MPI
	MPI_Finalize();

	return 0;
}
