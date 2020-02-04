# The Bridger Canyon Drive Construction Project

The object of this assignment is to give practical experience in using synchronization to solve concurrency problems. Details of the assignment can be found [here](https://www.traviswpeters.com/cs460/a1). This document outlines the following:

  - How this software is run
  - My overall strategy and design for solving this problem
  - How the final program was tested
  - Sample output

### Running the program

- I used a VM running Ubuntu 18.04 LTS
- From the terminal enter: 
```sh
$ gcc -std=c99 road.c -o road -lpthread
$ ./road <arg1> <arg2> <arg3>
```
-        arg1: # of cars going to Bridger
-        arg2: # of cars going to Bozeman
-        arg3: MAX number of cars allowed on bridge.


### Overall Design

Create a single thread representing a car. Multiple cars are allowed on the one-way bridge at a time, but cars must enter the bridge one at a time. My strategy for this problem was to imagine that there was a constructor worker at each side of the bridge that could signal back and forth for vehciles to travel across. I used two functions to represent this 'manageToBridger()' and 'manageToBozeman()'. Each function is responsible for getting their vehicles across the bridge by using a semaphore to signal the other function that they have completed their batch of cars to cross. Since only one car can enter the bridge at a given time, two mutexes were used to prevent more than one thread entering the critical section at a time. 

## Testing

All the requirements for this assignment were tested and working as designed. Below is a description of how the requirement was satisfied and/or with what tests to confirm that the requirement was met. 

#### Synchronization

I made no use of any monitors or separate threads that manage the car threads. When the program begins, the only threads that are created represent the cars at each side of the bridge. The cordination of these threads is dictated by the dance between the two functions who signal each other through a semaphore. 

#### Safety

Cars must not collide in the one-way and may not have more cars on the bridge than the specified MAXCARS value that is passed in by the user as the third argument. Safety was satisfied because when a car enters the bridge it is counted using a global variable. When the bridge's car limit has been reached the last thread will exit the bridge, but before releasing its lock on the mutex it signals the other side via a semaphore and loops until the other side has flipped the semaphore value back. This was tested using different values for MAXCARS. The current amount of cars on the bridge is printed to the screen to verify that the limit was never exceeded. 

#### Liveness

All cars that enter the bridge are immedietly counted and allowed to cross the bridge. Cars sit and wait to enter the bridge, because their thread is suspended by the mutex until the car in front of it unlocks it. This was tested successully be confirming that the result of each input returned me back to the terminal input. This indicates that all threads were allowed to cross at some point. I also tested against the case were there was only a few cars on one side and a very large number on the other. In this case the semaphore is permanetly flipped back to the other side once all its cars have passed, which allows all the cars on the other side to just cross in MAXCAR batch sizes. 

#### Efficiency

The efficiency of the program is dictated by the MAXCARS parameter. When the MAXCARS value is set to 5 for example, then 5 cars waiting to go across are allowed to cross as a batch. Once that batch of cars has passed, the semaphore signals the other function to send its cars across coming the other way in a batch of the same size. 

### Example Test Cases

| Input Parameters | Tested result |
| ------ | ------ |
| ./road 10 10 3 | Every three cars that pass, the other side takes turn |
| ./road 100 100 5 | All cars passed and flipped sides every 5 cars |
| ./road 5 100 3 | Once the bridger cars pass, all remaining cars go in batches <= MAXCARS|
| ./road 2 10 5 | Cars pass and signal even though the MAXCAR value not reached |
| ./road 10 2 5 | Other side honors the same rule as above which maintains liveness |
| ./road 11 11 100 | Odd numbers handled and cars pass in batches of 11 |
