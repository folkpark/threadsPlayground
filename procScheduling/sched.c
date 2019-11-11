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
#include <gsl/gsl_randist.h>

//Global Variables
pthread_mutex_t cpuMutex; //Global Mutex
int k = 0;
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

void buildQueue(head_t * head){
	for(int i=0;i<num_of_threads;i++){
		struct node *e = malloc(sizeof(struct node));
		e->id = i;
		e->arrivalTime = i+10;
		e->execTime = i+100;
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
			printf("id in the queue: %d\n",e->id);
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
	srand( time(NULL) );
	int index = rand() % 10000 + 1;
	return (guassian_nums[index]);
}

int selectRand(){ //Used for execution time
	//Generates rand numbers from 1-50
	srand( time(NULL) );
	return(rand() % 50 + 1);
}


int main(int argc, char** argv){
	pthread_mutex_init(&cpuMutex, NULL);  // initialize the mutex
	algType = atoi(argv[1]); //Type of algorithm being tested
	
/* 	head_t head;
	TAILQ_INIT(&head);
	
	buildQueue(&head);
	printQueue(&head); */
	buildGaussian();
	
	struct node procDetails[num_of_threads];
	for(int i=0;i<num_of_threads;i++){
		struct node e;
		e.id = i;
		e.arrivalTime = selectRandGauss();
		e.execTime = selectRand();
		procDetails[i] = e;
	}
	
/* 	struct node testNode = procDetails[2];
	int id = testNode.id;
	int aTime = testNode.arrivalTime;
	int eTime = testNode.execTime;
	printf("ID: %d\n",id);
	printf("arrivalTime: %d\n",aTime);
	printf("execTime: %d\n",eTime); */
	
	pthread_t procs[num_of_threads]; // List of threads
	
	switch( algType ){
		case 1: //FIFO
			printf("Evaluating FCFS... \n");
			//Sort the processes based on arrival time
			
			for(int i=0;i<num_of_threads;i++){
				//Give the process an arrival time based on gaussian distribution
				//Give the process a random execTime
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
