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

	int alphabet[26];


	// Start MPI
	MPI_Init(&argc, &argv);

	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// THE REAL PROGRAM IS HERE

	// create the dataset
	// read text file here
  ifstream inputf;
  inputf.open("letters.txt");

  string letters;
  inputf >> letters;

	if (my_rank == 0) {
    for (int x = 0; x < 26; x++) {
      alphabet[x] = 0;
    }
	}

	// divide the problem
	int local_n = n/p;
	string * local_a = new string[local_n];

	MPI_Scatter(&letters[0], local_n, MPI_DOUBLE, local_a, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// local work
  int temp;

	for (size_t x = 0; x < local_n; x++) {
    temp = ((int) local_a[x]) - 97;
    alphabet[temp]++;
	}

	// Shut down MPI
	MPI_Finalize();

  cout << "Max: " << the_max << ", Min: " << the_min << ", Avg: " << the_avg << endl;

	return 0;
}
