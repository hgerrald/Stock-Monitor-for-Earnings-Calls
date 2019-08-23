#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define TICKER_SIZE_HOLD 15

// Function Prototypes
void SetupPython();
void FindPrice();
void CallPythonGrabTickers();
void GrabTickersFromFile(char**, FILE*, int);
void FillArrayWithPrices(double*, int, char**, int, int);
void SplitInto4(char**, int, double*);
void ParseComma(char*);
void ComparePrices(double*, double*, int, char**);
void SendData(int*, int, double*, int);
void ReceiveData(int*, double*);
void OpenChart(char*);
void SplitInto2(char**, int, double*, int, int, int);
void FillArrayWithPrices_P2(double*, char**, int, int);
void SendDataP2(int*, double*, int, int);
void ReceiveDataP2(int*, double*);
