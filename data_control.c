/*
 * Created by Heath Gerrald
 */

#include "data_control.h"

int *alreadyOpened;

int main(int argc, char *argv[]){

 // Declare variables
  int totalTickers;
  FILE* fp;
  char** tickers;

  SetupPython();
  CallPythonGrabTickers();

// Grab how many tickers we are going to watch
  fp = fopen("tickers.txt", "r");
  fscanf(fp, "%d", &totalTickers);

 // Create a C object of all the tickers
  tickers = (char**)malloc(sizeof(char*) * totalTickers);
  alreadyOpened = (int*)calloc(totalTickers, sizeof(int));
  GrabTickersFromFile(tickers, fp, totalTickers);

  puts("Grabbing original prices");
  double* originalPrices = (double*)malloc(sizeof(double) * totalTickers);
  SplitInto4(tickers, totalTickers, originalPrices);

// Continue to check from large price changes until user quits
  double* newPrices = (double*)malloc(sizeof(double) * totalTickers);
  while (1){
      puts("Grabbing new prices");
      SplitInto4(tickers, totalTickers, newPrices);
      ComparePrices(originalPrices, newPrices, totalTickers, tickers);
      //sleep(10);
  }


 return 0;
}

/******************************************************************************
 * Sets up compatibility with Python functions
 */
void SetupPython(){

  Py_Initialize();
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
void FillArrayWithPrices(double* tickerArray, int numTickers, char** tickers, int start, int end){

  char* ticker_result = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);

  for (int i = start; i < end; i++)
  {
      FindPrice(tickers[i], ticker_result);

      if (strcmp(ticker_result, "N/A") == 0)
      {
    //    printf("%s - Error\n", tickers[i]);
        tickerArray[i] = -1.00;
      }

      else
      {
         ParseComma(ticker_result);
         sscanf(ticker_result, "%lf", &tickerArray[i]);
    //     printf("%s - %.4lf\n", tickers[i], tickerArray[i]);
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
void ComparePrices(double* original, double* new, int totalTickers, char** tickers){

  double difference = 0.00;
  double percent_change = 0.00;

  for (int i = 0; i < totalTickers; i++)
  {
    if (original[i] != -1.00){
      difference = new[i] - original[i];
      percent_change = new[i] / original[i];

    // Increases or decreases by 4%
      if (percent_change < 0.96 || percent_change > 1.04){
        printf("Difference for %s is %lf\n", tickers[i], difference);
        printf("   Percent change is %.2lf\n", percent_change);
        if (alreadyOpened[i] == 0){
          OpenChart(tickers[i]);
          alreadyOpened[i] = 1;
        }

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
