/*
  Harrison Lingren & Amy Street
  CS 452 Project 1
  3. In Your Eye
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
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
	//int source;				// rank of the sender
	//int dest;				// rank of destination
	//int tag = 0;			// message number
	//char message[100];		// message itself
	//MPI_Status status;		// return status for receive

	// Start MPI
	MPI_Init(&argc, &argv);

	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// THE REAL PROGRAM IS HERE

	int n = 1000;					// problem size
	char letters[1000];		// char array of all problem data
	int counts[256];	// int array to track amount of each letter

	// create the dataset
	if (my_rank == 0) {

		// create random data here
		char data[1000];
		srand(123456);
		for (size_t i = 0; i < n; i++) {
			char next = (char) (rand() % 26 + 97);
			data[i] = next;
		}

		// write the data to a text file here
		ofstream outputf("letters.txt");
		if (!outputf) {
			cout << "Can't open file" << endl;
		} else {
			outputf.write((char *) data, sizeof(data));
			outputf.close();
		}

		// read the text file here
	  ifstream inputf("letters.txt");
	  inputf >> letters;

		for (int x = 0; x < 26; x++) {
			counts[x] = 0;
		}
	}

	// divide the problem
	int local_n = n/p;
	char * local_a = new char[local_n];

	MPI_Scatter(&letters[0], local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);



	// local work
  int temp;
	int local_counts[256];

	for (size_t x = 0; x < 256; x++) {
		local_counts[x] = 0;
	}

	if (my_rank == 2) {cout << "my_rank: " << my_rank << " | chars: \n";}
	for (size_t x = 0; x < local_n; x++) {
		if (my_rank == 2) {cout << local_a[x];}
		temp = ((int) local_a[x]);
		if ( temp > 96 && temp < 123 ) {
			local_counts[temp]++;
		}
	}
	cout << endl;

	//MPI_Reduce(&local_counts, &counts, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	for (size_t x = 0; x < 256; x++) {
		MPI_Allreduce(&local_counts[x], &counts[x], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	}

	// Shut down MPI
	MPI_Finalize();

	return 0;
}
