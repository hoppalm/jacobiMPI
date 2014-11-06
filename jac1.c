/*
 * MPI blocked 1D Jacobi
 * author: wimbo
 */

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

double f(int i, int n){
    if (i<n/2) return i;
    else return n-i-1;
}

/*
 double f(int i, int n){
 return i;
 }
 */


int main(int argc, char **argv) {
    int        id, p, i, j, k, n, t, m, v, vp;
    double     startwtime, endwtime;
    float      time;
    MPI_Status status;	/* return status for receive */
    double     *prev, *cur, *temp;
    
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &id );
    MPI_Comm_size( MPI_COMM_WORLD, &p );
    if (argc < 4) {
        fprintf (stderr, "need problem size, #iterations and buffer size\n");
        goto EXIT;
    }
    if ((sscanf (argv[1], "%d", &n) < 1) ||
        (sscanf (argv[2], "%d", &m) < 1) ||
        (sscanf (argv[3], "%d", &k) < 1)) {
        fprintf (stderr, "need int params\n");
        goto EXIT;
    }
    
    if (argc > 4) {
        v=1;
        sscanf (argv[4], "%d", &vp);
    } else v=0;  /* are we in verbose mode? vp = reporting process*/
    
    int block_size = (n/p+2*k);
    // Memory allocation for data array.
    prev  = (double *) malloc( sizeof(double) * block_size);
    cur   = (double *) malloc( sizeof(double) * block_size);
    
    if ( prev == NULL || cur == NULL ) {
        printf("[ERROR] : Failed to allocate memory.\n");
        goto EXIT;
    }
    
    MPI_Barrier(MPI_COMM_WORLD);/* make sure everyone is active */
    
    printf("\nnumber of processers %d\tcurrent id %d\tblock_size %d\tsetting %d in each block\n", p,id,block_size,n/p);
    t = 0;
    if(p==1){
        // Initialization
        startwtime = MPI_Wtime();
        for(i=0;i<n;i++)  prev[i] = f(i,n);
        
        //printf checking something DEBUG DELETE LATER
        for(i=0;i<n;i++) printf("%f ",prev[i]);
        printf("\n");
        
        cur[0] = f(0,n); cur[n-1] = f(n-1,n);
        while (t < m) {
            for ( i=1 ; i < n-1 ; i++ ) {
                cur[i] = (prev[i-1]+prev[i]+prev[i+1])/3;
            }
            temp = prev; prev = cur;  cur  = temp; t++;
        }
        if(v){
            for(i=0;i<n;i++) printf("%f ",prev[i]);
            printf("\n");
        }
        endwtime = MPI_Wtime();
        time = endwtime-startwtime;
        printf("Sequential process complete, time: %f\n", time);
        goto EXIT;
    }
    else {
        double leftSendingBuffer[k];
        double rightSendingBuffer[k];
        double buffer[k];
        
        for (i = 0;i<block_size; i++){
            prev[0] = 0;
        }
        //initialization
        for(i=0;i<n/p;i++)  {
            printf("setting %d in n\n", i+(n/p*id));
            prev[i+k] = f(i+(n/p),n);
        }
        
        //printf checking something DEBUG DELETE LATER
        for(i=0;i<block_size;i++) printf("%f ",prev[i]);
        printf("\n");

        
        if (id == 0) {
            rightSendingBuffer[0] = id;
            
            //MPI_Send((double *)rightSendingBuffer, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
            
            //MPI_Recv((double *)buffer, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
            
            //MPI_Recv( (int *)A00, b*b, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
            
            /*MPI_Send( (int *)A01, b*b, MPI_INT, 0, 1, MPI_COMM_WORLD);
             
             MPI_Send(buffer, 10, MPI_INT, 1, 123, MPI_COMM_WORLD);
             
             MPI_Send(buffer, 10, MPI_INT, 1, 123, MPI_COMM_WORLD);*/
            //printf("%d int recieved from right", *buffer[0]);
            
            printf("first processor left processor: %d right processor: %d\n", id-1, id+1);
        }
        else if(id == p-1){
            leftSendingBuffer[0] = id;
            
            //MPI_Send((double *)leftSendingBuffer, 1, MPI_DOUBLE, id-1, id-1, MPI_COMM_WORLD);
            
            //MPI_Recv((double *)buffer, 1, MPI_DOUBLE, id-1, id, MPI_COMM_WORLD, &status);
            
            //printf("%d int recieved from left", *buffer[0]);
            
            printf("last  processor left processor: %d right processor: %d\n", id-1, id+1);
        }
        
        else{
            leftSendingBuffer[0] = id;
            rightSendingBuffer[0] = id;
            
            //MPI_Recv(buffer, 1, MPI_DOUBLE, id-1, id, MPI_COMM_WORLD, &status);
            
            //MPI_Send(rightSendingBuffer, 1, MPI_DOUBLE, id+1, id+1, MPI_COMM_WORLD);
            
            //printf("%d int recieved from left", buffer[0]);
            
            //MPI_Send(leftSendingBuffer, 1, MPI_DOUBLE, id-1, id-1, MPI_COMM_WORLD);
            
            //MPI_Recv(buffer, 1, MPI_DOUBLE, id+1, id, MPI_COMM_WORLD, &status);
            
            
            //printf("%d int recieved from right", buffer[0]);
            
            printf("left processor: %d right processor: %d\n", id-1, id+1);
        }
    }
    
    
EXIT:
    MPI_Finalize();
    return 0;
}
