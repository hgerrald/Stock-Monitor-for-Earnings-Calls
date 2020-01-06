/*
 * Splits the workload into processes instead of threads
 * Uses pipes to transfer data
 * Stock prices will be saved in "prices"
 * "tickers" is this list of strings containing the stock tickers to track
 */

#include "main.h"

// GLOBAL VARIABLES
extern double *new_prices;
extern double *original_prices;
extern char **tickers;
extern int totalTickers;
extern int *ps_variables;
extern pid_t parent_pid;
//int **fd;


/******************************************************************************
 * The child process will switch into this function after grabbing the prices
 * the first go around
 * Continuously waits for a signal and finds/sends prices once triggered
 */
void GetNewPricesProcesses(int *start, int *end, int i)
{
   while(1)
   {
     while (ps_variables[i] == 0); // Wait for the process to be signaled
     FillArrayWithPrices(new_prices, start, end);
     ps_variables[i] = 0; // Turn the signal back off
   }
}

/******************************************************************************
 * Fork to create new processes to distribute the workload
 * These new processes should not exit, but rather call GetNewPrices so the
 * new prices are continuously updated
 */
void SplitIntoProcesses()
{
  int start, end, i;
//  int status;
  pid_t *pid;
  pid = (pid_t*)malloc(sizeof(pid_t) * NUM_PROCESSES);

// Malloc for and open pipes
  // fd = (int**)malloc(sizeof(int*) * NUM_PROCESSES);
  // for (i = 0; i < NUM_PROCESSES; i++)
  // {
  //   fd[i] = (int*)malloc(sizeof(int) * 2);
  //   if (pipe(fd[i])==-1)
  //   {
  //       fprintf(stderr, "Pipe Failed" );
  //       return;
  //   }
  // }

// Create child processes
  for (i = 0; i < NUM_PROCESSES; i++)
  {
    if ((pid[i] = fork()) == 0)
    {
      // Child process
      pid[i] = getpid();
      if (i == 0) start = 0;
      else
      {
        start = i * totalTickers / NUM_PROCESSES;
        if (i < totalTickers / NUM_PROCESSES)
          start++;
      }

      end = (i+1) * totalTickers / NUM_PROCESSES - 1;
      if (i+1 != NUM_PROCESSES && totalTickers % NUM_PROCESSES != 0)
        end++;


      FillArrayWithPrices(original_prices, &start, &end);
    //  SendData(&fd[i][0], numTickers, original_prices, start);
      ps_variables[i] = 0; // Signal the process is done
      GetNewPricesProcesses(&start, &end, i);
    }


  }

   // Parent process
  for (i = 0; i < NUM_PROCESSES; i++)
  {
    while (ps_variables[i] == 1);
    ps_variables[i] = 0; // Make the process idle again
//    ReceiveData(&fd[i][0], original_prices);
  }


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
