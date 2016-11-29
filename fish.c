
/* Project: Assignment #3 - Concurrent Processing and Shared Memory
 * File name: fish.c
 * Author: Audrey Olson
 * Student ID: 0161753931
 */

// gcc -o "fish" fish.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 60

int main(){

  int *shmem;  	  // shared memory array denoting location
  int shmid;
  int key = 5678; // declares key
  int shmdtOut;   // holds shmdt output

  int i;            // for loop iteration
  int closest;      // denotes mem of closest pellet to fish
  int fishColumn;

  int pellColumn;
  int pellRow;
  int pellDist;     // relative displacement from fish
  
  int closestColumn;
  int closestRow;
  int closestDist;

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

  // change shared memory to denote fish's position
  // (locations 90-99 denoted bottom row)
  printf("%s", "hello from fish\n");
  shmem[0] = 94;
  
  closest = 1;  
  //printf("%s = %d\n", "fish shmem (fish) ", shmem[0]);

  // keeps fish running for duration of timer
  while( shmem[12] < shmem[13] ){
    
    fishColumn = shmem[0]%10;
    

    // scan shmem and direct fish to column of closest pellet
    for( i = 1; i <= shmem[11]; i++ ){
      
      pellColumn = shmem[i]%10;
      pellRow = (shmem[i]/10)%10;
      closestColumn = shmem[closest]%10;
      closestRow = (shmem[closest]/10)%10;

      //printf("pell: %d x %d\n", pellColumn, pellRow);
      //printf("closest: %d x %d\n", closestColumn, closestRow);

      // closest pellet algo
      // 1. biggest row number = closest by default
      if( (pellRow > closestRow) && (pellRow != 9) ){
      
        closest = i;
        closestColumn = pellColumn;

      }

      // 2. same row number =
      //    a. closest column number relative to fish
      //    - smallest abs(fish column - pellet column)
      //    b. if 2 equally distant pellets, leftmost/smaller column chosen
      else if( pellRow == closestRow ){

        pellDist = abs(fishColumn-pellColumn);
        closestDist = abs(fishColumn-closestColumn);

        // if current pellet is closer than closest pellet
        // closest is current pellet
        if( pellDist < closestDist ){
          closest = i;
          closestColumn = pellColumn;
        }
        
        // if current and closest pellet are equally distant
        else if( pellDist == closestDist ){

          // closest is leftmost pellet
          if( shmem[i] < shmem[closest] ){
            closest = i;
            closestColumn = pellColumn;
          }

        }

      }
      
    }
    
    // move fish one unit closer to closest pellet
    if( closestColumn > fishColumn )
      shmem[0]++;

    else if( closestColumn < fishColumn )
      shmem[0]--;

    //printf("closestColumn: %d\n", closestColumn);
    sleep(2);
  }

  printf("fish exited\n");
  shmdtOut = shmdt(shmem);
  if( shmdtOut == -1 ){
    perror("shmdt");
    exit(1);
  }
 
  exit(0);

}

