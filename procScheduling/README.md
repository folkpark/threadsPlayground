# Benchmarking OS Process Scheduling Algorithms

The object of this assignment is to give practical experience in using synchronization to solve concurrency problems. Details of the assignment can be found [here](https://www.traviswpeters.com/cs460/a2). This document outlines the following:

  - How this software is run
  - The parameters of the simulation
  - Description of the coded solution and results
  - Sample output

### Running the program

- I used a VM running Ubuntu 18.04 LTS
- You can build this software by using the provided makefile, OR
- From the terminal enter: 
```sh
$ gcc -std=gnu99 sched.c -o sched -lpthread -lgsl -lgslcblas -lm
$ ./sched <arg1>
```
-        arg1 = 1 => FCFS
-        arg1 = 2 => SPN
-        arg3 = 3 => SRT


### Simulation Parameters

Arrival times were randomly pulled from a set of arrival times that followed a gaussian distribution. A set of 100,000 numbers between 0-k were generated that followed this distribution. Arrival times were then randomly pulled from this set by using a seeded rand() C function. The execution times were randomly generated to be between 1-100 initially. To adjust the value of d, an offset was used to add a value to the execution times. One hundred processes were used to ensure that there was competition for the processor, but not such a large amount that the ATT time was large. The value of k was set to 1000 cpu time units. This variable acts as the available window in which processes may run. 


### Overview of Coded Solution

A queue was implemented to act as the Ready Queue. The sys/queue.h library was used to implement the queue. A struct was used to store all the information about the process such as arrival time and exection time etc. Once this queue was created it was sorted by either arrival time (for FCFS) or for execution time (SPN and SRT). The algorithm to schedule processes for FCFS and SPN is the exact same, with the exception of how the queue is sorted. The qsort function was used to sort the list of processes. The SRT algorithm is the same as SPN, with the exception that the process at the front of the queue is swapped with the current process if its execution time is less. The queue only needs to be resorted if a new process has arrived. 


### Results/Plots

Please refer to the expected output in the txt files in this repo. Evaluating these files shows that FCFS indeed takes the process that arrives first and executes it. The plots below show that SPN and SRT were able to handle more processes. This is because SPN and SRT were executing the processes that had the shortest execution time or shortest remanining time. 

![FCFS](https://github.com/folkpark/threadsPlayground/blob/master/procScheduling/FCFS.jpg)

![SPN](https://github.com/folkpark/threadsPlayground/blob/master/procScheduling/SPN.jpg)

![SRT](https://github.com/folkpark/threadsPlayground/blob/master/procScheduling/SRT.jpg)

With the given simulation parameters, there was not any significant difference seen between the SPN and SRT scheduling algorithms. This is likely due to the fact that SRT is also executing the shortest execution time first, and would only execute an interupt should a new processes with shorter execution time arrived. With only 100 processes, this did not happen very often if at all. 