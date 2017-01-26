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

	srand(123456);



	// create the dataset
	int n = 1000;
	char data[1000];

	for (size_t i = 0; i < n; i++) {
		char next = (char) (rand() % 26 + 97);
		data[i] = next;
	}

	ofstream outputf("letters.txt");
	if (!outputf) {
		cout << "Can't open file" << endl;
	} else {
		outputf.write((char *) data, sizeof(data));
		outputf.close();
	}


	// read text file here
  ifstream inputf("letters.txt");

  char letters[1000];
  inputf >> letters;

	int num_letters[26];

	if (my_rank == 0) {
    for (int x = 0; x < 26; x++) {
      num_letters[x] = 0;
    }
	}



	// divide the problem
	int local_n = n/p;
	char * local_a = new char[local_n];

	MPI_Scatter(&letters[0], local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);



	// local work
  int temp;
	int local_num_letters[26];

	for (size_t x = 0; x < local_n; x++) {
    temp = ((int) local_a[x]) - 97;
    local_num_letters[temp]++;
	}

	MPI_Reduce(&local_num_letters, &num_letters, 26, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);



	// Shut down MPI
	MPI_Finalize();


	return 0;
}
