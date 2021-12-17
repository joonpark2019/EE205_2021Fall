#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    FILE *fp;

    time_t t;
    srand((unsigned) time(&t));
    
    fp = fopen("test.txt", "w");
    for(int i = 0; i<10000000; i++){
        fprintf(fp, "%d\n", (rand()%10000000));
    }
    
    return 0;
}