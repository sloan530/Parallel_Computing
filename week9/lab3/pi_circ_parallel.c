/*
 * Purpose: Estimate the value of PI
 *

 * Input:     None
 * Output:    Estimate of pi
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/* Serial function */
void Get_args(char *argv[],
              long long int *number_of_tosses_p, int *thread_count);
void Usage(char *prog_name);

long long int Count_hits(long long int number_of_tosses, int thread_count);

/*---------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
   double pi_estimate;
   int thread_count;
   long long int number_in_circle;
   long long int number_of_tosses;

   if (argc != 3)
      Usage(argv[0]);
   Get_args(argv, &number_of_tosses, &thread_count);

   number_in_circle = Count_hits(number_of_tosses, thread_count);

   pi_estimate = 4 * number_in_circle / ((double)number_of_tosses);
   printf("Estimated pi: %e\n", pi_estimate);

   return 0;
}

/*---------------------------------------------------------------------
 * Function:      Count_hits
 * Purpose:       Calculate number of hits in the unit circle
 * In arg:        number_of_tosses
 * Return val:    number_in_circle
 */

long long int Count_hits(long long int number_of_tosses, int thread_count)
{

   long long int number_in_circle = 0;

/* START OF CODE TO BE PARALLELIZED */
#pragma omp parallel num_threads(thread_count)

   unsigned int seed = omp_get_thread_num(); // in a parallel program this should be seeded to the rank of the thread
   double x, y, distance_squared;
   long long int toss;

#pragma omp for num_thread(thread_count) \
    reduction(+                          \
              : number_in_circle)
   for (toss = 0; toss < number_of_tosses; toss++)
   {
      x = (double)rand_r(&seed) / RAND_MAX;
      y = (double)rand_r(&seed) / RAND_MAX;
      distance_squared = x * x + y * y;
      if (distance_squared <= 1)
         number_in_circle++;
   }

   /* END OF CODE TO BE PARALLELIZED */

#ifdef DEBUG
   printf("Total number in circle = %lld\n", number_in_circle);
#endif

   return number_in_circle;
} /* Count_hits */

/*---------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message showing how to run program and quit
 * In arg:    prog_name:  the name of the program from the command line
 */

void Usage(char prog_name[] /* in */)
{
   fprintf(stderr, "usage: %s ", prog_name);
   fprintf(stderr, "<total number of tosses>\n");
   exit(0);
} /* Usage */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argv
 * Out args:    number_of_tosses_p
 */

void Get_args(
    char *argv[] /* in  */,
    long long int *number_of_tosses_p /* out */,
    int thread_count)
{

   *number_of_tosses_p = strtoll(argv[1], NULL, 10);
   *thread_count = strtoi(argv[2], NULL, 10);
} /* Get_args */
