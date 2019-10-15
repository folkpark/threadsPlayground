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
int TO_BRIDGER;
int TO_BOZEMAN;
pthread_mutex_t mutexBridger; //Global Mutex
pthread_mutex_t mutexBozeman; //Global Mutex
int semaphore;
int on_bridge = 0;
int total_passed_bz = 0;
int total_passed_br = 0;

void *manageToBridger(void *threadid){
    long tid;
    tid = (long)threadid;
	
	int passed = 0;
	while(passed==0){ //Car hasn't passed yet
		
		if(semaphore==0){ //Cars initially wait their turn
			pthread_mutex_lock (&mutexBridger); //<<< lock!
			on_bridge += 1;
			printf("\t\t\t\t on_bridge %d\n", on_bridge);
			printf("Car id: %ld crossed the bridge going to Bridger \n",tid);
			passed = 0; //Car crossed the bridge
			total_passed_br += 1; //Keep track of all the cars who've passed
			printf("Total cars passed to Bridger %d\n", total_passed_br);
			
			if(total_passed_br==TO_BRIDGER){
				printf("		ALL CARS GOING TO BRIDGER ARE GONE \n");
				on_bridge = 0; //Reset car count on bridge
				semaphore = 1; //Signal the other side to go
			}
			
			if(on_bridge >= MAXCARS){ 
			    printf("  Max cars on bridge!!!!!!!!!!!!! \n");
				if(total_passed_bz==TO_BOZEMAN){
					on_bridge = 0; //Reset car count on bridge
					semaphore = 0; //Keep going since the other side is all gone
				}else{
					on_bridge = 0; //Reset car count on bridge
					semaphore = 1; //Signal the other side to go
				}
				while(semaphore==1 && total_passed_bz != TO_BOZEMAN){} //Loop until its safe to release lock
				//printf("Bridger changed states \n");
			} 
			
			pthread_mutex_unlock (&mutexBridger);  //<<< unlock!
			pthread_exit(NULL);
		}
	}
}

void *manageToBozeman(void *threadid){
    long tid;
    tid = (long)threadid;
	sleep(1);
	int passed = 0;
	while(passed==0){ //Car hasn't passed yet
		
		if(semaphore==1){ //Cars initially wait their turn
			//printf("Waiting my turn to go to Bozeman\n");
			pthread_mutex_lock (&mutexBozeman); //<<< lock!
			on_bridge += 1;
			printf("\t\t\t\t on_bridge %d\n", on_bridge);
			printf("Car id: %ld crossed the bridge going to Bozeman \n",tid);
			passed = 0; //Car crossed the bridge
			total_passed_bz += 1; //Keep track of total cars who've passed
			printf("Total cars passed to Bozeman %d\n", total_passed_bz);
			
			if(total_passed_bz==TO_BOZEMAN){
				printf("		ALL CARS GOING TO BOZEMAN ARE GONE \n");
				on_bridge = 0; //Reset car count on bridge
				semaphore = 0; //Signal the other side to go
			}
			
			if(on_bridge >= MAXCARS){ 
			    printf("   Max cars on bridge! \n");
				if(total_passed_br==TO_BRIDGER){
					on_bridge = 0; //Reset car count on bridge
					semaphore = 1; //Keep going since the other side is all gone
				}else{
					on_bridge = 0; //Reset car count on bridge
					semaphore = 0; //Signal the other side to go
				}
				while(semaphore==0 && total_passed_br != TO_BRIDGER){} //Loop until its safe to release lock
				//printf("Bozeman changed states");
			} 
			
			pthread_mutex_unlock (&mutexBozeman);  //<<< unlock!
			pthread_exit(NULL);
		}
	}
}

void *bridgerQueueEmpty(){
	printf("\n All cars headed to Bridger have passed!");
}

void *bozemanQueueEmpty(){
	printf("\n All cars headed to Bozeman have passed! \n");
}


int main(int argc, char** argv){
	pthread_mutex_init(&mutexBridger, NULL);  // initialize the mutex
	pthread_mutex_init(&mutexBozeman, NULL);  // initialize the mutex
	semaphore = 0;
   
    TO_BRIDGER = atoi(argv[1]); //# of cars to bridger from cmd line
    TO_BOZEMAN = atoi(argv[2]); //# of cars to bozeman from cmd line  
    MAXCARS = atoi(argv[3]); //Max number of cars allowed on the road per batch. 
	printf("MAXCARS is %d\n", MAXCARS);

    printf("There are %d cars going to bridger \n",TO_BRIDGER);
    pthread_t bridger_threads[TO_BRIDGER];//list of bridger threads
	printf("There are %d cars going to bozeman \n",TO_BOZEMAN);
    pthread_t bozeman_threads[TO_BOZEMAN];//list of bozeman threads
	
	for(int i=0;i<TO_BRIDGER;i++){
		int rc = pthread_create(&bridger_threads[i], NULL, manageToBridger, (void *)(__intptr_t)i);
			if(rc){
				printf("Error creating thread: %d\n", rc);
				exit(-1);
			}
	}
	
	for(int i=0;i<TO_BOZEMAN;i++){
		int rc = pthread_create(&bozeman_threads[i], NULL, manageToBozeman, (void *)(__intptr_t)i);
			if(rc){
				printf("Error creating thread: %d\n", rc);
				exit(-1);
			}
	}
	
	
	
/* 	int fork_rv = fork();
	if(fork_rv < 0){ //First check if the fork failed
		printf("Fork failed!!");
		exit(-1);
	}else if(fork_rv==0){ //Child process
		printf("Child process managing %d bridger bound threads has PID:  %d\n", TO_BRIDGER, getpid());
		for(int i = 0;i<TO_BRIDGER;i++){
			//printf("Creating bridger bound thread with id: %d\n", i);
			int rc = pthread_create(&bridger_threads[i], NULL, manageToBridger, (void *)(__intptr_t)i);
			if(rc){
				printf("Error creating thread: %d\n", rc);
				exit(-1);
			}
			//bridgerQueueEmpty();
		}
	}else{ //Parent process
		sleep(1);
		semaphore = 0;
		printf("Parent process managing %d bozeman bound threads has PID:  %d\n", TO_BOZEMAN, getpid());
		for(int i = 0;i<TO_BOZEMAN;i++){
			//printf("Creating bozeman bound thread with id: %d\n", i);
			int rc = pthread_create(&bozeman_threads[i], NULL, manageToBozeman, (void *)(__intptr_t)i);
			if(rc){
				printf("Error creating thread: %d\n", rc);
				exit(-1);
			}
			//bozemanQueueEmpty();
		}//end for
	} */
	
    
    pthread_exit(NULL);
    return 0;
}
