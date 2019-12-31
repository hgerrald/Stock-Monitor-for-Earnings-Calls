/*
 * Splits the workload into processes instead of threads
 * Uses pipes to transfer data
 * Stock prices will be saved in "prices"
 * "tickers" is this list of strings containing the stock tickers to track
 */

#include "main.h"

#define NUM_PROCESSES 8

void SplitIntoProcesses(char** tickers, int totalTickers, double* prices)
{
  int numTickers, status, start, end, i, x;
  pid_t pid[NUM_PROCESSES];
  int **fd;

// Malloc for and open pipes
  fd = (int**)malloc(sizeof(int*) * NUM_PROCESSES);
  for (i = 0; i < NUM_PROCESSES; i++)
  {
    fd[i] = (int*)malloc(sizeof(int) * 2);
    if (pipe(fd[i])==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return;
    }
  }

// Create child processes
  for (i = 0; i < NUM_PROCESSES; i++)
  {
    if ((pid[i] = fork()) == 0)
    {
      // Child process
      x = i;
      pid[i] = getpid();
      if (x == 0) start = 0;
      else
      {
        start = x * totalTickers / NUM_PROCESSES;
        if (x < totalTickers / NUM_PROCESSES)
          start++;
      }

      end = (x+1) * totalTickers / NUM_PROCESSES - 1;
      if (x+1 != NUM_PROCESSES && totalTickers % NUM_PROCESSES != 0)
        end++;

      numTickers = end - start + 1;
      FillArrayWithPrices(prices, numTickers, tickers, start, end);
      SendData(&fd[i][0], numTickers, prices, start);
      exit(0); // Change this to wait until triggered to restart
    }
  }

   // Parent process
  for (i = 0; i < NUM_PROCESSES; i++)
     waitpid(pid[i], &status, 0);

  for (i = 0; i < NUM_PROCESSES; i++)
     ReceiveData(&fd[i][0], prices);



}


/******************************************************************************
 * Sends data through a pipe
 */
void SendData(int *fd, int numTickers, double* tickerArray, int start){

  // Send the number of tickers and the index in this process
   write(fd[1], &numTickers, sizeof(int));
   write(fd[1], &start, sizeof(int));

   for (int i = start; i < numTickers + start; i++)
     write(fd[1], &tickerArray[i], sizeof(double));

   close(fd[1]);
}

/******************************************************************************
 * Receives data through a pipe
 */
void ReceiveData(int *fd, double* data_array){

  int numTickers, start;

  read(fd[0], &numTickers, sizeof(int));
  read(fd[0], &start, sizeof(int));

  for (int i = start; i < numTickers + start; i++)
       read(fd[0], &data_array[i], sizeof(double));

  close(fd[0]);
}
