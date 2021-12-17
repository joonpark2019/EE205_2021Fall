#define _POSIX_C_SOURCE 200809L

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>

int main(int argc, char* argv[]){
    
    int *A = (int*)calloc(n+1, sizeof(int)); memset()
    int *B = (int*)calloc(n+1, sizeof(int)); 

    double exec_time;
    gettimeofday(&start_time, NULL);
    for(int i=0; i<1000; i++){
        for
    }
    gettimeofday(&end_time, NULL);
    exec_time = end_time.tv_sec - start_time.tv_sec;
    if(exec_time > 10.0){
        fprintf(stderr, "program took longer than 10 sec.\n");
    }


    free(buffer);
    return 0;
}