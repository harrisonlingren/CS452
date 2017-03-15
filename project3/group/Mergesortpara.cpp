#include <iostream>
#include <stdio.h>
#include <string.h>
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
	int n;
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;			// message number
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
	
		
	ifstream in;                       // if stream to start pulling in file
	in.open("Input.txt");	
	
							// Size of the array
	in >> n;						// read first line for length of letters in the list of random letters
	//cout << n << endl;
	
	int * a = new int [n];		;			// creates an array that will hold all the random letters
	
	if (my_rank == 0) {                 // if you are process 0 then go down the list in the file and add them to the array.
		for (int x = 0; x < n; x++) {
			 in >> a[x] ;	
		}
				
	}
	if (my_rank == 0) { 
		for(int i=0; i<n;i++){
			cout << a[i] << endl;
		}
	}
	
	 
	MPI_Bcast(&a[0] , n , MPI_INT ,0 ,MPI_COMM_WORLD);
	
	
	
	
	if (my_rank == 1) { 
		for(int z=0; z < n;z++){
			cout << a[z] << endl;
		}
	}
	
	
	
	
	
	
	
	
	in.close();
	
	MPI_Finalize();
}