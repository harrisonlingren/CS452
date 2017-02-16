#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int L[n1], R[n2];
    for (i = 0; i < n1; i++) { L[i] = arr[l + i]; }
    for (j = 0; j < n2; j++) { R[j] = arr[m + 1+ j]; }

    i = 0; j = 0; k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        } k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

void printA(int A[], int n) {
    cout << endl;
    for (int i = 0; i < n-1; i++) {
        cout << A[i] << ", ";
    } cout << A[n-1];
    cout << endl;
}

int main(int argc, char const *argv[]) {
    srand(time(0));

    cout << "Please input size of array to sort" << endl << ">  ";
    int size; cin >> size;
    int * A = new int[size];

    for (int i = 0; i < size; i++) {
        A[i] = (rand() % 1000000);
    }

    cout << "Initial array:";
    printA(A, size);

    mergeSort(A, 0, size - 1);

    cout << endl << "Sorted array:";
    printA(A, size);

    return 0;
}
