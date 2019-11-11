/*
Author: Parker Folkman

To build/run this program:
gcc -std=c99 sched.c -o sched -lpthread -lgsl -lgslcblas -lm
./sched <algType>

*/

#include <stdio.h>     // printf()
#include <stdlib.h>    // exit(), qsort()
#include <pthread.h> //threads
#include <sys/queue.h> //queues
#include <gsl/gsl_rng.h> //gaussian nums
#include <gsl/gsl_randist.h> //gaussian nums
#include <time.h>

//Global Variables
pthread_mutex_t cpuMutex; //Global Mutex
int algType = 0;
int guassian_nums[10000];
int num_of_threads =  5;

typedef struct node{ //Used to implement a Queue of procs
		int id;
		int arrivalTime;
		int execTime;
		TAILQ_ENTRY(node) nodes;
} node;

typedef TAILQ_HEAD(head_s, node) head_t;

void *fifoScheduler(void *threadid){
	long tid;
    tid = (long)threadid;
	printf("Thread_id: %ld\n",tid);
	//pthread_exit(NULL);
}

void *spnScheduler(void *threadid){
	
}

void *srtScheduler(void *threadid){
	
}

void buildQueue(head_t * head, struct node *procDetails){
	for(int i=0;i<num_of_threads;i++){
		struct node *e = malloc(sizeof(struct node));
		e->id = procDetails[i].id;
		e->arrivalTime = procDetails[i].arrivalTime;
		e->execTime = procDetails[i].execTime;
		TAILQ_INSERT_TAIL(head,e,nodes);
		e = NULL;
	}
}

int compareArrivalTime(const void *a, const void *b){
	struct node *ia = (struct node *)a;
	struct node *ib = (struct node *)b;
	return (ia->arrivalTime > ib->arrivalTime) - (ia->arrivalTime < ib->arrivalTime);
}

void printQueue(head_t * head){
	struct node * e = NULL;
	TAILQ_FOREACH(e,head,nodes){
			printf("Thread_ID: %d\n",e->id);
			printf("arrivalTime: %d\n",e->arrivalTime);
			printf("ExecTime: %d\n",e->execTime);
	}
}

void buildGaussian(){ //Builds a list of ints following a gaussian distribution
	const gsl_rng_type * T;
    gsl_rng * r;
    double sigma = 100; 
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    /*
     *  Print n random variates chosen from the gaussian distribution
     *  with mean zero and standard deviation sigma.
     */
    for (int i = 0; i < 10000; i++) {
        double x = gsl_ran_gaussian(r, sigma);
		x += 500;
		int y = (int)x;
        guassian_nums[i] = y;
    }
	gsl_rng_free (r);
}

int selectRandGauss(){ //Selects random int from gaussian distribution
	int index = rand() % 10000 + 1;
	return (guassian_nums[index]);
}

int selectRand(){ //Used for execution time
	//Generates rand numbers from 1-50
	return(rand() % 50 + 1);
}

void printNodes(struct node *procDetails, int x){
	x = num_of_threads;
	for(int i=0;i<x;i++){
		printf("i = %d\n",i);
		printf("Thread_ID: %d, arrivalTime: %d, execTime: %d\n",procDetails[i].id, procDetails[i].arrivalTime, procDetails[i].execTime);
	}
}

int main(int argc, char** argv){
	pthread_mutex_init(&cpuMutex, NULL);  // initialize the mutex
	algType = atoi(argv[1]); //Type of algorithm being tested
	
	srand( time(NULL) );
	buildGaussian();
	
	/* 1. Give the process an arrival time based on gaussian distribution
	   2. Give the process a random execTime
	   3. put these into a list of nodes*/
	struct node procDetails[num_of_threads];
	for(int i=0;i<num_of_threads;i++){
		struct node e;
		e.id = i;
		e.arrivalTime = selectRandGauss();
		e.execTime = selectRand();
		procDetails[i] = e;
	}
	
	head_t head;
	TAILQ_INIT(&head);
	pthread_t procs[num_of_threads]; // List of threads
	
	switch( algType ){
		case 1: //FIFO
			printf("Evaluating FCFS... \n");
			//Sort the process details based on arrival time
			qsort(procDetails,num_of_threads,sizeof(struct node),compareArrivalTime);
			//printNodes(procDetails,num_of_threads);
			buildQueue(&head, procDetails);
			printQueue(&head);
			for(int i=0;i<num_of_threads;i++){
				int rc = pthread_create(&procs[i], NULL, fifoScheduler, (void *)(__intptr_t)i);
				if(rc){
					printf("Error creating thread: %d\n", rc);
					exit(-1);
				}
			}
			break;
			
		case 2: //SPN
			//do case 2
			break;
			
		case 3: //SRT
			//do case 3
			break;
			
		default:
			printf("Not a valid algType entry \n"); 
	}

	
    pthread_exit(NULL);
    return 0;
}
