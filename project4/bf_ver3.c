#define _POSIX_C_SOURCE 200809L

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>

enum bool{FALSE, TRUE};

//recursive function printing path from v -> w
//B[v][w]: predecessor node in v->w path ie last node before w
void print_path(int *B, int s, int v){
    
    int j = B[v];
    if(v==s){
        //printf(" %d", v); 
        return;
    }
    //if(v!=w) 
    print_path(B, s, j);
    printf(" %d", j);
    return ;
}

int bellman_ford(int graph[][3], int s, int n, int m){
    int *A = (int*)calloc(n+1, sizeof(int)); 
    int *B = (int*)calloc(n+1, sizeof(int)); 

    //bool stable is for indicating whether there is a negative cycle or not.
    enum bool stable; 
    
    //initializing the entries in array A:
    A[s] = 0;
    for(int i=1; i<n+1; i++){
        if(i!=s){
            //all entries initialized to infinity(INT_MAX)
            A[i] = INT_MAX;
        }
    }

    for(int i=1; i<n+1; i++){
        B[i] = -1;
    }

    //the main loop for the bf algorithm:
    int i; //int x;
    //run for n iterations instead of n-1 to check for negative cycle
    for(i=1; i<n+1; i++){
        stable = TRUE; //x = -1;

        //loop through all of the graph edges:
        for(int j=0; j<m; j++){
            int w = graph[j][0]; int v = graph[j][1]; int l_wv=graph[j][2];
            //case 1: nothing changes -> A[v] gets maintained
            //case 2: A[w] + l_wv is shorter so A[v] is updated
            if(A[w] != INT_MAX && A[v] >  A[w] + l_wv){ //first iteration: just with the source
                A[v] = A[w] + l_wv;
                B[v] = w;
                stable = FALSE; //x = v;
            }
        }
        if(stable==TRUE) break;
    }
    //ran n iterations and still failed to stabilize: negative cycle


    if(stable != TRUE){
        printf("Error: negative cost cycle is found\n");
        free(A); free(B);
        return -1;
    }

    //for tracking the path (max path length possible is n edges for no negative cycles):
    //int track[n];
    //the for loop for printing paths to all 
    for (int i=1; i<n+1; i++){
    /*
        int j = i; //initialize j with i: j will be used to reconstruct shortest path
        int t=0;
    */
        if(i==s) continue;
        if(A[i] == INT_MAX){
            printf("%d %d length: inf path: none\n", s, i);
            continue;
        }
        printf("%d %d length: %d", s, i, A[i]); printf(" path:");
            //populate the path tracking array:
            //from index j, go back in predecessor array B until s is reached
        
    /*
        while(j != s){
            track[t++] = j;
            j = B[j];
        } 
        track[t] = s; //set final entry in track array to source s
        //print out the path from source to destination:
        for(int l=t; l>=0; l--){
            printf(" %d", track[l]);
        }
    */
        print_path(B, s, i); 
        printf(" %d", i);
        printf("\n");
        
    }
    free(A); free(B);
    return 0;
}

int main(int argc, char* argv[]){
    char *buffer = NULL;
    int read;
    size_t len;
    struct timeval start_time, end_time;

    //optional source vertex provided from stdin
    int source_id = -1;

    if(argc > 1){
        source_id = atoi(argv[1]);
        //printf("source_id from stdin is: %d\n", source_id);
    }
    //else{
    //    printf("no source_id provided from stdin\n");
    //}


    char *ptr; char delim[] = " ";
    int n, m; //n: number of vertices, m: number of edges
    int s, d, w; //source, destination, weight

//read 1st line:
    read = getline(&buffer, &len, stdin);
    assert(read != -1);
    int max_len = 2*strlen(buffer);
    
    //get n(number of vertices) and m(number of edge) from first line:
    ptr = strtok(buffer, delim); assert(ptr != NULL);
    n = atoi(ptr);
    ptr = strtok(NULL, delim); assert(ptr != NULL);
    m = atoi(ptr);
    //printf("n is: %d, m is: %d\n", n, m);

    int graph[m][3];
    //int A[n+1][n];
    char line[max_len];

    int i = 0;
    //while( (read = getline(&buffer, &len, stdin)) != -1)
    while(fgets(line, max_len + 1, stdin))
    {
        
        ptr = strtok(line, delim); assert(ptr != NULL);
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

/*
    for(int j=0; j<m; j++){
        fprintf(stdout, "%d %d %d\n", graph[j][0], graph[j][1], graph[j][2]);
    }*
*/
    int stop;
    double exec_time;
    gettimeofday(&start_time, NULL);
    if(source_id != -1){ //argument provided
        stop = bellman_ford(graph, source_id, n, m);
    }else{
        for(int i=1; i<=n && stop != -1; i++){
            stop = bellman_ford(graph, i, n, m);
        }
    }
    gettimeofday(&end_time, NULL);
    exec_time = end_time.tv_sec - start_time.tv_sec;
    if(exec_time > 10.0){
        fprintf(stderr, "program took longer than 10 sec.\n");
    }


    free(buffer);
    return 0;
}