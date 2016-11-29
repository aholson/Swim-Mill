
/* Project: Assignment #3 - Concurrent Processing and Shared Memory
 * File name: swim_mill.c
 * Author: Audrey Olson
 * Student ID: 0161753931
 */

// DUE WEDNESDAY 11/30/2016

// gcc -o "swim_mill" swim_mill.c

// for solution to no space left on device: bash kill_ipcs.sh

/*
 * TODO
 * - change all execl to execv
 * - print pellet id #, etc.
 * - write to file 
 * - ensure that fish and pellet processes are killed before swim_mill
 */

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 60     // shared memory segment size 15  
                        // fish[0] 
                        // pellets[1-10] 
                        // numPellets[11]
                        // start[12] & endwait[13]
                        // numPelletsCaught[14]

static volatile int keepRunning = 1;

void intHandler(int dummy) {
  keepRunning = 0;
  printf("\nProcess killed.\n");
}
  
int main(void){

  int *shmem;               // shared memory array denoting location
  int shmid;
  int key = 5678;           // declares key 
  int shmdtOut;             // holds shmdt output

  int pCounter;             // ensures <=20 processes in system at any time 
                            // incremented by fork, decremented by wait

  char swimMill[99];        // declare visible swim mill

  // pids for swim mill parent and fish/pellet child processes
  pid_t parentId = getpid();
  pid_t fishId, pelletId;

  // measure length of computation
  clock_t endwait;             
  clock_t start = clock();  // returns start time
  clock_t duration = 15;     // process ends after 30 seconds
  endwait = start + duration;

  // allocates shared memory segment
  shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
  if( shmid < 0 ){
    perror("shmget");
    exit(1);
  }

  // attaches shared memory segment identified by shmid
  // to the address space of the calling process
  shmem = shmat(shmid, NULL, 0);
  if( shmem == (int*) -1 ){
    perror("shmat");
    exit(1);
  } 

  // ensures that all processes only run for 30 second duration
  shmem[12] = start;
  shmem[13] = endwait;
  
  // sets pellets caught count to 0
  shmem[14] = 0;

  signal(SIGINT, intHandler);

  //while( keepRunning ){

  // fork child process for fish
  fishId = fork();

  if( fishId < 0 ){
    perror("cannot fork\n");
    exit(1);
  }
  
  else if( fishId > 0 ){
    //printf("hello from fish parent\n");
    //int status;
    //waitpid(fishId, &status, 0);

    // fork child process for pellet
    pelletId = fork();
  
    if( pelletId < 0 ){
      perror("cannot fork\n");
      exit(1);
    }
  
    else if( pelletId > 0 ){
      //printf("hello from pellet parent\n");
      
      //int status1;
      //waitpid(pelletId, &status1, 0);
      
      sleep(2);

      // 30 second computation
      while( (start < endwait) && keepRunning ){ 

        /* Print result to terminal */

        // fills in or clears swim mill
        int i;
        for( i = 0; i < 100; i++ ){
          swimMill[i] = '~';
        }

        // assign pellet location based on shared mem
        for( i = 1; i <= shmem[11]; i++ ){
          swimMill[shmem[i]] = 'o';
        }

        // assign fish location based on shared mem
        swimMill[shmem[0]] = 'F';
        //printf("fishpos: %d\n", shmem[0]);

        // print swim mill
        for( i = 0; i < 100; i++ ){  
          
          if( i%10 == 0 )
            printf("\n%c", swimMill[i]);

          else
            printf("%c", swimMill[i]);
  
          if(i == 99)
            printf("\n\n");
        }

        sleep(2);
        start++;
        shmem[12] = start; 
        /* Write result to file after each child is finished */
  

        printf("start: %Lf\n", (long double) start);
        printf("endwait: %Lf\n", (long double) endwait);

      }


    }

    else{
      //printf("hello from child - pellet\n");
    
      // executes and leaves pellet process
      int pelletOut = execl("./pellet", "./pellet", NULL);
      if( pelletOut == -1 ){
        perror("execl pellet");
        exit(1);
      }

      _exit(EXIT_FAILURE);
    }

  }

  else{
    //printf("hello from child - fish\n");

    // executes and leaves fish process
    int fishOut = execl("./fish", "./fish", NULL);
    if( fishOut == -1 ){
      perror("execl fish");
      exit(1);
    }

    _exit(EXIT_FAILURE);
  }

  //}

  sleep(2);
  printf("swim mill exited\n");

  shmdtOut = shmdt(shmem);
  if( shmdtOut == -1 ){
    perror("shmdt");
    exit(1);
  }  

  exit(0);

}

