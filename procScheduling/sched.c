/*
Author: Parker Folkman

To build/run this program:
gcc -std=c99 sched.c -o sched -lpthread -lgsl -lgslcblas -lm
./sched <algType>

*/

#include <stdio.h>     // printf()
#include <stdlib.h>    // exit(), qsort()
#include <sys/queue.h> //queues
#include <gsl/gsl_rng.h> //gaussian nums
#include <gsl/gsl_randist.h> //gaussian nums
#include <time.h> //clock()

//Global Variables
int algType = 0;
int guassian_nums[10000];
int num_of_threads =  100;
int k = 1000;
double d;
int printed = 1;

typedef struct node{ //Used to implement a Queue of processes
		int id;
		int arrivalTime;
		int execTime;
		TAILQ_ENTRY(node) nodes;
} node;

typedef TAILQ_HEAD(head_s, node) head_t;

void *fifo_spn_Scheduler(head_t * head){
	
	FILE *outfile;
	
	if(algType==1){
		outfile = fopen("FCFS_Data.csv","a");
	}else{
		outfile = fopen("SPN_Data.csv","a");
	}
	
	int TT;
	int TT_arr[num_of_threads];
	struct node *e  = NULL;
	clock_t cpuStart, cpuEnd, procStop, procStart;
	cpuStart = clock();
	cpuEnd = cpuStart + k; //Window of availability
	int remaingCPU;
	int counter = 0;
	
	while(!TAILQ_EMPTY(head) && clock() < cpuEnd){
		e = TAILQ_FIRST(head);
		procStart = clock() - cpuStart;
		while(procStart < e->arrivalTime){ //Wait until the next proc is ready
			procStart = clock() - cpuStart;
		}
		//printf("procStart = %ld\n", procStart);
		procStop = procStart + e->execTime;
		while(clock() < procStop){
			//Process performs its action here
		}
		remaingCPU = cpuEnd - clock();
		TT = procStop - e->arrivalTime; //Calc turn around time
/* 		if(printed){
			printf("Proc_ID: %d, Proc_Arrive: %d, Proc_ExecTime: %d, Remaing_Time: %d, TT: %d\n", 
			e->id, e->arrivalTime,e->execTime, remaingCPU,TT);
		} */
		TT_arr[counter] = TT;
		counter +=1;
		TAILQ_REMOVE(head,e,nodes);
		free(e);
		e = NULL;
	}
	int TTsum = 0;
	for(int i=0;i<num_of_threads;i++){
		TTsum += TT_arr[i];
	}
	double ATT = TTsum/num_of_threads;
	double d_ratio = d/ATT;
	fprintf(outfile,"%d,%f\n",(int)d,d_ratio);
	fclose(outfile);
	for(int i=0;i<num_of_threads;i++){TT_arr[i] = 0;}
	ATT = 0;
	//printed = 0;
	d_ratio = 0;
	d = 0;
}

void *srtScheduler(head_t * head){
	FILE *outfile;
	outfile = fopen("SRT_Data.csv","a");
	
	int TT;
	int TT_arr[num_of_threads];
	struct node *e  = NULL;
	clock_t cpuStart, cpuEnd, procStop, procStart;
	cpuStart = clock();
	cpuEnd = cpuStart + k; //Window of availability
	int remaingCPU;
	int counter = 0;
	
	while(!TAILQ_EMPTY(head) && clock() < cpuEnd){
		e = TAILQ_FIRST(head);
		procStart = clock() - cpuStart;
		while(procStart < e->arrivalTime){ //Wait until the next proc is ready
			procStart = clock() - cpuStart;
		}
		//printf("procStart = %ld\n", procStart);
		procStop = procStart + e->execTime;
		while(clock() < procStop){
			//Process performs its action here
		}
		remaingCPU = cpuEnd - clock();
		TT = procStop - e->arrivalTime; //Calc turn around time
		if(printed){
			printf("Proc_ID: %d, Proc_Arrive: %d, Proc_ExecTime: %d, Remaing_Time: %d, TT: %d\n", 
			e->id, e->arrivalTime,e->execTime, remaingCPU,TT);
		}
		TT_arr[counter] = TT;
		counter +=1;
		TAILQ_REMOVE(head,e,nodes);
		free(e);
		e = NULL;
	}
	int TTsum = 0;
	for(int i=0;i<num_of_threads;i++){
		TTsum += TT_arr[i];
	}
	double ATT = TTsum/num_of_threads;
	double d_ratio = d/ATT;
	fprintf(outfile,"%d,%f\n",(int)d,d_ratio);
	fclose(outfile);
	for(int i=0;i<num_of_threads;i++){TT_arr[i] = 0;}
	ATT = 0;
	printed = 0;
	d_ratio = 0;
	d = 0;
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

int compareExecTime(const void *a, const void *b){
	struct node *ia = (struct node *)a;
	struct node *ib = (struct node *)b;
	return (ia->execTime > ib->execTime) - (ia->execTime < ib->execTime);
}

void printQueue(head_t * head){
	struct node * e = NULL;
	TAILQ_FOREACH(e,head,nodes){
			printf("Thread_ID: %d",e->id);
			printf(" arrivalTime: %d",e->arrivalTime);
			printf(" ExecTime: %d\n",e->execTime);
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

int selectRand(int offset){ //Used for execution time
	//Generates rand numbers from 1-50
	return(rand() % 50 + 1 +offset);
}

void printNodes(struct node *procDetails, int x){
	x = num_of_threads;
	for(int i=0;i<x;i++){
		printf("i = %d\n",i);
		printf("Thread_ID: %d, arrivalTime: %d, execTime: %d\n",procDetails[i].id, procDetails[i].arrivalTime, procDetails[i].execTime);
	}
}

void calc_d(struct node *procDetails){
	int sum;
	for(int i=0;i<num_of_threads;i++){
		sum += procDetails[i].execTime;
	}
	d = sum/num_of_threads;
}

int main(int argc, char** argv){
	algType = atoi(argv[1]); //Type of algorithm being tested
	
	srand( time(NULL) );
	buildGaussian();
	int trials = 25*3;
	for(int offset=0;offset<trials;offset+=3){
			/* 1. Give the process an arrival time based on gaussian distribution
		   2. Give the process a random execTime
		   3. put these into a list of nodes*/
		struct node procDetails[num_of_threads];
		for(int i=0;i<num_of_threads;i++){
			struct node e;
			e.id = i;
			e.arrivalTime = selectRandGauss();
			e.execTime = selectRand(offset);
			procDetails[i] = e;
		}
		
		head_t head; //Head of the queue
		TAILQ_INIT(&head);
		pthread_t procs[num_of_threads]; // List of threads
		calc_d(procDetails);
		
		switch( algType ){
			case 1: //FIFO
				printf("Evaluating FCFS... \n");
				//Sort the process details based on arrival time
				qsort(procDetails,num_of_threads,sizeof(struct node),compareArrivalTime);
				//printNodes(procDetails,num_of_threads);
				buildQueue(&head, procDetails);
				//printQueue(&head);
				fifo_spn_Scheduler(&head);
				break;
				
			case 2: //SPN
				printf("Evaluating SPN... \n");
				//Sort the process details based on arrival time
				qsort(procDetails,num_of_threads,sizeof(struct node),compareExecTime);
				buildQueue(&head, procDetails);
				//printQueue(&head);
				fifo_spn_Scheduler(&head);
				break;
				
			case 3: //SRT
				printf("Evaluating SRT... \n");
				qsort(procDetails,num_of_threads,sizeof(struct node),compareExecTime);
				buildQueue(&head, procDetails);
				//printQueue(&head);
				srtScheduler(&head);
				break;
				
			default:
				printf("Not a valid algType entry \n"); 
		}
	}

    return 0;
}
