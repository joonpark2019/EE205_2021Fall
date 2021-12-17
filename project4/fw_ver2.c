#define _POSIX_C_SOURCE 200809L

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>

//recursive function printing path from v -> w
//B[v][w]: predecessor node in v->w path ie last node before w
void print_path(int **B, int v, int w){
    
    int j = B[v][w];
    if(v==w){
        //printf(" %d", v); 
        return;
    }
    //if(v!=w) 
    print_path(B, v, j);
    printf(" %d", j);
    return ;
}


void floyd_warshall(int graph[][3], int n, int m){
    //int A[n+1][n+1] for caching shortest distance
    //A[v][w] is the shortest distance from node v to node w
    int **A = calloc(n+1, sizeof(int*));
    //array that stores all of the predecessors: B[i][j] = immediate predecessor of j in path i->j
    //int B[n+1][n+1];
    int **B = calloc(n+1, sizeof(int*));
    //int A[n+1][n+1]; //n+1 because indexing starts at 1
    for(int i=1; i<=n; i++){
        A[i] = calloc(n+1, sizeof(int*));
        B[i] = calloc(n+1, sizeof(int*));
    }

    //initialize all values of A:
    //INT_MAX is used instead of infinity here
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


    //use predecessor node for path reconstruction:
    for(int v=1; v<=n; v++){
        for(int w=1; w<=n; w++){
            if( A[v][w] != INT_MAX)
                B[v][w] = v;
            else
                B[v][w] = -1;
        }
        
    }
    
    for(int k=1; k<=n; k++){
    //kth iteration means optimal substructure including all vertices up to k
        for(int v=1; v<=n; v++){
            for(int w=1; w<=n; w++){ //path from vertex v to w
                if(A[v][k] != INT_MAX && A[k][w] != INT_MAX && A[v][w] > A[v][k] + A[k][w]){
                    A[v][w] = A[v][k] + A[k][w];
                    //add predecessor of v->w path as the predecessor in the k->w path
                    //this will allow for recursive path reconstruction later
                    if(A[v][w] != INT_MAX) B[v][w] = B[k][w];
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



    //the forloop for printing paths between all vertices v, w with path v->w
    for (int v=1; v<=n; v++){
        for(int w=1; w<=n; w++){
            //skip path from node to itself
            if(w==v) continue;
            if(A[v][w] == INT_MAX){
                printf("%d %d length: inf path: none", v, w);
                continue;
            }
            printf("%d %d length: %d", v, w, A[v][w]); printf(" path:");

            print_path(B, v, w); 
            printf(" %d", w);
            printf("\n");

        }
    }
    return ;
    
}


int main(int argc, char* argv[]){
    char *buffer = NULL;
    int read;
    size_t len;


    /* since for each line, source, destination, and edge weight 
    are separated by a space, use strtok with space as delimeter character */
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

    int graph[m][3];
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


    floyd_warshall(graph, n, m);
    


    free(buffer);
    return 0;
}