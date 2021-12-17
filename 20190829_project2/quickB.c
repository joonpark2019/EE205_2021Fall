
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define MAXLEN 10000000


//functions for testing:
/****************************************************************************/
char compare_arrays(int* arr1, int* arr2, int len)
{
    for(long i = 0; i < len; i++)
    {
        if(arr1[i] != arr2[i]){
            return 1;
        }
    }
    return 0;
}

/*function which takes array arr as input and prints all elements
from indices n1 to n2 to stdout */
void print_out(int* arr, long n1, long n2)
{
    for(long i = n1; i <= n2; i++)
        fprintf(stdout, "%d\n", arr[i]);
    //fprintf(stdout, "%c", '\n');
    return;
}

/****************************************************************************/

//comparison function used as input when calling qsort()
int compare (const void * A, const void * B) {
    //const int *A = a; const int *B = a;
    int el1 = *(int*)A;  int el2 = *(int*)B;
   return ( el1 - el2 );
}

//function swaps elements at indices n1 and n2 in array arr
void swap(int* arr, int n1, int n2)
{
    int temp = arr[n1]; arr[n1] = arr[n2]; arr[n2] = temp;
    return;
}

//Implementation of the median of three method:
/*The function compares the elements at n1, n2, and the middle (n1+n2/2)
, swapping until the median of those three is placed at the middle index.
It returns the middle index as the index of the pivot*/
int MedThree(int* arr, long n1, long n2)
{
    long mid = (n2 + n1)/2;
    if(arr[mid] < arr[n1])
        swap(arr, mid, n1);
    if(arr[n2] < arr[mid])
        swap(arr, mid, n2);
    if(arr[mid] < arr[n1])
        swap(arr, mid, n1);
    return mid;
}

//Implementation of ordinary insertion sort function:
/*Insertion Sort is used in the base case of the optimized quicksort for 
enhanced performance against qsort(). For arrays of small length,
insertion sort is faster than quicksort as it has no recursive calls*/
void InsertionSort(int* arr, long len)
{
    long i, j; int key;
    for (i = 1;  i < len; i++)
    {
        key = arr[i];
        j = i - 1;

        for(; j >= 0 && arr[j] > key;j--)
        {
            //shifting all elements upward
            arr[j+1] = arr[j];
        }
        arr[j+1] = key;
    }
    return;
}

//function for partitioning: utilizes the median-of-three approach
long partition(int* arr, long n1, long n2){

    //put median of three in the middle index
    long mid = MedThree(arr, n1, n2);
    //swap the pivot with the n1+1th element
    swap(arr, n1+1, mid);
    //store value of pivot in variable pivot
    long pivot = arr[n1+1];

//perform partition exclusing the pivot and excluding the element at n2
// we do this because we know from the median-of-three that 
// the element at n2 is larger than the pivot

/* Optimized Partition method: use 2 indices l, h at the beginning and end of the
array respectively (excluding last element and the pivot at index n1+1). 
In each iteration, increment l until arr[l] is larger than or equal to the pivot.
At the same time, decrement h until arr[h] is smaller than the pivot.
Then, swap the elements at l and h so that all elements at indices 
less than l are lower than the pivot and all elements at indices 
greater than h are greater than the pivot.

The break condition is when l gets larger than h.
*/
    long l = n1+2; long h = n2-1;

    while (1)
    {
        while(pivot >= arr[l] && l <= n2-1)
            l++;
        while(pivot < arr[h] && h >= (n1+2))
            h--;
        if(l >= h) break;
        swap(arr, h, l);
    }
    //swap element at h with pivot
    swap(arr, h, n1+1);
    return h;


}

//sorts array pointed to by arr between indices n1 and n2 inclusive
void quicksort(int* arr, long n1, long n2)
{
    //optimization: when array length is less than 20, use InsertionSort
    if( (n2-n1+1) <= 20 )
    {
        InsertionSort(arr+n1, n2-n1+1);
        return;
    }
    
    //p: index of the pivot in the array partitioned from n1 to n2
    long p = partition(arr, n1, n2);
    //recursively sort first and second half of the array in place
    // excluding the pivot
    quicksort(arr, n1, p - 1);
    quicksort(arr, p+1, n2);

}

//use built in qsort in base case of 5 element array
//cutoff and median of 3 approach:

int main(int argc, char* argv[])
{

    int* arr = (int *)calloc(MAXLEN, sizeof(int));
    //int* arr2 = (int *)calloc(MAXLEN, sizeof(int));

    struct timespec start_time, end_time;

/* Read integers as strings from input. 
NOTE: this assumes that input integer-representations are in 
different lines*/
    char input[11]; int elem;
    int len = 0;
    while(scanf("%s\n", input) != EOF){
        elem = atoi(input);
        arr[len] = elem; 
        len++;
    }

    char* argv_0 = argv[1];
    if(strcmp("mine", argv_0) == 0){
        
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        quicksort(arr, 0, len-1);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        
        double time = (end_time.tv_sec - start_time.tv_sec)*1000000000 + 
                                (end_time.tv_nsec - start_time.tv_nsec);
        fprintf(stderr, "It took %.3lf seconds\n", time/1000000000);
        print_out(arr, 0, len-1);
    }

    else if(strcmp("crt", argv_0)==0) {
        
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        qsort(arr, (size_t)len, sizeof(int), compare);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        
        double time = (end_time.tv_sec - start_time.tv_sec)*1000000000 + 
                        (end_time.tv_nsec - start_time.tv_nsec);
        fprintf(stderr, "It took %.3lf seconds\n", time/1000000000);
        print_out(arr, 0, len-1);
    }

    free(arr); 

    return 0;
}