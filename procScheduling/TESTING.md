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

Arrival times were randomly pulled from a set of arrival times that followed a gaussian distribution. A set of 100,000 numbers between 0-k were generated that followed this distribution. Arrival times were then randomly pulled from this set by using a seeded rand() c function. 

The value of k was set to 1000 cpu time units. This variable acts as the available window in which processes may run. 


### Plots

![FCFS](https://github.com/folkpark/threadsPlayground/blob/master/procScheduling/FCFS.jpg)

![SPN](https://github.com/folkpark/threadsPlayground/blob/master/procScheduling/SPN.jpg)

![SRT](https://github.com/folkpark/threadsPlayground/blob/master/procScheduling/SRT.jpg)