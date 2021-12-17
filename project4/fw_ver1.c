#define _POSIX_C_SOURCE 200809L

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>

enum bool{FALSE, TRUE};


void floyd_warshall(int graph[][3], int n, int m){

    int A[n+1][n+1]; //n+1 because indexing starts at 1

    //array that stores all of the predecessors: B[i][j] = immediate predecessor of j in path i->j
    int B[n+1][n+1];

    //initialize:
    for(int v=1; v<=n; v++){
        for(int w=1; w<=n; w++){
            if(v==w) A[v][w] = 0;
            else A[v][w] = INT_MAX;
        }
    }
    //A[i][j] = l_ij if (i,j) is a vertex in the graph input
    for(int i=0; i<m; i++){
        A[graph[i][0]][graph[i][1]] = graph[i][2];
    }

    //can check if there is no edge now: if there is no edge, then the 
    for(int v=1; v<=n; v++){
        for(int w=1; w<=n; w++){
            if( A[v][w] != INT_MAX)
                B[v][w] = v;
            else
                B[v][w] = -1;
        }
        
    }
    
    for(int k=1; k<=n; k++){
    //kth iteration means optimal substructure including the vertex k
        for(int v=1; v<=n; v++){
            for(int w=1; w<=n; w++){ //path from vertex v to w
                if(A[v][k] != INT_MAX && A[k][w] != INT_MAX && A[v][w] > A[v][k] + A[k][w]){
                    A[v][w] = A[v][k] + A[k][w];
                    B[v][w] = B[k][w];
                }

            }
        }
    }

    for(int v=1; v<=n; v++){
        if(A[v][v] < 0){
            printf("Error: negative cost cycle is found\n");
            return ;
        }
    }


    //for tracking the path (max path length possible is n-1 edges for no negative cycles):
    //int track[n+1];
    //the for loop for printing paths to all 
    for (int v=1; v<=n; v++){
        for(int w=1; w<=n; w++){
            //for path v->w, populate track array for path backwards from w to v
            
            if(w==v) continue;
            if(A[v][w] == INT_MAX){
                printf("%d %d length: inf path: none", v, w);
                continue;
            }
            printf("%d %d length: %d", v, w, A[v][w]); printf(" path:");
/*
            int j=w; int t = 0;
            while(j!=v){
                track[t++] = j;
                j = B[w][j];
            }
            track[t] = v;
            //printing path from v to w
            for(int l=t; l>=0; l--){
                printf(" %d", track[l]);
            } */
            printf("\n");

        }
    }
    return ;
    
}


int main(int argc, char* argv[]){
    char *buffer = NULL;
    int read;
    size_t len;
    struct timeval start_time, end_time;


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

    double exec_time;
    gettimeofday(&start_time, NULL);

    
    floyd_warshall(graph, n, m);
    
    
    gettimeofday(&end_time, NULL);
    exec_time = end_time.tv_sec - start_time.tv_sec;
    if(exec_time > 10.0){
        fprintf(stderr, "program took longer than 10 sec.\n");
    }


    free(buffer);
    return 0;
}