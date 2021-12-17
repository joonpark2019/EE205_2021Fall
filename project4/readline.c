#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char* argv[]){
    char *buffer = NULL;
    int read;
    size_t len;

    //optional source vertex provided from stdin
    int source_id;

    if(argc > 1){
        source_id = atoi(argv[1]);
        printf("source_id from stdin is: %d\n", source_id);
    }else{
        printf("no source_id provided from stdin");
    }


    char *ptr; char delim[] = " ";
    int n, m; //n: number of vertices, m: number of edges
    int s, d, w; //source, destination, weight

//read 1st line:
    read = getline(&buffer, &len, stdin);
    assert(read != -1);
    /*
    if (-1 != read)
        puts(buffer);
    else
        printf("No line read...\n");
    */

    //printf("Size read: %d\n Len: %ld\n", read, len);
    
    //get n(number of vertices) and m(number of edge) from first line:
    ptr = strtok(buffer, delim); assert(ptr != NULL);
    n = atoi(ptr);
    ptr = strtok(NULL, delim); assert(ptr != NULL);
    m = atoi(ptr);
    printf("n is: %d, m is: %d\n", n, m);

    int graph[m][3];
    //int A[n+1][n];

    int i = 0;
    while( (read = getline(&buffer, &len, stdin)) != -1)
    {
        
        ptr = strtok(buffer, delim); assert(ptr != NULL);
        s = atoi(ptr); 
        //fprintf(stdout, "%d ", s);
        ptr = strtok(NULL, delim); assert(ptr != NULL);
        d = atoi(ptr); 
        //fprintf(stdout, "%d ", d);
        ptr = strtok(NULL, delim); assert(ptr != NULL);
        w = atoi(ptr); 
        //fprintf(stdout, "%d\n", w);
        assert(strtok(NULL, delim) == NULL); //this might cause error
        graph[i][0] = s; graph[i][1] = d; graph[i][2] = w;
        i++;
    }
    if (i != m) printf("number of edges provided does not match number of edges found\n");

    for(int j=0; j<m; j++){
        fprintf(stdout, "%d %d %d\n", graph[j][0], graph[j][1], graph[j][2]);
    }

    free(buffer);
    return 0;
}