#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

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

void print(int* arr, long n1, long n2)
{
    for(long i = n1; i <= n2; i++)
        printf("%d ", arr[i]);
    printf("%c ", '\n');
    return;
}

void print_out(int* arr, long n1, long n2)
{
    for(long i = n1; i <= n2; i++)
        fprintf(stdout, "%d\n", arr[i]);
    //fprintf(stdout, "%c", '\n');
    return;
}

/****************************************************************************/


int compare (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void swap(int* arr, long n1, long n2)
{
    int temp = arr[n1]; arr[n1] = arr[n2]; arr[n2] = temp;
    return;
}

long MedThree(int* arr, long n1, long n2)
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

long partition(int* arr, long n1, long n2, long* p_ptr){
    //int i, j;
    //make sure pivot is at first element:
    
    //put median of first, last, and middle element in the middle index
    
    long p_index = *p_ptr;
    swap(arr, n1+1, p_index);
    int pivot = arr[n1+1];
    //printf("pivot selected is %ld\n", pivot);
    //print(arr, n1, n2);

//make sure pivot is at the beginning: swap until index n2 - 1

    long l = n1+2; long m = n1+2; long h = n2-1;
    //printf("l is %ld\n", l);printf("m is %ld\n", m);printf("h is %ld\n", h);
    /*
    while (1)
    {
        while(pivot > arr[l] && l <= n2-1){
            l++; m++;
        }
        while(pivot < arr[h] && h >= (n1+2))
            h--;
        while(pivot == arr[m] && m <= n2-1)
            m++;
        if(m >= h) break;
        swap(arr, l, m);
        swap(arr, m, h); 
    }
    */

    while(1){
        while(arr[m] < pivot && m <= n2-1){
            swap(arr, l++, m++); 
        }
        while(pivot < arr[h] && h >= (n1+2))
            h--;
        while(pivot == arr[m] && m <= n2-1)
            m++;
        if(m >= h) break;
        swap(arr, m, h); 
    }
    
//swap pivot with h
    swap(arr, h, n1+1);
    //swap(arr, l, n1);
    *p_ptr = l+1;
    return h+1;

}


//sorts array pointed to by arr between indices n1 and n2 inclusive
void quicksort(int* arr, long n1, long n2)
{
    //optimization:use heapsort if you need to optimize
    if( (n2-n1+1) <= 20 )
    {
        InsertionSort(arr+n1, n2-n1+1);
        return;
    }  
    
    long* p_ptr; long p = MedThree(arr, n1, n2);
    p_ptr = &p;//p_ptr will have the value of 
    long right = partition(arr, n1, n2, p_ptr);
    //printf("n1 is %d, n2 is %d, pivot is %d, pivot moved to %d\n", n1, n2, arr[p], p);
    //print(arr, n1, n2);
    quicksort(arr, n1, p - 1);
    quicksort(arr, right, n2);

}

//use built in qsort in base case of 5 element array
//cutoff and median of 3 approach:

int main(int argc, char* argv[])
{
    int* arr1 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr2 = (int *)calloc(MAXLEN, sizeof(int));

    //struct timeval start_time, end_time;
    struct timespec start_time, end_time;

    char input[11]; int elem;
    long len = 0;
    while(scanf("%s\n", input) != EOF){
        elem = atoi(input);
        arr1[len] = elem; arr2[len] = elem;
        len++;
    }

   char* argv_0 = argv[1];
   if(strcmp("mine", argv_0) == 0){
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
        print_out(arr1, 0, len-1);
    }

   else if(strcmp("crt", argv_0)==0) {
        //gettimeofday(&start_time, NULL);
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        qsort(arr2, len, sizeof(int), compare);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        //gettimeofday(&end_time, NULL);
        //time = ( end_time.tv_sec - start_time.tv_sec ) 
        //+ (( end_time.tv_usec - start_time.tv_usec ) / 1000000);
         double time = (end_time.tv_sec - start_time.tv_sec)*1000000000 + 
                        (end_time.tv_nsec - start_time.tv_nsec);
        fprintf(stderr, "It took %.3lf nanoseconds\n", time);
        
        print_out(arr2, 0, len-1);
    }
    else{
      printf("give a command line argument\n");
    }

/*
    if(compare_arrays(arr1, arr2, len))
    {
        fprintf(stderr, "Difference between sorted arrays detected\n");
        //printf("arr1\n:"); print(arr1, 0, len); 
        //printf("arr2\n:");print(arr2, 0, len);
    }
*/
    free(arr1); free(arr2);

    return 0;
}