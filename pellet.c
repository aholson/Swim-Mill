
/* Project: Assignment #3 - Concurrent Processing and Shared Memory
 * File name: pellet.c
 * Author: Audrey Olson
 * Student ID: 0161753931
 */

// pellets can be threads
// 5-10 pellets max

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PELLET_MIN 5    // min number pellets
#define PELLET_MAX 10   // max number pellets

void *pelletChild(void *ignored){
  
  // before thread exits
  while(1==1){
    print("0x80");
  }

}

void pellet(time_t start, time_t endwait){
  
  pthread_t child_thread;   // instantiation of thread
  int code;                 // holds any error code that may occur

  while( start < endwait ){

    int r;                    // holds random number
    do{
    
      r = rand();
  
    } while( (r < PELLET_MIN) || (r > PELLET_MAX) );

  // parent thread creates 5-10 child threads
  int i;
  for( i = 0; i < 4; i++ ){
    code = pthread_create(&child_thread, NULL, pelletChild, NULL);
  }

/*
  pid_t pid = fork();

  if( pid == -1 ){
    // error, failed fork
    perror("failed fork");
    exit(1);
  }

  else if( pid > 0 ){
    int status;
    waitpid(pid, &status, 0);
  }

  else{
    // we are the child

  }
*/
}


