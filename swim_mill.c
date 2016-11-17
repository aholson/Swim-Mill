
/* Project: Assignment #3 - Concurrent Processing and Shared Memory
 * File name: swim_mill.c
 * Author: Audrey Olson
 * Student ID: 0161753931
 */

// DUE WEDNESDAY 11/30/2016

#include <stdio.h>

#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//#include "pellet.c"   // pellets at swim mill positions 1+
#include "fish.c"     // fish always at swim mill position 0

#define SHM_SIZE 20   // size 20 of shared memory segment


int main(int argc, char** argv){

  char* shmem;              // shared memory array denoting location
  int pCounter;             // ensures <=20 processes in system at any time 
                            // incremented by fork, decremented by wait
  int key = 1;              // declares key
  
  char swimMill[99];        // declare visible swim mill

  // measure length of computation
  time_t endwait;             
  time_t start = time(NULL);  // returns time since epoch
  time_t duration = 30;       // process ends after 30 seconds
  endwait = start + duration;

  // allocates shared memory segment
  int shmid = shmget(key, SHM_SIZE, IPC_CREAT);

  if( shmid == -1 ){
    perror("shmget");
    exit(1);
  }

  // attaches shared memory segment identified by shmid
  // to the address space of the calling process
  shmem = (char*) shmat(shmid, 0, 0);

  // fills in swim mill
  int i;
  for( i = 0; i < 100; i++ ){
    swimMill[i] = '~';
  }

  // print swim mill
  for( i = 0; i < 100; i++ ){
    if( i%10 == 0 )
      printf("\n%c", swimMill[i]);

    else
      printf("%c", swimMill[i]);
  
    if(i == 99)
      printf("\n\n");
  }

  //fish();
  //pellet(start, endwait);

  // 30 second computation
  //while( start < endwait ){
    
    /* Print result to terminal */

    /* Write result to file after each child is finished */
  
  //}

}

