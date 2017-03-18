#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <math.h>
#include <mpi.h>
using namespace std;

int Rank(int, int, int*);
void mergesort(int*, int*, int, int, int, int, int);
void pmerge(int*, int*, int*, int, int, int);
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
  in  >> n;
  // read first line for length of letters in the list of random letters
  int * a = new int [n];  
  int * b = new int[n];               // creates an array that will hold all the random letters

  
  
  
  
  
  if (my_rank == 0) {                 // if you are process 0 then go down the list in the file and add them to the array.
    for (int x = 0; x < n; x++) {
       in >> a[x] ;
	   b[x]= 0;
	   
	   cout << "a[" << x << "]: " << a[x] << ", b[" << x << "]: " << b[x] << endl;
    }
    
	// bcast the entire array A to all processors
	MPI_Bcast(&a[0], n, MPI_INT, 0, MPI_COMM_WORLD);
	
 
  }
  
  if (my_rank == 1) {
	  for (int x=0; x<n; x++) {cout << "a[" << x << "]: " << a[x] << endl;}
  }
 

  mergesort(&a[0], &b[0], 0, n-1, n, my_rank, p);



  
  in.close();
  MPI_Finalize();
}



// sequential mergesort call
void mergesort(int * input, int * output, int first, int last, int size, int my_rank, int p) {
	int mid;
	int * rankArray= new int[size];
	for(int x=0;x<size;x++){
		rankArray[x]=0;
	}
	
  if (first < last) {
    mid= first+((last-first)/2);
    mergesort(&input[0],&rankArray[0], first, mid,size,my_rank,p);
    mergesort(&input[size/2],&rankArray[size/2], mid+1 , last, size, my_rank,p);
    pmerge(&rankArray[0],&rankArray[size/2],&output[0],size,my_rank,p); 

  } return;
}


void pmerge(int * L , int * R , int * sortedArray, int n, int my_rank, int p) 
{
	//cout << "This is totally working...?" << endl;
  int logOfHalf = log2(n/2);
  int sub_size= ceil((n/2)/(logOfHalf));
  int local_start = my_rank;    // striping
  

  int * srankA= new int[n/2];
  int * srankB= new int[n/2]; 
  int * totalsrankA = new int[n/2]; 
  int * totalsrankB = new int[n/2]; 
 
	for (int i =0;i<n/2;i++){
		
		srankA[i]= 0;
		srankB[i]= 0; 
		totalsrankA[i]= 0; 
		totalsrankB[i] = 0; 	
	}
  
	int counter = local_start; int posA; int posB;
	for (int x = local_start; x < sub_size; x+=p) {
	  counter = counter + x * logOfHalf;
	  
	  // srankA: Find position of R to search for its Rank within L
	  posA = x*logOfHalf;
	  srankA[x] = Rank(R[posA], n/2, L);      // R[x] in L and R[x] in R

	  // srankB: Find position of L to search for its Rank within R
	  posB = x*logOfHalf;
	  srankB[x] = Rank(L[posB], n/2, R);      // L[x] in R and R[x] in L
	  
	  
	}

	/* //if (my_rank=0) {
		cout << endl;
		for(int x = 0; x < n/2; x++) {
			cout << "my_rank: " << my_rank << ", srankA["<<x<<"]: " << srankA[x] << endl;
		}
	//} */
 
  //MPI_Allreduce(&srankA, &totalsrankA, n/2, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  //MPI_Allreduce(&srankB, &totalsrankB, n/2, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  // print srank arrays after reduction
  /* if (my_rank == 0) {
	cout << "this is the right array" << endl;
    for (int x = 0; x < sub_size; x++) {
      //cout << R[x] <<" Rank ";
	  cout << totalsrankA[x] << endl;
    }
	cout << endl;
	cout << "this is the left array" << endl;
    for (int x = 0; x < sub_size; x++){
      //cout << L[x*logOfHalf] <<" Rank ";
      cout << totalsrankB[x] << endl;
    }
  } */  
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
	
#include <math.h>
#include <mpi.h>
using namespace std;

int Rank(int, int, int*);
void mergesort(int*, int*, int, int, int, int, int);
void pmerge(int*, int*, int*, int, int, int);
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
  int * a = new int [n];  
  int * b = new int[n];               // creates an array that will hold all the random letters

  
  
  
  
  
  if (my_rank == 0) {                 // if you are process 0 then go down the list in the file and add them to the array.
    for (int x = 0; x < n; x++) {
       in >> a[x] ;
	   b[x]= 0;
    }
    
	// bcast the entire array A to all processors
	MPI_Bcast(&a[0] , n , MPI_INT ,0 ,MPI_COMM_WORLD);
	
 
  }
  
 

  mergesort(&a[0], &b[0], 0, n-1, n, my_rank, p);



  
  in.close();
  MPI_Finalize();
}



// sequential mergesort call
void mergesort(int * input, int * output, int first, int last, int size, int my_rank, int p) {
	int mid;
	int * rankArray= new int[size];
	for(int x=0;x<size;x++){
		rankArray[x]=0;
	}
	
  if (first < last) {
    mid= first+((last-first)/2);
    mergesort(&input[0],&rankArray[0], first, mid,size,my_rank,p);
    mergesort(&input[size/2],&rankArray[size/2], mid+1 , last, size, my_rank,p);
    pmerge(&rankArray[0],&rankArray[size/2],&output[0],size,my_rank,p); // This is where Pmerge goes

  } return;
}


void pmerge(int * L , int * R , int * sortedArray, int n, int my_rank, int p) 
{
  int logOfHalf = log2(n/2);
  int sub_size= ceil((n/2)/(logOfHalf));
  int local_start = my_rank;    // striping
  

  int * srankA= new int[sub_size];
  int * srankB= new int[sub_size]; 
  int * totalsrankA = new int[sub_size]; 
  int * totalsrankB = new int[sub_size]; 
 
for (int i =0;i<sub_size;i++){
	
	srankA[i]= 0;
	srankB[i]= 0; 
	totalsrankA[i]= 0; 
	totalsrankB[i] = 0; 	
}
  
   
	int counter = 0;
	int posA; int posB;
	for (int x = local_start; x < sub_size; x+=p) {
	  // srankA: Find position of R to search for its Rank within L
	  posA = 1+x*logOfHalf;
	  srankA[counter] = Rank(R[posA], n/2, L);      // R[x] in L and R[x] in R

	  // srankB: Find position of L to search for its Rank within R
	  posB = 1+x*logOfHalf;
	  srankB[counter] = Rank(L[posB], n/2, R);      // L[x] in R and R[x] in L
	  counter++;
	}


  
 
  MPI_Allreduce(&srankA, &totalsrankA, sub_size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&srankB, &totalsrankB, sub_size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  // print srank arrays after reduction
  if (my_rank == 0) {
	cout << "this is the right array" << endl;
    for (int x = 0; x < sub_size; x++) {
      cout << R[x*logOfHalf] <<" Rank ";
	  cout << totalsrankA[x] << endl;
    }
	cout << endl;
	cout << "this is the left array" << endl;
    for (int x = 0; x < sub_size; x++){
      cout << L[x*logOfHalf] <<" Rank ";
      cout << totalsrankB[x] << endl;
    }
  }

  
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
	
