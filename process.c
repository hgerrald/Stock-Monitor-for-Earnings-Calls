/*
 * Splits the workload into processes instead of threads
 * Uses pipes to transfer data
 * Stock prices will be saved in "prices"
 * "tickers" is this list of strings containing the stock tickers to track
 */

#include "main.h"

extern double *new_prices;
extern double *original_prices;
extern char **tickers;
extern int totalTickers;
extern int *ps_variables;
extern pid_t parent_pid;
int **fd;


/******************************************************************************
 * The child process will switch into this function after grabbing the prices
 * the first go around
 * Continuously waits for a signal and finds/sends prices once triggered
 */
void GetNewPrices(double* prices, int numTickers, int start, int end)
{
   while(1)
   {
     for (int i = 0; i < NUM_PROCESSES; i++)
     {
       while (ps_variables[i] == 0); // Wait for the process to be signaled
       FillArrayWithPrices(prices, tickers, &start, &end);
       SendData(&fd[i][0], numTickers, prices, start);
     }
   }
}

/******************************************************************************
 * Fork to create new processes to distribute the workload
 * These new processes should not exit, but rather call GetNewPrices so the
 * new prices are continuously updated
 */
void SplitIntoProcesses()
{
  int numTickers, start, end, i, x;
//  int status;
  pid_t pid[NUM_PROCESSES];

// Malloc for and open pipes
  fd = (int**)malloc(sizeof(int*) * NUM_PROCESSES);
  for (i = 0; i < NUM_PROCESSES; i++)
  {
    ps_variables[i] = 0; // Will trigger the process to find prices when = 1
    fd[i] = (int*)malloc(sizeof(int) * 2);
    if (pipe(fd[i])==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return;
    }
    printf("ps_var[i] = %d\n", ps_variables[0]);
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
      FillArrayWithPrices(original_prices, tickers, &start, &end);
      SendData(&fd[i][0], numTickers, original_prices, start);
      ps_variables[i] = 1;
      GetNewPrices(new_prices, numTickers, start, end);
    }


  }

   // Parent process
  for (i = 0; i < NUM_PROCESSES; i++)
  {
    while (ps_variables[i] == 0); // wait for the process to finish its job
  }
    // waitpid(pid[i], &status, 0);

  for (i = 0; i < NUM_PROCESSES; i++)
  {
  //  while (ps_variables[i] == 0);
    ReceiveData(&fd[i][0], original_prices);
    printf("ps_vars[i] = %d\n", ps_variables[i]);
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
