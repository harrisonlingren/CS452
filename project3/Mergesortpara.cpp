#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <math.h>
#include <mpi.h>
using namespace std;



void mergesort(int *, int , int );
void merge(int *, int , int , int );
int Rank(int, int , int *);
int log2(int);
void pmerge(int *,int*, int , int , int,int,int,int );


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
  in  >> n;                                             // read first line for l                                                                                                                     ength of letters in the list of random letters

  int * a = new int [n];                        // creates an array that will ho                                                                                                                     ld all the random letters



    if (my_rank == 0) {                 // if you are process 0 then go down the l                                                                                                                     ist in the file and add them to the array.
        for (int x = 0; x < n; x++) {
            in >> a[x] ;
        }



        /* if (my_rank == 0) {
                cout << " This is processor 0 array A"<<endl;     // Uncomment if need                                                                                                                      to test array is in A
                for (int x=0; x<n; x++) {
                        cout << a[x]<< endl;
                    }
            cout<<endl;
        } */
    }



    // bcast the entire array A to all processors
    MPI_Bcast(&a[0], n, MPI_INT, 0, MPI_COMM_WORLD);




  /* if (my_rank == 1) {
          cout<< endl;
          cout << " This is processor 1 array A"<<endl;                 // Uncom                                                                                                                     ment if needed to test bcast
          for (int x=0; x<n; x++) {
                  cout << a[x] << endl;
                }
  }
  */


    int *L;
    int *R;
    L = new int[32];
    R= new int [32];

    L = &a[0];
    R= &a[32];

    mergesort(L, 0, (n/2)-1);   // works  will look at later ***************                                                                                                                     *********************************************
    mergesort(R, 0, (n/2)-1);  // These 2 sort the array for us thats all I                                                                                                                      wanted out of them
    pmerge(L,R,0,n-1,n/2,n,my_rank,p);

    //mergesort(&a[0],0,n-1);   // does not work segmetation fault
    /*
        if (my_rank == 0){

        cout<< "This is the sorted array"<<endl;         // Use to test if L and                                                                                                                      R went though
                for (int x=0; x<n/2; x++) {
                        cout << L[x] << endl;
                }
    } */





  in.close();
  MPI_Finalize();
}


void pmerge(int * L,int * R, int first, int last, int mid,int size,int my_rank,int p){
    int logOfHalf = log2(size/2);
    int local_start= my_rank;
    int partition = ceil((double(size/2)/(logOfHalf)));

    int *srankA= new int[partition];
    int *srankB= new int[partition];
    int *totalsrankA= new int[partition];
    int *totalsrankB= new int[partition];

    //cout<< " This is the value of partition"<< partition << endl;      //                                                                                                                      uncomment if need to test partiton value

    for (int i =0;i < partition;i++){

            srankA[i]= 0;
            srankB[i]= 0;
            totalsrankA[i]=0;
            totalsrankB[i]=0;

    }

    /*
    if(my_rank==0){
    cout<< "this is srankA"<<endl;
            for (int i =0;i<partition;i++){

            cout<< srankA[i]<<endl;
            }
    } */




    for (int x = local_start; x < partition; x+=p) {
            srankA[x] = Rank(R[x*logOfHalf], size/2, L);
            srankB[x] =     Rank(L[x*logOfHalf], size/2, R);

    }


    /* if (my_rank == 3) {
    cout << "this is the processor 3's array A" << endl;            // Uncom                                                                                                                     ment to see what values should be in each processor
            for (int x = 0; x < partition; x++) {
            cout << srankA[x] << endl;

            }

    } */

    MPI_Allreduce(srankA, totalsrankA, partition, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(srankB, totalsrankB, partition, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


    if (my_rank == 0) {
        cout << "This is the ending array A" << endl;
        for (int x = 0; x < partition; x++) {
            cout << totalsrankA[x] <<" "<< x<< endl;

        }

    }

    if (my_rank == 0) {
        cout << "This is the ending array B" << endl;
        for (int x = 0; x < partition; x++) {
            cout << totalsrankB[x] <<" "<< x << endl;
        }
    }
    cout<<endl;
    int * endpointA = new int[2*partition];
    int * endpointB = new int[2*partition];

    for (int x = 0; x < partition; x++) {
        endpointA[x] = totalsrankB[x] * logOfHalf; 
        endpointB[x] = totalsrankA[x] * logOfHalf;
    }

    for (int x = 0; x < partition; x++) {
        endpointA[partition + x] = totalsrankA[x]; 
        endpointB[partition + x] = totalsrankB[x];
    }

    sort(endpointA, endpointA + 2 * partition);
    sort(endpointB, endpointB + 2 * partition);

	cout<< "this is endpointA"<< endl;
    for (int x = 0; x < 2* partition; x++) {
        //cout << "end A[" << x << "]: " << endpointA[x] << ", end B[" << x << "]: " << endpointB[x] << endl;
		cout << endpointA[x]<< endl;
    }
}





// returns log base 2 of x
int log2(int x) {
   return (log(x) / log(2) );
}

void mergesort(int * a, int first, int last)
{

    int mid;
    if (first < last)
    {
        mid= first+((last-first)/2);
        mergesort(a,first,mid);
        mergesort(a,mid+1,last);
        merge(a,first,last,mid);
    }
    return;

}
void merge(int * a, int first, int last, int mid)
{
    int i, j, k, c[50];
    i = first;
    k = first;
    j = mid + 1;
    while (i <= mid && j <= last)
    {
        if (a[i] < a[j])
        {
            c[k] = a[i];
            k++;
            i++;
        }
        else
        {
            c[k] = a[j];
            k++;
            j++;
        }
    }
    while (i <= mid)
    {
        c[k] = a[i];
        k++;
        i++;
    }
    while (j <= last)
    {
        c[k] = a[j];
        k++;
        j++;
    }
    for (i = first; i < k; i++)
    {
        a[i] = c[i];
    }
}

int Rank(int value, int size, int * array)
{

    if(size == 1){
            if(value < array[0]){

                    return 0;
            }
            else{
                    return 1;
            }

    }
    else{
        if(value < array[size/2]){
                return Rank(value,size/2,array) ;
        }
        else{
                return size/2 + Rank(value, size/2,&array[size/2]) ;

        }
    }
}
