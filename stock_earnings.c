#include <Python.h>
#include <stdio.h>

//gcc $(python-config --includes) stock_earnings.c

int main(){
  PyObject* myModuleString = PyString_FromString((char*)"mytest");
}
