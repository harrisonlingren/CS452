#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;

int Rank(int,int,int*);
void merge(int * a,int first, int last , int mid );
void mergesort(int * a, int first, int last);


int main() {
	int n;
	ifstream in;                       // if stream to start pulling in file
	in.open("Input.txt");
	in >> n;

	int a[n];

    for (int i = 0; i < n; i++) {
      in >> a[i];
			// cout<<a[j]<< endl;
    }

	//cout << Rank(7,n,a) <<endl;
	int *l; int *r;
	l = new int[32];
	r = new int[32];

  l = &a[0];
	r = &a[32];

	/*
	for (int z = 0; z < n/2; z++) {
  	cout<<l[z]<< endl;
  }

	for (int k = 0; k < n/2; k++) {
    cout<<r[k]<< endl;
  }
	*/

	mergesort(l, 0, (n/2)-1);

	int t[32];
	for( int p=0; p < n/2 ; p++){
		t[p]= Rank(p,32,l);
	}

	for (int z = 0; z < n/2; z++) {
  	cout<< l[z] << " Rank " << t[z] << endl;
  }

 	in.close();
}


void mergesort(int * a, int first, int last) {
    int mid;
    if (first < last)
    {
        mid=(first+last)/2;
        mergesort(a,first,mid);
        mergesort(a,mid+1,last);
        merge(a,first,last,mid);
    }
    return;
}

void merge(int * a, int first, int last, int mid) {
	int i, j, k, c[50];
	i = first;
	k = first;
	j = mid + 1;

	while (i <= mid && j <= last) {
    if (a[i] < a[j]) {
      c[k] = a[i];
      k++;
      i++;
    } else {
      c[k] = a[j];
      k++;
      j++;
    }
	}

	while (i <= mid) {
    c[k] = a[i];
    k++;
    i++;
	}

	while (j <= last) {
	  c[k] = a[j];
	  k++;
	  j++;
	}

	for (i = first; i < k; i++) {
	  a[i] = c[i];
	}
}

int Rank(int value, int size, int * array) {
	if(size == 1) {
		if(value < array[0]) {
			return 0;
		}	else {
			return 1;
		}

	}	else {
		if (value < array[size/2]) {
			return Rank(value,size/2,array);
		} else {
			return size/2 + Rank(value, size/2,&array[size/2]);
		}
	}
}