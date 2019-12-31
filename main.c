/*
 * Created by Heath Gerrald
 */

#include "main.h"

#define NUM_THREADS 8
// NUM_THREADS should be a power of 2: 2,4,8,16,32...

// GLOBAL VARIABLES
char** tickers; // Stores the strings of each ticker
int *alreadyOpened; // Does not open a ticker chart twice
int totalTickers;
pthread_t thread_id[NUM_THREADS];
pthread_t main_thread;
int cond_variables[NUM_THREADS];
double *original_prices;
double* new_prices;


int main(int argc, char *argv[])
{
  FILE* fp;
  time_t start_time, end_time;


  SetupPython();
// Grab tickers with earnings calls today and store in "tickers.txt"
  //CallPythonGrabTickers();

// Grab how many tickers we are watching from the file
  fp = fopen("tickers.txt", "r");
  fscanf(fp, "%d", &totalTickers);

 // Create a C object to store the tickers
  tickers = (char**)malloc(sizeof(char*) * totalTickers);
  alreadyOpened = (int*)calloc(totalTickers, sizeof(int));
  GrabTickersFromFile(tickers, fp, totalTickers);

 // Grab the original prices, create threads so distribute work load
  puts("GRABBING ORIGINAL PRICES");
  original_prices = (double*)malloc(sizeof(double) * totalTickers);
  main_thread = pthread_self();
  start_time = time(NULL);

  for (int i = 0; i < NUM_THREADS; i++)
  {
    cond_variables[i] = 1;
    pthread_create(&thread_id[i], NULL, start_scan, original_prices);
  }

// wait for all threads to complete
  for (int i = 0; i < NUM_THREADS; i++)
      while (cond_variables[i] == 1);

// print the time it took to complete
  end_time = time(NULL);
  printf("Time elapsed = %ld seconds\n", end_time - start_time);


// Continue to check for large price changes until user quits
  new_prices = (double*)malloc(sizeof(double) * totalTickers);
  while (1)
  {
    puts("GRABBING NEW PRICES");
    start_time = time(NULL);
    for (int i = 0; i < NUM_THREADS; i++)
      cond_variables[i] = 1;

    for (int i = 0; i < NUM_THREADS; i++) // wait for threads to complete
        while (cond_variables[i] == 1);

    ComparePrices(original_prices, new_prices);
    end_time = time(NULL);
    printf("Time elapsed = %ld seconds\n", end_time - start_time);
  }




 return 0;
}


/******************************************************************************
 * Distribute the workload between NUM_THREADS threads
 */
void *start_scan(void *vargp)
{
   PyGILState_STATE gstate;
   gstate = PyGILState_Ensure();


   int x, start, end, numTickers;
   double* prices = vargp;

  // Find which thread this is (the value of x)
   x = 0;
   while (pthread_self() != thread_id[x] && x < NUM_THREADS)
      x++;

   if (x == 0) start = 0;
   else
   {
     start = x * totalTickers / NUM_THREADS;
     if (x < totalTickers / NUM_THREADS)
       start++;
   }


   end = (x+1) * totalTickers / NUM_THREADS - 1;
   if (x+1 != NUM_THREADS && totalTickers % NUM_THREADS != 0)
     end++;

   numTickers = end - start + 1;

  // printf("x = %d, start = %d, end = %d\n", x, start, end);

   FillArrayWithPrices(prices, numTickers, tickers, start, end);

   cond_variables[x] = 0;

   PyGILState_Release(gstate);

   while (cond_variables[x] == 0);
   start_scan(new_prices);

   return NULL;
}

/******************************************************************************
 * Sets up compatibility with Python functions
 */
void SetupPython(){

  Py_Initialize();
  PyEval_InitThreads();
  PyEval_ReleaseLock();
  PyObject *sysmodule = PyImport_ImportModule("sys");
  PyObject *syspath = PyObject_GetAttrString(sysmodule, "path");
  PyList_Append(syspath, PyString_FromString("."));
  Py_DECREF(syspath);
  Py_DECREF(sysmodule);

}

/******************************************************************************
 * Calls the python function from stock_functions.py to find the price of a ticker
 */
void FindPrice(char* symbol, char* result){

  PyObject* myModuleString = PyString_FromString((char*)"stock_functions");
  PyObject* myModule = PyImport_Import(myModuleString);

  PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"GrabStockPrice");
  PyObject* args = PyTuple_Pack(1,PyString_FromString(symbol));
  PyObject* myResult = PyObject_CallObject(myFunction, args);

  strcpy(result, PyString_AsString(myResult));
}

/******************************************************************************
 * Grabs all of the tickers from a text file created by the python function
 */
void GrabTickersFromFile(char** ticker_array, FILE* fp, int totalTickers){

  for (int i = 0; i < totalTickers; i++)
  {
    ticker_array[i] = malloc(sizeof(char) * TICKER_SIZE_HOLD);
    memset(ticker_array[i], '\0', TICKER_SIZE_HOLD);
    fscanf(fp, "%s", ticker_array[i]);
  }
  printf("Successfully transfered %d tickers into C\n", totalTickers);
}

/******************************************************************************
 * Fills the array "tickerArray" with double type values of stock prices for
 * each ticker in "tickers"
 */
void FillArrayWithPrices(double* tickerArray, int numTickers, char** tickers, int start, int end)
{
  char* ticker_result = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);

  for (int i = start; i <= end; i++)
  {
      FindPrice(tickers[i], ticker_result);

      if (strcmp(ticker_result, "N/A") == 0)
      {
      //  printf("%s - Error\n", tickers[i]);
        tickerArray[i] = -1.00;
      }

      else
      {
         ParseComma(ticker_result);
         sscanf(ticker_result, "%lf", &tickerArray[i]);
      //   printf("%s - %.4lf\n", tickers[i], tickerArray[i]);
    }

  } // end for loop

  free(ticker_result);

}

/******************************************************************************
 * Used to parse out the comma of stocks >= $1000
 */
void ParseComma(char* ticker_result){

   char* comma_string = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
   char* return_string = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
   comma_string = strchr(ticker_result, ',');

   if (!comma_string) return;

   else
   {
     memcpy(return_string, &ticker_result[0], 1);
     memcpy(return_string+1, &comma_string[1], strlen(comma_string) - 1);
     strcpy(ticker_result, return_string);
   }
}


/******************************************************************************
 * Compares the most recent prices with the original. Looking for a jump of 3.5%
 * If one is found will send alert and call a python function that opens a web
 * browser to a live chart of that stock
 */
void ComparePrices(double* original, double* new){

  double difference = 0.00;
  double percent_change = 0.00;

  for (int i = 0; i < totalTickers; i++)
    if (original[i] != -1.00){
      difference = new[i] - original[i];
      percent_change = new[i] / original[i];

  //    printf("Difference for %s is %lf\n", tickers[i], difference);

    // Increases by 4%
      if (percent_change >= 1.04){
        printf("Difference for %s is %lf\n", tickers[i], difference);
        printf("   Percent change is %.2lf\n", percent_change);
        if (alreadyOpened[i] == 0){
          OpenChart(tickers[i]);
          alreadyOpened[i] = 1;
        }

      }

    }



}

/******************************************************************************
 * Calls the python function to open a live chart of a specific ticker
 */
void OpenChart(char* symbol){

  PyObject* myModuleString = PyString_FromString((char*)"stock_functions");
  PyObject* myModule = PyImport_Import(myModuleString);

  PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"OpenTicker");
  PyObject* args = PyTuple_Pack(1,PyString_FromString(symbol));
  PyObject_CallObject(myFunction, args);

}


/******************************************************************************
 * Calls the python function to grab all the stock tickers from TradingView
 */
void CallPythonGrabTickers(){

  PyObject* myModuleString = PyString_FromString((char*)"stock_functions");
  PyObject* myModule = PyImport_Import(myModuleString);

  PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"FindStocksWithEarningsCalls");
//  PyObject* args = PyTuple_Pack(1,PyString_FromString(symbol));
  PyObject_CallObject(myFunction, NULL);
}
