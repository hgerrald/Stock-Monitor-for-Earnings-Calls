/*
 * Splits the workload into 4 processes
 * Forks into 4 processes and uses pipes to transfer data
 * Stock prices will be saved in "data_array"
 * "tickers" is this list of string containing the stock tickers to track
 */

#include "data_control.h"

void SplitInto4(char** tickers, int totalTickers, double* data_array){

  int numTickers, status, start, end;
  int NUM_PROCESSES = 4;
  pid_t pid[NUM_PROCESSES];
  int fd1[2];
  int fd2[2];
  int fd3[2];
  int fd4[2];

// Open all of our pipes
  if (pipe(fd1)==-1)
  {
      fprintf(stderr, "Pipe 1 Failed" );
      return;
  }

  if (pipe(fd2)==-1)
  {
      fprintf(stderr, "Pipe 2 Failed" );
      return;
  }

  if (pipe(fd3)==-1)
  {
      fprintf(stderr, "Pipe 3 Failed" );
      return;
  }

  if (pipe(fd4)==-1)
  {
      fprintf(stderr, "Pipe 4 Failed" );
      return;
  }

  // Create child processes
    if ((pid[0] = fork()) == 0)
    {
    // Child process 1
       pid[0] = getpid();
       numTickers = totalTickers / NUM_PROCESSES;
       end = numTickers;
       FillArrayWithPrices(data_array, numTickers, tickers, 0, end);
       SendData(&fd1[0], numTickers, data_array, 0);
       exit(0);
     } // END CHILD 1

    else if ((pid[1] = fork()) == 0)
    {
     // Child process 2
       pid[1] = getpid();
       numTickers = totalTickers / NUM_PROCESSES;
       if (totalTickers % NUM_PROCESSES == 3) { numTickers++; }
       start = totalTickers / NUM_PROCESSES;
       end = start + numTickers;
       FillArrayWithPrices(data_array, numTickers, tickers, start, end);
       SendData(&fd2[0], numTickers, data_array, start);
       exit(0);
    } // END CHILD 2

    else if ((pid[2] = fork()) == 0)
    {
     // Child process 3
       pid[2] = getpid();
       numTickers = totalTickers / NUM_PROCESSES;

       if (totalTickers % NUM_PROCESSES == 3)
       {
         start = totalTickers / NUM_PROCESSES * 2 + 1;
         numTickers++;
       }

       else if (totalTickers % NUM_PROCESSES == 2)
       {
         start = totalTickers / NUM_PROCESSES * 2;
         numTickers++;
       }
       else
       {
         start = totalTickers / NUM_PROCESSES * 2;
       }

       end = start + numTickers;
       FillArrayWithPrices(data_array, numTickers, tickers, start, end);
       SendData(&fd3[0], numTickers, data_array, start);
       exit(0);
     } // END CHILD 3

     else if ((pid[3] = fork()) == 0)
     {
       // Child process 4
       pid[3] = getpid();
       numTickers = totalTickers / NUM_PROCESSES;
       if (totalTickers % NUM_PROCESSES == 3)
       {
         start = numTickers * 3 + 2;
         numTickers++;
       }

       else if (totalTickers % NUM_PROCESSES == 2)
       {
          start = numTickers * 3 + 1;
          numTickers++;
       }

       else if (totalTickers % NUM_PROCESSES == 1)
       {
          start = numTickers * 3;
          numTickers++;
       }

       else
       {
         start = numTickers * 3;
       }

       end = start + numTickers;
       FillArrayWithPrices(data_array, numTickers, tickers, start, end);
       SendData(&fd4[0], numTickers, data_array, start);
       exit(0);
     } // END CHILD 4

   // Parent process
     waitpid(pid[0], &status, 0);
     waitpid(pid[1], &status, 0);
     waitpid(pid[2], &status, 0);
     waitpid(pid[3], &status, 0);

     ReceiveData(&fd1[0], data_array);
     ReceiveData(&fd2[0], data_array);
     ReceiveData(&fd3[0], data_array);
     ReceiveData(&fd4[0], data_array);


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
