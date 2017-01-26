/*
	Harrison Lingren & Amy Street
	CS 452 Project 1
	3. In Your Eye
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <mpi.h>
using namespace std;

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;						// number of CPUs that we have

	// Start MPI
	MPI_Init(&argc, &argv);

	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// THE REAL PROGRAM IS HERE

	int n = 10000;					// problem size
	char letters[10000];		// char array of all problem data
	int counts[26];	// int array to track amount of each letter

	// create the dataset
	if (my_rank == 0) {

		// create random data here
		char data[10000];
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

		for (size_t x = 0; x < 26; x++) {
			counts[x] = 0;
		}
	}

	// divide the problem
	int local_n = n/p;
	char * local_a = new char[local_n];

	MPI_Scatter(&letters[0], local_n, MPI_CHAR, local_a, local_n, MPI_CHAR, 0, MPI_COMM_WORLD);

	// local work
	int temp;
	int local_counts[26];

	for (size_t x = 0; x < 26; x++) {
		local_counts[x] = 0;
	}

	for (size_t x = 0; x < local_n; x++) {
		temp = ((int) local_a[x]);
		if ( temp >= 0 && temp < 27 ) {
			local_counts[temp]++;
		}
	}

	for (size_t x = 0; x < 26; x++) {
		MPI_Reduce(&local_counts[x], &counts[x], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	}

	// answer
	if (my_rank == 0) {
		for (size_t i = 0; i < 26; i++) {
			cout << (char) (i+97) << ": " << counts[i] << endl;
		}
	}

	// Shut down MPI
	MPI_Finalize();

	return 0;
}
