#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define TICKER_SIZE_HOLD 15

// Function Prototypes
void SetupPython();
void FindPrice();
void CallPythonGrabTickers();
void GrabTickersFromFile(char**, FILE*, int);
void FillArrayWithPrices(double*, int, char**, int, int);
void SplitInto4(char**, int, double*);
void ParseComma(char*);
void ComparePrices(double*, double*);
void SendData(int*, int, double*, int);
void ReceiveData(int*, double*);
void OpenChart(char*);
void *start_scan(void *);

// void SplitInHalf(char**, int, double*, int, int, int);
// void FillArrayWithPrices_P2(double*, char**, int, int);
// void SendDataP2(int*, double*, int, int);
// void ReceiveDataP2(int*, double*);

void isolated_fork(char** tickers, int totalTickers, double* originalPrices, double*);
