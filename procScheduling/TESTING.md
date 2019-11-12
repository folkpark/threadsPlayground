# Benchmarking OS Process Scheduling Algorithms

This document outlines what steps were taken to test this software and verify its validity. 

### Testing the program

The program mainly utilized unit tests to test each function in the process. In order to visualize what is happening in the program several functions were written to print the state to the screen. Most of these functions do not print to the screen in the final resulting output, but rather were used to incrementally test as the code was being written. The printNodes function prints the contents of a list of nodes (the struct for a process) to the screen. This function was written to test how the nodes were sorted, since the compareArrivalTime and compareExecTime functions were written to compare values of the nodes. Additionally, the printQueue function was written to print the contents of the Ready Queue to the screen. This ensured that the processes being inserted into the Ready Queue were indeed inserted in the expected order. 

The simulation parameters were tuned such that meaningful results were obtained. The value for k and number of threads were modified to allow for competititon for the processor, but also keeping the average turnaround time (ATT) at a reasonable level. 

