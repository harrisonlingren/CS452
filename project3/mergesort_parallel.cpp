#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <math.h>
#include <mpi.h>
#include <algorithm>
#include <iomanip>
using namespace std;

void mergesort(int*, int, int, int);
void merge(int*, int, int, int);
int Rank(int, int, int*);
int log2(int);
void pmerge(int*, int*, int, int, int, int*);
void smerge(int*, int, int, int*, int, int, int*, int, int);

int main(int argc, char* argv[]) {
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

  // if stream to start pulling in file
  ifstream in;
  in.open("input.txt");

  int n;  // problem size
  in >>
      n;  // read first line for length of letters in the list of random letters

  int* a =
      new int[n];  // creates an array that will hold all the random letters

  if (my_rank == 0) {  // if you are process 0 then go down the list in the file
                       // and add them to the array.
    for (int x = 0; x < n; x++) {
      in >> a[x];
    }
  }

  // bcast the entire array A to all processors
  MPI_Bcast(&a[0], n, MPI_INT, 0, MPI_COMM_WORLD);

  mergesort(a, n, my_rank, p);

  if (my_rank == 0) {
    cout << "This is the sorted array" << endl;
    for (int x = 0; x < n; x++) {
      cout << a[x] << endl;
    }
  }

  in.close();
  MPI_Finalize();
}

void pmerge(int* L, int* R, int size, int my_rank, int p, int* a) {
  int logOfHalf = log2(size / 2);
  int local_start = my_rank;
  int partition = ceil((double(size / 2) / (logOfHalf)));

  int* srankA = new int[partition];
  int* srankB = new int[partition];
  int* totalsrankA = new int[partition];
  int* totalsrankB = new int[partition];

  // cout<< " This is the value of partition"<< partition << endl;
  // uncomment if need to test partiton value

  for (int i = 0; i < partition; i++) {
    srankA[i] = 0;
    srankB[i] = 0;
    totalsrankA[i] = 0;
    totalsrankB[i] = 0;
  }

  if (my_rank == 0) {
    cout << "this is L array" << endl;
    for (int i = 0; i < size / 2; i++) {
      cout << L[i] << endl;
    }
  }

  if (my_rank == 0) {
    cout << "this is R array" << endl;
    for (int i = 0; i < size / 2; i++) {
      cout << R[i] << endl;
    }
  }

  for (int x = local_start; x < partition; x += p) {
    srankA[x] = Rank(L[x * logOfHalf], size / 2, R);
    srankB[x] = Rank(R[x * logOfHalf], size / 2, L);
  }

  MPI_Allreduce(srankA, totalsrankA, partition, MPI_INT, MPI_SUM,
                MPI_COMM_WORLD);
  MPI_Allreduce(srankB, totalsrankB, partition, MPI_INT, MPI_SUM,
                MPI_COMM_WORLD);

  if (my_rank == 0) {
    cout << "This is the ending array A" << endl;
    for (int x = 0; x < partition; x++) {
      cout << totalsrankA[x] << endl;
    }
  }

  if (my_rank == 0) {
    cout << "This is the ending array B" << endl;
    for (int x = 0; x < partition; x++) {
      cout << totalsrankB[x] << endl;
    }
  }

  int* endpointA = new int[2 * partition];
  int* endpointB = new int[2 * partition];

  for (int x = 0; x < partition; x++) {
    endpointA[x] = x * logOfHalf;
    endpointB[x] = x * logOfHalf;
  }

  for (int x = 0; x < partition; x++) {
    endpointA[partition + x] = totalsrankB[x];
    endpointB[partition + x] = totalsrankA[x];
  }

  sort(endpointA, endpointA + 2 * partition);
  sort(endpointB, endpointB + 2 * partition);

  if (my_rank == 0) {
    cout << "this is endpointA" << endl;
    for (int x = 0; x < 2 * partition; x++) {
      cout << endpointA[x] << endl;
    }
  }

  if (my_rank == 0) {
    cout << "this is endpointB" << endl;
    for (int x = 0; x < 2 * partition; x++) {
      cout << endpointB[x] << endl;
    }
  }

  int* localOut = new int[size];
  int* WIN = new int[size];
  for (int x = 0; x < size; x++) {
    localOut[x] = 0;
    WIN[x] = 0;
  }

  for (int x = local_start; x < 2 * partition - 1; x += p) {
    if (my_rank == 0) {
      cout << "A: ";
      for (int y = endpointA[x]; y <= endpointA[x + 1] - 1; y++)
        cout << L[y] << " ";
      cout << endl;
      cout << "B: ";
      for (int y = endpointB[x]; y <= endpointB[x + 1] - 1; y++)
        cout << R[y] << " ";
      cout << endl;
    }

    smerge(L, endpointA[x], endpointA[x + 1] - 1, R, endpointB[x],
           endpointB[x + 1] - 1, localOut, endpointA[x] + endpointB[x],
           endpointA[x + 1] + endpointB[x + 1]);

    if (my_rank == 0) {
      cout << "This is the sorted A for " << my_rank << " when x is " << x
           << endl;
      for (int x = 0; x < size; x++) {
        cout << setw(3) << localOut[x] << " ";
      }
      cout << endl << endl;
    }
  }

  if (my_rank == 0) {
    cout << "A: ";
    for (int y = endpointA[2 * partition - 1]; y <= size / 2 - 1; y++)
      cout << L[y] << " ";
    cout << endl;
    cout << "B: ";
    for (int y = endpointB[2 * partition - 1]; y <= size / 2 - 1; y++)
      cout << R[y] << " ";
    cout << endl;

    smerge(L, endpointA[2 * partition - 1], size / 2 - 1, R,
           endpointB[2 * partition - 1], size / 2 - 1, localOut,
           endpointA[2 * partition - 1] + endpointB[2 * partition - 1], size);
  }

  MPI_Allreduce(localOut, WIN, size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  for (int x = 0; x < size; x++) {
    a[x] = WIN[x];
  }
}

// returns log base 2 of x
int log2(int x) { return (log(x) / log(2)); }

void mergesort(int* a, int n, int my_rank, int p) {
  int mid;
  if (n == 32) {
    sort(a, a + n);
  } else {
    mid = n / 2;
    mergesort(&a[0], n / 2, my_rank, p);
    mergesort(&a[n / 2], n / 2, my_rank, p);
    pmerge(&a[0], &a[n / 2], n, my_rank, p, a);
  }
  return;
}

void smerge(int* a, int a0, int a1, int* b, int b0, int b1, int* c, int c0,
            int c1) {
  int i = a0;
  int j = b0;
  int k = c0;

  while (i <= a1 && j <= b1)
    if (a[i] < b[j])
      c[k++] = a[i++];
    else
      c[k++] = b[j++];

  while (i <= a1) c[k++] = a[i++];
  while (j <= b1) c[k++] = b[j++];
}

int Rank(int value, int size, int* array) {
  if (size == 1) {
    if (value < array[0]) {
      return 0;
    } else {
      return 1;
    }
  } else {
    if (value < array[size / 2]) {
      return Rank(value, size / 2, array);
    } else {
      return size / 2 + Rank(value, size / 2, &array[size / 2]);
    }
  }
}
