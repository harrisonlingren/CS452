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

	// Start MPI
	MPI_Init(&argc, &argv);

	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// THE REAL PROGRAM IS HERE

  char * baton = message;
  sprintf(message, "Sent list: ");
  sprintf(baton + strlen(baton), "%d, ", my_rank);

  if (my_rank == 0) {

    cout << "Cool runnings around the track with no potato number: " << my_rank << "!" << endl;
    MPI_Send(baton, strlen(baton) + 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
    MPI_Recv(baton, 100, MPI_CHAR, p - 1, tag, MPI_COMM_WORLD, &status);
    cout << "Declare a provisional victory lel" << endl;
    cout << baton << endl;

  } else {

    MPI_Recv(baton, 100, MPI_CHAR, ((my_rank + p - 1) % p), tag, MPI_COMM_WORLD, &status);
    sprintf(baton + strlen(baton), "%d, ", my_rank);
    cout << "Cool runnings around the track with no potato number: " << my_rank << "!" << endl;
    MPI_Send(baton, strlen(baton) + 1, MPI_CHAR, ((my_rank + 1) % p), tag, MPI_COMM_WORLD);

  }

	// Shut down MPI
	MPI_Finalize();


	return 0;
}

