
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define MAXLEN 10000000


//code for testing:
char compare_arrays(int* arr1, int* arr2, int len)
{
    for(int i = 0; i < len; i++)
    {
        if(arr1[i] != arr2[i]){
            return 1;
        }
    }
    return 0;
}

void print(int* arr, int n1, int n2)
{
    for(int i = n1; i <= n2; i++)
        printf("%d ", arr[i]);
    printf("%c ", '\n');
    return;
}

int compare (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void swap(int* arr, int n1, int n2)
{
    int temp = arr[n1]; arr[n1] = arr[n2]; arr[n2] = temp;
    return;
}

int MedThree(int* arr, int n1, int n2)
{
    int mid = (n1 + n2)/2;
    if(arr[mid] < arr[n1])
        swap(arr, mid, n1);
    if(arr[n2] < arr[mid])
        swap(arr, mid, n2);
    if(arr[mid] < arr[n1])
        swap(arr, mid, n1);
    return mid;
}

void InsertionSort(int* arr, int len)
{
    int i, key, j;
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

int partition(int* arr, int n1, int n2){
    //int i, j;
    //make sure pivot is at first element:
    
    //put median of first, last, and middle element in the middle index
    int mid = MedThree(arr, n1, n2);
    swap(arr, n1+1, mid);
    int pivot = arr[n1+1];

    
//make sure pivot is at the beginning: swap until index n2 - 1

    int l, m = n1+1; int h = n2 -1;
    while (m <= h)
    {
        if(arr[m] < pivot){
            swap(arr, l, m);
            l++; m++;
        }
        else if(arr[h] > pivot){
            swap(arr, m, h);
            h--;
        }
        else{ //if element equals array;
            m++;
        }
    }

    return m;


    //n1 + 1 + 1 since we exclude the first element as well as the pivot
    /*
    i = n1 + 1 + 1;
    for(j = n1 + 1 + 1; j < n2; j++){ // use j < n2 so j goes up to n2 - 1 instead of n2 since we exclude last element
        if(arr[j] < pivot){
            swap(arr, j, i);
            i++;
        }
    }
    swap(arr, n1+1, i-1);
    */
    //return index of the 
    //return i-1;

}

//sorts array pointed to by arr between indices n1 and n2 inclusive
void quicksort(int* arr, int n1, int n2)
{
    //optimization:use heapsort if you need to optimize
    if( (n2-n1+1) <= 20 )
    {
        InsertionSort(arr+n1, n2-n1+1);
        return;
    }
    
    //each recursive call 'sees' its input subarray as having indices 0 ~ len
    //int mid = MedThree(arr, n1, n2);
    int p = partition(arr, n1, n2);
    quicksort(arr, n1, p - 1);
    quicksort(arr, p+1, n2);

}

//use built in qsort in base case of 5 element array
//cutoff and median of 3 approach:

int main(int argc, char** argv)
{
    int* arr1 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr2 = (int *)calloc(MAXLEN, sizeof(int));

    //struct timeval start_time, end_time;
    struct timespec start_time, end_time;

    char input[11]; int elem;
    int len = 0;
    while(scanf("%s\n", input) != EOF){
        elem = atoi(input);
        arr1[len] = elem; arr2[len] = elem;
        len++;
    }

    //gettimeofday(&start_time, NULL);
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    quicksort(arr1, 0, len-1);
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    //gettimeofday(&end_time, NULL);
    //double time = ( end_time.tv_sec - start_time.tv_sec ) 
    //+ (( end_time.tv_usec - start_time.tv_usec ) / 1000000);
    double time = (end_time.tv_sec - start_time.tv_sec)*1000000000 + 
                            (end_time.tv_nsec - start_time.tv_nsec);
    fprintf(stderr, "It took %.3lf nanoseconds\n", time);


    //gettimeofday(&start_time, NULL);
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    qsort(arr2, (size_t)len, sizeof(int), compare);
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    //gettimeofday(&end_time, NULL);
    //time = ( end_time.tv_sec - start_time.tv_sec ) 
    //+ (( end_time.tv_usec - start_time.tv_usec ) / 1000000);
    time = (end_time.tv_sec - start_time.tv_sec)*1000000000 + 
                    (end_time.tv_nsec - start_time.tv_nsec);
    fprintf(stderr, "It took %.3lf nanoseconds\n", time);

    if(compare_arrays(arr1, arr2, len))
    {
        fprintf(stderr, "Difference between sorted arrays detected\n");
        //printf("arr1\n:"); print(arr1, 0, len); 
        //printf("arr2\n:");print(arr2, 0, len);
    }
    free(arr1); free(arr2);

    return 0;
}