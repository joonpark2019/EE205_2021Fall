#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define MAXLEN 10000000

void swap(int* arr, int n1, int n2)
{
    int temp = arr[n1]; arr[n1] = arr[n2]; arr[n2] = temp;
    return;
}

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

int ChoosePivot(int *arr, int n1, int n2, int type, int *comparisons);

//returns index of pivot between n1 and n2
//pivot_num is the index of the pivot
int partition(int* arr, int n1, int n2, int pivot_num, int *comparisons){
    int i, j;
    //make sure pivot is at first element:
        //if(arr[pivot_num] > arr[n1])
    swap(arr, n1, pivot_num);
    int pivot = arr[n1];

    i = n1 + 1;
    for(j = n1 + 1; j <= n2; j++){
        if(arr[j] < pivot){
            swap(arr, j, i);
            i++;
        }
        *comparisons += 1;
    }
    swap(arr, n1, i-1);
    
    return i-1;
}

//the same exact array pointer passed to every recursive call
void quicksort(int* arr, int n1, int n2, int type, int *comparisons)
{
    if(n1>=n2)
        return;
    int pivot_num = ChoosePivot(arr, n1, n2, type, comparisons);
    
    int p = partition(arr, n1, n2, pivot_num, comparisons);
    //printf("n1 is %d, n2 is %d, pivot is %d, pivot moved to %d\n", n1, n2, arr[p], p);
   // print(arr, n1, n2);
    /*
    int left = (p - n1); int right = (n2 - p);
    if(left > 0)
        *comparisons += left - 1;
    
    if(right > 0)
        *comparisons += right - 1;
    */
    //printf("    Number of left elements: %d\n", left);
    //printf("    Number of right elements: %d\n", right);
    quicksort(arr, n1, p - 1, type, comparisons);
    quicksort(arr, p+1, n2, type, comparisons);

    //printf("FINAL SORTED ARRAY: \n");
    //print(arr, n1, n2);

}

//optimization strategies:
    //use bubble sort for when integer is of lower length

//how to split the array?
    //

int main(int argc, char* argv[])
{
    
    int* arr1 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr2 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr3 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr4 = (int *)calloc(MAXLEN, sizeof(int));
    int* arr5 = (int *)calloc(MAXLEN, sizeof(int));

    //how to count number of comparisons?? - global variable? 
    //how to run all three different pivot element selection methods separately?
    //
    char input[10]; int elem;
    int len = 0;
    while(scanf("%s\n", input) != EOF){
        elem = atoi(input);
        arr1[len] = elem; arr2[len] = elem; arr3[len] = elem; arr4[len] = elem;
        arr5[len] = elem;
        len++;
    }
    int comparisons = 0;
    quicksort(arr1, 0, len-1, 1, &comparisons);
    printf("First: %d\n", comparisons); comparisons = 0;

    quicksort(arr2, 0, len-1, 2, &comparisons);
    printf("Last: %d\n", comparisons); comparisons = 0;

    quicksort(arr3, 0, len-1, 3, &comparisons);
    printf("Random: %d\n", comparisons); comparisons = 0;

    quicksort(arr4, 0, len-1, 4, &comparisons);
    printf("Median-of-three: %d\n", comparisons); comparisons = 0;

    free(arr1); free(arr2); free(arr3); free(arr4);

    return 0;
    
}

//if type == 4, then 2 additional comparisons are done
//returns index of the pivot in the original array
int ChoosePivot(int *arr, int n1, int n2, int type, int *comparisons)
{   
    assert(type > 0 && type <= 4);
    int mid = (n1 + n2) / 2;
    int pivot;
    time_t t;
    srand((unsigned) time(&t));

    //return first element
    switch(type)
    {
        case 1:
            pivot = n1;
            break;
        //return last element
        case 2:
            pivot = n2;
            break;
        case 3:
            pivot = n1 + (rand() % (n2 - n1 + 1));
            break;
        case 4:
            if(arr[mid] < arr[n1])
                swap(arr, mid, n1);
            if(arr[n2] < arr[mid])
                swap(arr, mid, n2);
            if(arr[mid] < arr[n1])
                swap(arr, mid, n1);
            pivot = mid;
            // *comparisons += 3;
            break;
        default:
            printf("Error! type number out of bounds");
            
    }
    assert(pivot <= n2);
    
    return pivot;
}


