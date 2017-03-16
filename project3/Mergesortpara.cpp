#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <math.h>
#include <mpi.h>
using namespace std;

int Rank(int, int, int*);
void merge(int*,int, int, int);
void mergesort(int*, int, int);
int log2(int);

int main (int argc, char * argv[]) {

  int my_rank;        // my CPU number for this process
  int p;              // number of CPUs that we have
  int source;         // rank of the sender
  int dest;           // rank of destination
  int tag = 0;        // message number
  MPI_Status status;  // return status for receive

  // Start MPI
  MPI_Init(&argc, &argv);

  // Find out my rank!
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  // Find out the number of processes!
  MPI_Comm_size(MPI_COMM_WORLD, &p);


  /* PROGRAM STARTS HERE */

  ifstream in;                        // if stream to start pulling in file
  in.open("mergepara.txt");

  int n;                              // problem size
  in >> n;                            // read first line for length of letters in the list of random letters
  int * a = new int [n];              // creates an array that will hold all the random letters

  if (my_rank == 0) {                 // if you are process 0 then go down the list in the file and add them to the array.
    for (int x = 0; x < n; x++) {
       in >> a[x] ;
    }
    
    /* for (int x = 0; x < n; x++) {  // print array A in process 0
      cout << a[i] << endl;
    } */
  }
  
  // bcast the entire array A to all processors
  MPI_Bcast(&a[0] , n , MPI_INT ,0 ,MPI_COMM_WORLD);

  // create left and right arrays for array A
  int *L;
  int *R;
  L = new int[32];   L = &a[0];
  R = new int [32];  R = &a[32];

  int srankA[32]= {0};
  int srankB[32]= {0};

  int logOfHalf = log2(n/2);

  int local_start = my_rank;
  for (int x = local_start; x < n/2; x = x +(p * logOfHalf) ) {
    srankA[x] = Rank(R[x], 32, L);      // R[x] in L and R[x] in R
    srankB[x] = Rank(L[x], 32, R);      // L[x] in R and R[x] in L

    /*
    for (int x = local_start; x < ((n/2) / logOfHalf); x += p){
      cout<< L[x] <<" Rank " << srankB[d] << endl;
    } cout << endl;

    for (int x = local_start; x < ((n/2) / logOfHalf); x += p){
      cout<< R[x] <<" Rank " << srankA[x] << endl;
    }
    */
  }

  // create arrays for reduction
  int totalsrankA[32];
  int totalsrankB[32];

  MPI_Allreduce(&srankA, &totalsrankA, 32, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&srankB, &totalsrankB, 32, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  // print srank arrays after reduction
  if (my_rank == 0) {
    for (int x = 0; x< 32; x++) {
      cout << R[x] <<" Rank " << totalsrankA[x] << endl;
    }

    for (int x = 0; x < 32; x++){
      cout << L[x] <<" Rank " << totalsrankB[x] << endl;
    }
  }

  /*
  // check to see if array A is in process 1
  if (my_rank == 1) {
    for (int x = 0; x < n; x++){
      cout << a[x] << endl;
    }
  }

  //  print out alias commented out once you verify they are correct
  cout<< "array A"<< endl;
  for (int x = 0; x < n/2; x++) {
    cout << L[x] << endl;
  }

  cout << endl; << "array B" << endl;
  for (int x = 0; x < n/2; x++) {
    cout << R[x] << endl;
  }
  */

  // Mergesort the left and right to sort
   mergesort(L, 0, (n/2) - 1);
   mergesort(R, 0, (n/2) - 1);

  // Create a dummy array to hold ranks of A and B and fill them
  // WE MOVED CODE FROM HERE

  /*  // print array A in process 0
  for (int i=0; i<n; i++) {
    cout << A[i] << endl;
  }*/

  in.close();
  MPI_Finalize();
}


// returns log base 2 of x
int log2(int x) {
  //cout << (log(x)/log(2)) << endl;
  return ( log(x) / log(2) );
}

// recursive Rank function (binary search)
// returns the rank for any given index on an array
int Rank(int searchItem, int size, int * array) {
  // base case
  if (size == 1){
    // checks midpoint, returns either 0 or 1 if in 1st or 2nd half
    if (searchItem < array[0]) { return 0; }
    else { return 1; }

  } else {
    // we must go deeper!
    if (searchItem < array[size/2]) {
      return Rank(searchItem, (size/2) ,array);

    // we're just about finished...
    } else {
      return size/2 + Rank(searchItem, (size/2), &array[size/2]);
    }
  }
}

// sequential mergesort call
void mergesort(int * a, int first, int last) {
  int mid;
  if (first < last) {
    mid = ((first + last) / 2);

    mergesort(a, first, mid);
    mergesort(a, (mid+1), last);
    merge(a, first, last, mid); // This is where Pmerge goes

  } return;
}

//recursive merge function
void merge(int * a, int first, int last, int mid) {
  int i, j, k, c[50];
  i = first;
  k = first;
  j = mid + 1;
  while (i <= mid && j <= last) {
    if (a[i] < a[j]) {
      c[k] = a[i];
      k++; i++;
    } else {
      c[k] = a[j];
      k++; j++;
    }
  }

  while (i <= mid) {
    c[k] = a[i];
    k++; i++;
  }

  while (j <= last) {
    c[k] = a[j];
    k++; j++;
  }

  for (i = first; i < k; i++) {
    a[i] = c[i];
  }
}
