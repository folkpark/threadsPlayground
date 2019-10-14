/*
Author: Parker Folkman

To build/run this program:
gcc -std=c99 road.c -o road -lpthread
./road <#TO_BRIDGER> <#TO_BOZEMAN> <#MAXCARS>

*/

#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <pthread.h> 
#include <unistd.h> //fork(), getpid() etc. 

//Global Variable
int MAXCARS; //Global variable 
pthread_mutex_t mutexsum; //Global Mutex


void *printThreadInfo(void *threadid){
    long tid;
    tid = (long)threadid;
    printf("Thread ID: %ld\n", tid);
    pthread_exit(NULL);
}

void *printThreadInfo2(void *threadid){
    long tid;
    tid = (long)threadid;
    printf("Thread ID: %ld\n", tid);
    pthread_exit(NULL);
}

void *OnBridgerOneWay(){
	
}

void *OnBozemanOneWay(){
	
}


int main(int argc, char** argv){
	pthread_mutex_init(&mutexsum, NULL);  // initialize the mutex
   
    int TO_BRIDGER = atoi(argv[1]); //# of cars to bridger from cmd line
    int TO_BOZEMAN = atoi(argv[2]); //# of cars to bozeman from cmd line  
    MAXCARS = atoi(argv[3]); //Max number of cars allowed on the road per batch. 

    pthread_t bridger_threads[TO_BRIDGER];//list of bridger threads
    pthread_t bozeman_threads[TO_BOZEMAN];//list of bozeman threads
	
	int fork_rv = fork();
	
	if(fork_rv < 0){ //First check if the fork failed
		printf("Fork failed!!");
		exit(-1);
	}else if(fork_rv==0){ //Child process
		printf("Child process created successfully with PID:  %d\n", getpid());
		for(int i = 0;i<TO_BRIDGER;i++){
			printf("Creating bridger bound thread with id: %d\n", i);
			int rc = pthread_create(&bridger_threads[i], NULL, printThreadInfo, (void *)(__intptr_t)i);
			if(rc){
				printf("Error creating thread: %d\n", rc);
				exit(-1);
			}
		}
	}else{ //Parent process
		sleep(1);
		printf("Parent process with PID: %d\n", getpid());
		for(int i = 0;i<TO_BOZEMAN;i++){
			printf("Creating bozeman bound thread with id: %d\n", i);
			int rc = pthread_create(&bozeman_threads[i], NULL, printThreadInfo2, (void *)(__intptr_t)i);
			if(rc){
				printf("Error creating thread: %d\n", rc);
				exit(-1);
			}
		}//end for
	}
	
    
    pthread_exit(NULL);
    return 0;
}
