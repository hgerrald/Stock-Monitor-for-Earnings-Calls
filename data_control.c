/*
 * Created by Heath Gerrald
 */

#include "data_control.h"

int main(int argc, char *argv[]){

 // Declare variables
  int totalTickers;
  FILE* fp;
  char** tickers;

// Grab how many tickers we are going to watch
  fp = fopen("tickers.txt", "r");
  fscanf(fp, "%d", &totalTickers);

 // Create a C object of all the tickers
  tickers = (char**)malloc(sizeof(char*) * totalTickers);
  setupPython();
  GrabTickers(tickers, fp, totalTickers);

  puts("Grabbing original prices");
  double* originalPrices = (double*)malloc(sizeof(double) * totalTickers);
  splitInto4(tickers, totalTickers, originalPrices);

  for (int i = 0; i < totalTickers; i++)
  {
    printf("%lf\n", originalPrices[i]);
  }

 return 0;
}

/******************************************************************************
 * Sets up compatibility with Python functions
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
 * Calls the python function from stock_functions.py to find the price of a ticker
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
 * Grabs all of the tickers from a text file created by the python function
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
 * Fills the array "tickerArray" with double type values of stock prices for
 * each ticker in "tickers"
 */
void fillArrayWithPrices(double* tickerArray, int numTickers, char** tickers, int start, int end){

  char* ticker_result = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);

  for (int i = start; i < end; i++)
  {
      findPrice(tickers[i], ticker_result);

      if (strcmp(ticker_result, "N/A") == 0)
      {
        printf("%s - Error\n", tickers[i]);
        tickerArray[i] = -1.00;
      }

      else
      {
         parseComma(ticker_result);
         sscanf(ticker_result, "%lf", &tickerArray[i]);
         printf("%s - %.4lf\n", tickers[i], tickerArray[i]);
      }
  } // end for loop
}

/******************************************************************************
 * Used to parse out the comma of stocks >= $1000
 */
void parseComma(char* ticker_result){

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
 * Compares the most recent prices with the original. Looking for a jump of 4%
 * If one is found will send alert and call a python function that opens a web
 * browser to a live chart of that stock
 */
void comparePrices(double** original, double** new){

   double** diff = (double**)malloc(sizeof(double*) * 4);

   for (int i = 0; i < 4; i++)
   {
     size_t elementsInList = sizeof(original[i]) / sizeof(double);
     diff[i] = (double*)malloc(sizeof(double) * elementsInList);
     for (int j = 0; j < elementsInList; j++)
     {
       if (original[i][j] != 0.00)
       {
         diff[i][j] = new[i][j] - original[i][j];
         printf("%lf\n", diff[i][j]);
       }

     }
   }

}
