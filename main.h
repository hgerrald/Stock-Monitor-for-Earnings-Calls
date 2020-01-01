#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define TICKER_SIZE_HOLD 15
#define NUM_THREADS 8
#define NUM_PROCESSES 16
// NUM_THREADS/PROCESSES should be a power of 2: 2, 4, 8, 16, 32...

// Function Prototypes
void SetupPython();
void FindPrice();
void CallPythonGrabTickers();
void GrabTickersFromFile(FILE*);
void FillArrayWithPrices(double*, int*, int*);
void SplitIntoProcesses();
void ParseComma(char*);
void ComparePrices(double*, double*);
//void SendData(int*, int, double*, int);
//void ReceiveData(int*, double*);
void OpenChart(char*);
void *StartPriceScan();
void GrabNewPricesThreads();
void GetNewPricesProcesses(int*, int*, int);
