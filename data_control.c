#define TICKER_SIZE_HOLD 15
#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

//gcc $(python-config --includes) stock_earnings.c
//python -c 'from stock_functions import *; print GrabStockPrice("AMZN")'


int main(int argc, char *argv[]){

   int totalTickers;
   FILE* fp = fopen("tickers.txt", "r");
   fscanf(fp, "%d", &totalTickers);

   char* ticker_hold = malloc(sizeof(char) * TICKER_SIZE_HOLD);
   char** tickers = malloc(sizeof(char) * totalTickers);
   for (int i = 0; i < totalTickers; i++)
   {
     tickers[i] = malloc(sizeof(char) * TICKER_SIZE_HOLD);
     memset(ticker_hold, '\0', TICKER_SIZE_HOLD);
     fscanf(fp, "%s", ticker_hold);
     strcpy(tickers[i], ticker_hold);
     printf("%s\n", tickers[i]);
   }


   Py_Initialize();

   PyObject *sysmodule = PyImport_ImportModule("sys");
   PyObject *syspath = PyObject_GetAttrString(sysmodule, "path");
   PyList_Append(syspath, PyString_FromString("."));
   Py_DECREF(syspath);
   Py_DECREF(sysmodule);

   PyObject* myModuleString = PyString_FromString((char*)"stock_functions");
   PyObject* myModule = PyImport_Import(myModuleString);

   PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"GrabStockPrice");
   PyObject* args = PyTuple_Pack(1,PyString_FromString("TURN"));
   PyObject* myResult = PyObject_CallObject(myFunction, args);

   char* result = malloc(15);
   strcpy(result, PyString_AsString(myResult));

   printf("%s", result);



  // Run the python function on each ticker in array

  // Fork and put half of the tickers in each process

 return 0;
}
