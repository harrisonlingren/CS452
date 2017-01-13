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
	char message[100];		// message itself
	MPI_Status status;		// return status for receive


	int lastEvenP;
	int lastOddP;

	if (p % 2 == 0) {
		lastEvenP = p - 2;
		lastOddP = p - 1;
	} else {
		lastEvenP = p - 1;
		lastOddP = p - 2;
	}

	cout << "p: " << p << ", lastOddP: " << lastOddP << ", lastEvenP: " << lastEvenP << endl;

	// Start MPI
	MPI_Init(&argc, &argv);

	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// THE REAL PROGRAM IS HERE

  // set up ring message
	char * ring_message = message;
	sprintf(message, "Ring path: ");
	if (my_rank % 2 == 0) {
		sprintf(ring_message + strlen(ring_message), "rank: %d, ring: One Ring;", my_rank);
	} else {
		sprintf(ring_message + strlen(ring_message), "rank: %d, ring: Two Ring;", my_rank);
	}

  if (my_rank == 0) {
		cout << "Starting for ring one, sending to " << 2 << "!" << endl;
		MPI_Send(ring_message, strlen(ring_message) + 1, MPI_CHAR, 2, tag, MPI_COMM_WORLD);
		MPI_Recv(ring_message, 100, MPI_CHAR, lastEvenP, tag, MPI_COMM_WORLD, &status);
		cout << ring_message << endl;

  } else if (my_rank == 1) {
		cout << "Starting for ring two, sending to " << lastOddP << "!" << endl;
    MPI_Send(ring_message, strlen(ring_message) + 1, MPI_CHAR, lastOddP, tag, MPI_COMM_WORLD);
    MPI_Recv(ring_message, 100, MPI_CHAR, 3, tag, MPI_COMM_WORLD, &status);
    cout << ring_message << endl;
	
	} else if (my_rank = lastEvenP) {
		MPI_Recv(ring_message, 100, MPI_CHAR, my_rank - 2, tag, MPI_COMM_WORLD, &status);
    sprintf(ring_message + strlen(ring_message), "%d, ", my_rank);
    cout << "Moving along the path, p rank: " << my_rank << "!" << endl;
    MPI_Send(ring_message, strlen(ring_message) + 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

	} else if (my_rank = lastOddP) {
		MPI_Recv(ring_message, 100, MPI_CHAR, my_rank + 2, tag, MPI_COMM_WORLD, &status);
    sprintf(ring_message + strlen(ring_message), "%d, ", my_rank);
    cout << "Moving along the path, p rank: " << my_rank << "!" << endl;
    MPI_Send(ring_message, strlen(ring_message) + 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);

	} else if ((my_rank % 2 == 0) && (my_rank != 0) && (my_rank != lastEvenP)) {
		MPI_Recv(ring_message, 100, MPI_CHAR, my_rank - 2, tag, MPI_COMM_WORLD, &status);
    sprintf(ring_message + strlen(ring_message), "%d, ", my_rank);
    cout << "Moving along the path, p rank: " << my_rank << "!" << endl;
    MPI_Send(ring_message, strlen(ring_message) + 1, MPI_CHAR, my_rank + 2, tag, MPI_COMM_WORLD);

	} else if (my_rank % 2 != 0) {
		MPI_Recv(ring_message, 100, MPI_CHAR, my_rank + 2, tag, MPI_COMM_WORLD, &status);
		sprintf(ring_message + strlen(ring_message), "%d, ", my_rank);
		cout << "Moving along the path, p rank: " << my_rank << "!" << endl;
		MPI_Send(ring_message, strlen(ring_message) + 1, MPI_CHAR, my_rank - 2, tag, MPI_COMM_WORLD);
	}


	// Shut down MPI
	MPI_Finalize();

	return 0;
}
