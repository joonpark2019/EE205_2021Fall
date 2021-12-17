#include <stdlib.h>

void swap(int arr[], int i, int j)
{
    int temp = arr[i]; arr[i] = arr[j]; v[j] = temp;
}

//naive implementation:
void qsort(int arr[], int n1, int n2)
{
    int i, last;

    if (n1 >= n2)
        return;
    swap(arr, n1, (n1+n2)/2);
    last = n1; 

    for (i = n1 + 1; i<=n2; i++)
    {
        
    }
}