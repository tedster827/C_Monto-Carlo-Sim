/*
 * Teddy Williams
 * Feburary 28, 2017
 * Monto Carlo Simulation Using Threads
 * CS 433 Operating Systems
 */
// Including the Standard Library Header File for C
#include <stdlib.h>
// Including the Standard Input/Output Header File for C
#include <stdio.h>
// Including the PThread Header File for C
#include <pthread.h>
// Including the Math Header File for C
#include <math.h>

// PLEASE NOTE: Compile Flags: gcc -Wall -pthread -lm mcarlo.c -o mcarlo

// Declaring a global variable to keep track of the number of variables within
// the circle.
int hitCount;

/*
 * Purpose: This function calculates a double precision random number.
 * Inputs: None.
 * Output: double random value.
 * NOTE: Given within the assignment instructions with permission for
 * use.
 */
double randomDouble() {
  return random()/((double)RAND_MAX+1);
}

/*
 * Purpose: This function finds how many point are inside the circle.
 * Inputs: None.
 * Output: None (Void Function).
 * NOTE:
 * This is a function that is called by a thread.
 * A segment of this code was taken from the assignment instructions with
 * permission for use.
 */
void *pointChecking(void *numPoints) {
  printf("Child Thread Successfully Created\n");
  // NOTE: void* is a point so to display, use %p.
  // printf("Requested Number of Points: %i\n", numPoints);
  // x and y values for graph
  double xPoint = 0;
  double yPoint = 0;
  // for Loop counter
  int i;
  // Casting here so it's not in the loop.
  int* iNumPoints = (int*)numPoints;

  printf("%i\n", *iNumPoints);
  // Checking for points inside the circle
  for(i = 0; i < *iNumPoints; i++) {
    // Generating random numbers between -1 and 1.
     xPoint = (randomDouble() * 2.0) - 1.0;
     yPoint = (randomDouble() * 2.0) - 1.0;
    // Using the distance formula to check for hits within circle
    if(sqrt((xPoint * xPoint)+(yPoint * yPoint)) < 1.0) {
      // Updating shared global variable.
      ++hitCount;
      printf("Point: (%f,%f)\n", xPoint, yPoint);
      printf("hitCount within pointChecking: %i.\n", hitCount);
    }
  }
  // Terminating Child Thread.
  pthread_exit(0);
}

/*
 * Purpose: This function will get the pointer array given by the user and
 * convert it to an integer.
 * Inputs:
 * char const arrayONumbers[]
 * The pointer array given by the user
 * Output: The resulting value of the convertion
 * NOTE:
 * Does not work with negative numbers.
 * Taken from a previous assignment from this class written by Teddy Williams
 * (me).
 */
 int arguInterrupter(char const arrayONumbers[]) {
   // Final result variable
   int result = 0;
   // Counter
   int i;
   // Getting size and making sure there are no letters
   for(i = 0; arrayONumbers[i] != 0; i++) {
     // If something isn't a letter it will terminate function.
     if(arrayONumbers[i] < 47 || arrayONumbers[i] > 58){
       return -1;
     }
   }
   // Storing Size
   int size = i;
   // Index storage for exponent.
   int index = size - 1;
   // Gathering input to make an integer.
   for(i = 0; i < size; i++) {
     result = ((arrayONumbers[i] + '0' - 96)*pow(10,index--))+result;
   }
   // Returning Number
   return result;
 }
 /*
  * Purpose: This function is the main driver function that will handle execution
  * arguments.
  * Inputs: The integer argc is the number of arguments that are passing using the
  * execution command. The char const* (or C string array) stores all the arguments.
  * Output: Exit Code (0)
  */
int main(int argc, char const *argv[]) {
  if(argc == 2) {
    // int requestNumPoints = malloc(sizeof(requestNumPoints));
    int requestNumPoints = arguInterrupter(argv[1]);
    if(requestNumPoints >= 0) {
      // Creating a new thread identifier.
      pthread_t tid;
      // Creating a set of thread attributes.
      pthread_attr_t attr;
      // Getting the default attributes for the pthread.
      pthread_attr_init(&attr);
      // Creating the new pthread
      // Calling a function that calculates the number of points that hit within
      // the circle.
      pthread_create(&tid, &attr, pointChecking, (void*)&requestNumPoints);
      // Making the parent thread wait for the child to finish.
      pthread_join(tid, NULL);
      printf("Child Thread Terminated. Parent is no longer waiting.\n");
      double pi;
      printf("Hit Count: %i\n", hitCount);
      pi = (double)(4 * hitCount)/requestNumPoints;
      printf("Estimated Value of PI: %f.\n", pi);
    }
    else {
      // Error Message for...
      printf("Invalid Number of Points\n");
      printf("%s Is Invalid.\n", argv[1]);
      printf("Usage: ./mcarlo <number of points>\n");
      printf("Note: Number of Points Must Be Positive\n");
      // Exit Call (-1) Error
      return -1;
    }
  }
  else{
    // Error Messafe for...
    printf("Invalid Number of Arguments\n");
    printf("Usage: ./mcarlo <number of points>\n");
    // Exit Call (-1) Error
    return -1;
  }
  // Exit Call (0)
  return 0;
}
