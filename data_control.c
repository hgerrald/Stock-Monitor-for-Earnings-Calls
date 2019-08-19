/*
 * Created by Heath Gerrald
 */

#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define TICKER_SIZE_HOLD 15
#define NUM_PROCESSES 4

// Function Prototypes
void setupPython();
void findPrice();
void GrabTickers(char**, FILE*, int);
void fillArrayWithPrices(double*, int, char**, int, int);


int main(int argc, char *argv[]){

 // Declare variables
  int totalTickers, numTickers, status, start, end;
  FILE* fp;
  char** tickers;
  pid_t pid[NUM_PROCESSES];

// Grab how many tickers we are going to watch
  fp = fopen("tickers.txt", "r");
  fscanf(fp, "%d", &totalTickers);

 // Create a C object of all the tickers
  tickers = (char**)malloc(sizeof(char*) * totalTickers);
  GrabTickers(tickers, fp, totalTickers);
  setupPython();

 // Create child processes
   if ((pid[0] = fork()) == 0)
   {
   // Child process 1
      pid[0] = getpid();
      numTickers = totalTickers / NUM_PROCESSES;
      double tickerArray_A[numTickers];
      end = numTickers;
      fillArrayWithPrices(tickerArray_A, numTickers, tickers, 0, end);
      exit(0);
    } // END CHILD 1

    else if ((pid[1] = fork()) == 0)
    {
    // Child process 2
      pid[1] = getpid();
      numTickers = totalTickers / NUM_PROCESSES;
      if (totalTickers % NUM_PROCESSES == 3) { numTickers++; }
      double tickerArray_B[numTickers];
      start = totalTickers / NUM_PROCESSES;
      end = start + numTickers;
      fillArrayWithPrices(tickerArray_B, numTickers, tickers, start, end);
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
      double tickerArray_C[numTickers];
      fillArrayWithPrices(tickerArray_C, numTickers, tickers, start, end);
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
      double tickerArray_D[numTickers];
      fillArrayWithPrices(tickerArray_D, numTickers, tickers, start, end);
      exit(0);
    } // END CHILD 4

    // Parent process
      waitpid(pid[0], &status, 0);
      waitpid(pid[1], &status, 0);
      waitpid(pid[2], &status, 0);
      waitpid(pid[3], &status, 0);


 return 0;
}

/******************************************************************************
 *
 */
void setupPython(){

  Py_Initialize();
  PyObject *sysmodule = PyImport_ImportModule("sys");
  PyObject *syspath = PyObject_GetAttrString(sysmodule, "path");
  PyList_Append(syspath, PyString_FromString("."));
  Py_DECREF(syspath);
  Py_DECREF(sysmodule);

}

/******************************************************************************
 *
 */
void findPrice(char* symbol, char* result){

  PyObject* myModuleString = PyString_FromString((char*)"stock_functions");
  PyObject* myModule = PyImport_Import(myModuleString);

  PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"GrabStockPrice");
  PyObject* args = PyTuple_Pack(1,PyString_FromString(symbol));
  PyObject* myResult = PyObject_CallObject(myFunction, args);

  strcpy(result, PyString_AsString(myResult));
}

/******************************************************************************
 *
 */
void GrabTickers(char** ticker_array, FILE* fp, int totalTickers){

  for (int i = 0; i < totalTickers; i++)
  {
    ticker_array[i] = malloc(sizeof(char) * TICKER_SIZE_HOLD);
    memset(ticker_array[i], '\0', TICKER_SIZE_HOLD);
    fscanf(fp, "%s", ticker_array[i]);
  }
  printf("Successfully grabbed %d tickers\n", totalTickers);
}

/******************************************************************************
 *
 */
void fillArrayWithPrices(double* tickerArray, int numTickers, char** tickers, int start, int end){

  char* ticker_result = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  for (int i = start; i < end; i++)
  {
      findPrice(tickers[i], ticker_result);
      if (strcmp(ticker_result, "N/A") == 0)
         printf("%s - Error\n", tickers[i]);
      else
      {
         sscanf(ticker_result, "%lf", &tickerArray[i]);
         printf("%s - %.4lf\n", tickers[i], tickerArray[i]);
      }
  } // end for loop
}
