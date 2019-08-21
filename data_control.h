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
void setupPython();
void findPrice();
void GrabTickers(char**, FILE*, int);
void fillArrayWithPrices(double*, int, char**, int, int);
void splitInto4(char**, int, double*);
void parseComma(char*);
void comparePrices(double**, double**);
void sendData(int*, int, double*, int);
void receiveData(int*, double*);
