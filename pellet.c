
/* Project: Assignment #3 - Concurrent Processing and Shared Memory
 * File name: pellet.c
 * Author: Audrey Olson
 * Student ID: 0161753931
 */

// pellets can be threads
// 5-10 pellets max

// gcc -pthread -o "pellet" pellet.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PELLET_MIN 5    // min number pellets
#define PELLET_MAX 10   // max number pellets
#define SHM_SIZE 60     // size 15 shmem

// global vars accessible by both parent process and threads

int pellet;
int *shmem;         // shared memory array denoting location
int shmid;
int key;
int shmdtOut;       // holds shmdt output

//bool cancel;

void *pelletChild(void *ignored){
 
  int pos;
  int thisPellet = pellet;              // denotes unique pellet
  //pid_t pid = syscall(SYS_gettid);   // denotes thread id

  // generate random pellet location in first 9 rows
  srand(time(NULL));
  int r = rand() % 90;
  
  // ensure pellet does not overlap with another
  int i;
  for( i = 0; i < pellet; i++ ){
    while( r == shmem[i] ){
      r = rand() % 90;
    }
  }

  shmem[thisPellet] = r;
  shmem[11] = thisPellet;

  printf("shmem[%d] = %d\n", thisPellet, shmem[thisPellet]);
  
  // keep thread running until cancelled
  while( shmem[12] < shmem[13] ){
    sleep(2);
    
    /* TODO fix so that no overlap with other pellets */
    // if pellet has left range of mill
    if( shmem[thisPellet] > 99 ){
      
      // Each pellet's status printed before exiting
      /*if( shmem[thisPellet] < 110 ){
        printf("pellet ID: %Lf\n", (long double) pid);
        printf("pellet position: %d\n", shmem[thisPellet]-10);
        printf("pellet status: MISSED\n");
      }
      else{
        printf("pellet ID: %Lf\n", (long double) pid);
        printf("pellet position: %d\n", shmem[0]);
        printf("pellet status: EATEN\n");
      }*/

      // move back to random position
      srand(time(NULL));
      for( i = 0; i < pellet; i++ ){
        while( r == shmem[i] ){
          r = rand() % 90;
        }
      }

      shmem[thisPellet] = r;

    }

    // if next position doesnt overlap with fish
    pos = shmem[thisPellet];
    if( (pos += 10) != shmem[0] )  
      // move pellet one row down 
      shmem[thisPellet] += 10;
    
    else {
      // or move pellet out of range of mill
      shmem[thisPellet] = 110;
      
      //increment pelletsCaughtCount
      shmem[14]++;
      //printf("pellet caught!: %d\n", shmem[14]);
    }
    
    //printf("location[%d]: %d\n", thisPellet, shmem[thisPellet]);

  } 

}

int main(){
  
  pthread_t child_thread;   // instantiation of thread
  int code;                 // holds any error code that may occur

  key = 5678;     // declares key
  pellet = 1;     // tracks shmem locations of pellet (1-10)

  // allocates shared memory segment
  if( (shmid = shmget(key, SHM_SIZE, 0666)) < 0 ){
    perror("shmget");
    exit(1);
  }   

  // attach shared memory segment to address space
  if( (shmem = shmat(shmid, NULL, 0)) == (int *) -1){
    perror("shmat");
    exit(1);
  }

  // change shared memory to denote pellet positions
  printf("%s", "hello from pellet\n");

  // parent thread creates up to 10 child threads
  while( (pellet <= PELLET_MAX) && (shmem[12] < shmem[13])){
    
    code = pthread_create(&child_thread, NULL, pelletChild, NULL);
    if(code){
      fprintf(stderr, "pthread_create failed with code %d\n", code);
    }
    
    sleep(7);   // allow first pellet to run before creating second
                // 30 should divide by this number evenly
    pellet++;

    // send thread cancellation request
    //if( cancel ){
    //}
  }

  // keep process running until cancelled
  //while( shmem[12] < shmem[13] ){
    //printf("shmem[12]: %Lf\n", (long double) shmem[12]);
    //printf("shmem[13]: %Lf\n", (long double) shmem[13]);
    //sleep(1);
  //}

  printf("# pellets caught: %d\n", shmem[14]);

  shmdtOut = shmdt(shmem);
  if( shmdtOut == -1 ){
    perror("shmdt");
    exit(1);
  }  

  exit(0);
}


