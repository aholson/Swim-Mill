
/* Project: Assignment #3 - Concurrent Processing and Shared Memory
 * File name: fish.c
 * Author: Audrey Olson
 * Student ID: 0161753931
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHM_SIZE 44

int main(){

  //pid_t fishpid = fork();
  char *shmem, *s;	// shared memory array denoting location
  int shmid;
  int key = 1;		// declares key

  // allocates shared memory segment
  if( (shmid = shmget(key, SHM_SIZE, IPC_CREAT)) < 0 ){
    perror("shmget");
    exit(1);
  }

  // attach shared memory segment to address space
  if( (shmem = shmat(shmid, NULL, 0)) == (char *) -1){
    perror("shmat");
    exit(1);
  }



}

