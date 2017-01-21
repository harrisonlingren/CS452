/*
  Harrison Lingren & Amy Street
  CS 452 Project 1
  1. Two runnings
*/

#include <iostream>
#include <stdio.h>
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
	char message1[200];
	char message2[200];		// message itself
	MPI_Status status;		// return status for receive

	// Start MPI
	MPI_Init(&argc, &argv);

	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// THE REAL PROGRAM IS HERE

  // set up ring message
	char* one_message = message1;
	char* two_message = message2;
	sprintf(message1, "One ring: ");
	sprintf(message2, "Two ring: ");

  int lastEvenP;
	int lastOddP;

	if (p % 2 == 0) {
		lastEvenP = p - 2;
		lastOddP = p - 1;
	} else {
		lastEvenP = p - 1;
		lastOddP = p - 2;
	}


  if (my_rank == 0) {
		sprintf(one_message + strlen(one_message), "%d, ", my_rank);
    MPI_Send(one_message, strlen(one_message) + 1, MPI_CHAR, 2, tag, MPI_COMM_WORLD);
    MPI_Recv(one_message, 200, MPI_CHAR, lastEvenP, tag, MPI_COMM_WORLD, &status);
		sprintf(one_message + strlen(one_message), "0");

  } else if (my_rank == 1) {
		sprintf(two_message + strlen(two_message), "%d, ", my_rank);
    MPI_Send(two_message, strlen(two_message) + 1, MPI_CHAR, lastOddP, tag, MPI_COMM_WORLD);
    MPI_Recv(two_message, 200, MPI_CHAR, 3, tag, MPI_COMM_WORLD, &status);
		sprintf(two_message + strlen(two_message), "1");

  } else if (my_rank == lastOddP) {
    MPI_Recv(two_message, 200, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);
		sprintf(two_message + strlen(two_message), "%d, ", my_rank);
    MPI_Send(two_message, strlen(two_message) + 1, MPI_CHAR, (my_rank - 2), tag, MPI_COMM_WORLD);

  } else if (my_rank == lastEvenP) {
    MPI_Recv(one_message, 200, MPI_CHAR, (lastEvenP - 2), tag, MPI_COMM_WORLD, &status);
		sprintf(one_message + strlen(one_message), "%d, ", my_rank);
    MPI_Send(one_message, strlen(one_message) + 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

  } else if (my_rank % 2 == 0) {
    MPI_Recv(one_message, 200, MPI_CHAR, (my_rank - 2), tag, MPI_COMM_WORLD, &status);
		sprintf(one_message + strlen(one_message), "%d, ", my_rank);
    MPI_Send(one_message, strlen(one_message) + 1, MPI_CHAR, (my_rank + 2), tag, MPI_COMM_WORLD);

  } else if (my_rank % 2 != 0) {
    MPI_Recv(two_message, 200, MPI_CHAR, (my_rank + 2), tag, MPI_COMM_WORLD, &status);
		sprintf(two_message + strlen(two_message), "%d, ", my_rank);
    MPI_Send(two_message, strlen(two_message) + 1, MPI_CHAR, (my_rank - 2), tag, MPI_COMM_WORLD);
  }

	// if (my_rank % 2 == 0) {
	// 	sprintf(one_message + strlen(one_message), "%d, ", my_rank);
	// } else {
	// 	sprintf(two_message + strlen(two_message), "%d, ", my_rank);
	// }

	if (my_rank == 0) {
		cout << one_message << endl;
	}

	if (my_rank == 1) {
		cout << two_message << endl;
	}

	// Shut down MPI
	MPI_Finalize();
	//
	// cout << one_message << endl;
	// cout << two_message << endl;

	return 0;
}
