/*
 * Splits the work load into 2 processes
 */

#include "data_control.h"

void SplitInto2(char** tickers, int totalTickers, double* data_array, int count, int start, int end){

  pid_t pid;
  int status;
  int fd1[2];

  if (count != 0)
  {
     int totalTickers_A, totalTickers_B;
     totalTickers_A = totalTickers / 2;
     totalTickers_B = totalTickers / 2;
     if (totalTickers % 2 == 1) totalTickers_B++;
     SplitInto2(tickers, totalTickers_A, data_array, count-1, start, totalTickers_A);
     SplitInto2(tickers, totalTickers_B, data_array, count-1, totalTickers_A+1, totalTickers);
  }

// Open the pipe
  if (pipe(fd1)==-1)
  {
      fprintf(stderr, "Pipe 1 Failed" );
      return;
  }

 // Create child processes
   if ((pid = fork()) == 0)
   {
   // Child process 1
      pid = getpid();
      end = totalTickers / 2;
      FillArrayWithPrices_P2(data_array, tickers, start, end);
      SendDataP2(&fd1[0], data_array, start, end);
      exit(0);
  }

  else
  {
    // Parent process
      start = totalTickers / 2;
      FillArrayWithPrices_P2(data_array, tickers, start, end);
      waitpid(pid, &status, 0);
      ReceiveDataP2(&fd1[0], data_array);
  }



}


void FillArrayWithPrices_P2(double* data_array, char** tickers, int start, int end){


  char* ticker_result = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);

  for (int i = start; i < end; i++)
  {
      FindPrice(tickers[i], ticker_result);

      if (strcmp(ticker_result, "N/A") == 0)
      {
      //  printf("%s - Error\n", tickers[i]);
        data_array[i] = -1.00;
      }

      else
      {
         ParseComma(ticker_result);
         sscanf(ticker_result, "%lf", &data_array[i]);
      //   printf("%s - %.4lf\n", tickers[i], data_array[i]);
      }
  } // end for loop

}

/******************************************************************************
 * Sends data through a pipe
 */
void SendDataP2(int *fd, double* data, int start, int end){

  int numTickers = end - start;

  // Send the number of tickers and the index in this process
   write(fd[1], &numTickers, sizeof(int));
   write(fd[1], &start, sizeof(int));

   for (int i = start; i < end + 1; i++)
     write(fd[1], &data[i], sizeof(double));

   close(fd[1]);
}


/******************************************************************************
 * Receives data through a pipe
 */
void ReceiveDataP2(int *fd, double* data_array){

  int numTickers, start;

  read(fd[0], &numTickers, sizeof(int));
  read(fd[0], &start, sizeof(int));

  for (int i = start; i < numTickers + start; i++){
      read(fd[0], &data_array[i], sizeof(double));
  }


  close(fd[0]);
}
