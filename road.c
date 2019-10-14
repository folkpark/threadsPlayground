/*
Author: Parker Folkman

To build/run this program:
gcc -std=c99 road.c -o road -lpthread
./road

*/

#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <pthread.h> 

//Use global variables for testing
//#define TO_BRIDGER 3
//#define TO_BOZEMAN 3


void *printThreadInfo(void *threadid){
   long tid;
   tid = (long)threadid;
   printf("Thread ID: %ld\n", tid);
   pthread_exit(NULL);
}


int main(int argc, char** argv){

   
    int TO_BRIDGER = atoi(argv[1]); //# of cars to bridger from cmd line
    int TO_BOZEMAN = atoi(argv[2]); //# of cars to bozeman from cmd line  

    pthread_t bridger_threads[TO_BRIDGER];//list of bridger threads
    pthread_t bozeman_threads[TO_BOZEMAN];//list of bozeman threads
    int rc;
    int i;
    for(i = 0;i<TO_BRIDGER;i++){
        printf("Creating eastbound thread with id: %d\n", i);
        rc = pthread_create(&bridger_threads[i], NULL, printThreadInfo, (void *)(__intptr_t)i);
        if(rc){
            printf("Error creating thread: %d\n", rc);
            exit(-1);
        }   
    }
    
    pthread_exit(NULL);
    return 0;
}
