#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define MAXLEN 10000000

//function for swapping elements at indices n1 and n2 in array arr
void swap(int* arr, long n1, long n2)
{
    int temp = arr[n1]; arr[n1] = arr[n2]; arr[n2] = temp;
    return;
}

void print(int* arr, long n1, long n2)
{
    for(long i = n1; i <= n2; i++)
        printf("%d ", arr[i]);
    printf("%c ", '\n');
    return;
}

//function called in partition function to choose pivot
//code is added the end of the file as it is somewhat lengthy
long ChoosePivot(int *arr, long n1, long n2, int type, long *comparisons);

//partition() returns index of pivot between n1 and n2
//pivot_num is the index of the pivot
long partition(int* arr, long n1, long n2, long pivot_num){
    assert(n2 > n1);
    long i, j;
    //swap pivot with the first element:
        //if(arr[pivot_num] > arr[n1])
    swap(arr, n1, pivot_num);
    int pivot = arr[n1];

    i = n1 + 1;
    for(j = n1 + 1; j <= n2; j++){
        if(arr[j] < pivot){
            swap(arr, j, i);
            i++;
        }
    }
    swap(arr, n1, i-1);
    
    return i-1;
}

//the same exact array pointer passed to every recursive call
void quicksort(int* arr, long n1, long n2, int type, long *comparisons)
{
    assert(*comparisons >= 0); 
    
    if(n1>=n2)
        return;
    long pivot_num = ChoosePivot(arr, n1, n2, type, comparisons);
    
    long p = partition(arr, n1, n2, pivot_num);

    long left = (p - n1); long right = (n2 - p);
    //length of left subarray is p - n1 + 1, so number of comparisons will be equal to p - n1
    if(left > 0) //if left == 0, then no more comparisons should be added
        *comparisons += left;
    //length of left subarray is n2 - p + 1, so number of comparisons will be equal to n2 - p
    if(right > 0)
        *comparisons += right;

    
    quicksort(arr, n1, p - 1, type, comparisons);
    quicksort(arr, p+1, n2, type, comparisons);


}


int main(int argc, char* argv[])
{
//allocate 4 separate arrays for the 4 different quicksort methods:
    int* arr1 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr2 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr3 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr4 = (int *)calloc(MAXLEN, sizeof(int));

    //in decimal form, INT_MAX has 10 decimal places at most.
    char input[10]; int elem;
    long len = 0;
    //read strings representing integers from stdin until EOF
    while(scanf("%s\n", input) != EOF){
        elem = atoi(input);
        arr1[len] = elem; arr2[len] = elem; arr3[len] = elem; arr4[len] = elem;
        len++;
    }
    long comparisons = 0;

    //Run each sorting method:
    //value at comparisons is re-initialized to zero after each method

    //printf("Running First:\n");
    quicksort(arr1, 0, len-1, 1, &comparisons);
    printf("First: %ld\n", comparisons); comparisons = 0;

    //printf("Running Last:\n");
    quicksort(arr2, 0, len-1, 2, &comparisons);
    printf("Last: %ld\n", comparisons); comparisons = 0;

    //printf("Running Random:\n");
    for (int i = 0; i<10; i++){
        quicksort(arr3, 0, len-1, 3, &comparisons);
    }
    printf("Random: %ld\n", comparisons/10); comparisons = 0;

    //printf("Running Median of three:\n");
    quicksort(arr4, 0, len-1, 4, &comparisons);
    printf("Median-of-three: %ld\n", comparisons); comparisons = 0;

    free(arr1); free(arr2); free(arr3); free(arr4);

    return 0;
    
}

//Description of ChoosePivot():
//function returns index of the pivot in the original array
//receives array and the lower bound n1 and upper bound n2 for the indices of sorting
//type input indicates the quicksort method:
    //type == 1: choose first element as pivot
    //type == 2: choose last element as pivot
    //type == 3: choose random pivot
    //type == 4: choose median-of-3 pivot
//if type == 4, then 2 or 3 additional comparisons are done
//comparisons pointer stores integer with the number of comparisons
long ChoosePivot(int *arr, long n1, long n2, int type, long *comparisons)
{   
    assert(type > 0 && type <= 4);
    assert(n2 > n1);
    long mid = (n1 + n2) / 2;
    long pivot;
    time_t t;
    srand((unsigned) time(&t));

    
    switch(type)
    {
        //return first element as pivot
        case 1:
            pivot = n1;
            break;
        //return last element as pivot
        case 2:
            pivot = n2;
            break;
        //return random element as pivot
        case 3:
            pivot = n1 + (rand() % (n2 - n1 + 1));
            break;
        //return median of first, last, center element as pivot
        case 4:
    //finding the median leads to three comparisons at most with this approach
            /*it is possible for it to only lead to 2 comparisons:
                ex. the middle and last element are always compared first. 
                If the middle element is larger than the last element and the last
                element is found larger than the first element, then the pivot is
                deduced as the last element with only 2 comparisons.

                3 comparisons are needed if, for example, the last element is smaller
                than the first element in the above example. 
                In this case, the middle element must be compared to the first element.
            */

            //compare values at mid and n2
            *comparisons += 1;
            if(arr[mid] < arr[n2]){
                //compare values at n1 and mid
                if(arr[n1] < arr[mid]){
                    *comparisons += 1;
                    pivot = mid;
                }
                //compare values at n1 and n2
                else if(arr[n1] < arr[n2]){
                    *comparisons += 1;
                    pivot = n1;
                }
                else{
                    pivot = n2;
                }
            }
            else{ //arr[mid] > arr[n2]
                //compare values at n1 and n2
                if(arr[n1] < arr[n2]){
                    *comparisons += 1;
                    pivot = n2;
                }
                //compare values at mid and n2
                else if(arr[mid] < arr[n1]){
                    *comparisons += 1;
                    pivot = mid;
                }
                else{
                    pivot = n1;
                }
            }
            
            break;
        default:
            printf("Error! type number out of bounds");
            
    }
    assert(pivot <= n2);
    
    return pivot;
}


