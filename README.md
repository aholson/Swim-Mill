
# CECS Assignment #3 - Concurrent Processing and Shared Memory

## Author: Audrey Olson
## PID: 016175391

#### The purpose of this assignment was to learn how to implement multiple
#### processes in conjunction with one another so that they could be used to
#### alter a space in shared memory through the use of the C language in a
#### Linux environment. The swim mill in this project acts as a coordinator
#### process for the fish and pellet processes, the latter of which uses threads
#### to simulate up to 10 pellets in the program.

* To compile:
  * gcc -o "swim`_`mill" swim`_`mill.c
  * gcc -pthread -o "pellet" pellet.c
  * gcc -o "fish" fish.c

* To clean up any allocated shared memory and run:
  * bash kill`_`ipcs.sh
  * ./swim`_`mill

