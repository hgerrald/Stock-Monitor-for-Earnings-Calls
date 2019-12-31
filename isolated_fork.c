#include "main.h"

double convertToDouble(char* ticker, char* memhold);

void isolated_fork(char** tickers, int totalTickers, double* originalPrices, double*new_prices){

  int status;
  char* memhold;
  pid_t PID[15];
  int i = 0;
  double price_hold;

  int fd0[2];
  int fd1[2];
  int fd2[2];
  int fd3[2];
  int fd4[2];
  int fd5[2];
  int fd6[2];
  int fd7[2];
  int fd8[2];
  int fd9[2];
  int fd10[2];
  int fd11[2];
  int fd12[2];
  int fd13[2];
  int fd14[2];
  // int fd15[2];
  // int fd16[2];
  // int fd17[2];
  // int fd18[2];
  // int fd19[2];
  // int fd20[2];
  // int fd21[2];
  // int fd22[2];
  // int fd23[2];
  // int fd24[2];
  // int fd25[2];
  // int fd26[2];
  // int fd27[2];
  // int fd28[2];
  // int fd29[2];

  pipe(fd0);
  pipe(fd1);
  pipe(fd2);
  pipe(fd3);
  pipe(fd4);
  pipe(fd5);
  pipe(fd6);
  pipe(fd7);
  pipe(fd8);
  pipe(fd9);
  pipe(fd10);
  pipe(fd11);
  pipe(fd12);
  pipe(fd13);
  pipe(fd14);
  // pipe(fd15);
  // pipe(fd16);
  // pipe(fd17);
  // pipe(fd18);
  // pipe(fd19);
  // pipe(fd20);
  // pipe(fd21);
  // pipe(fd22);
  // pipe(fd23);
  // pipe(fd24);
  // pipe(fd25);
  // pipe(fd26);
  // pipe(fd27);
  // pipe(fd28);
  // pipe(fd29);


  if ((PID[0] = fork()) == 0){
    i = 0;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd0[1], &price_hold, sizeof(double));
    close(fd0[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[1] = fork()) == 0){
    i = 1;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd1[1], &price_hold, sizeof(double));
    close(fd1[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[2] = fork()) == 0){
    i = 2;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd2[1], &price_hold, sizeof(double));
    close(fd2[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[3] = fork()) == 0){
    i = 3;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd3[1], &price_hold, sizeof(double));
    close(fd3[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[4] = fork()) == 0){
    i = 4;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd4[1], &price_hold, sizeof(double));
    close(fd4[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[5] = fork()) == 0){
    i = 5;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd5[1], &price_hold, sizeof(double));
    close(fd5[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[6] = fork()) == 0){
    i = 6;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd6[1], &price_hold, sizeof(double));
    close(fd6[1]);
    free(memhold);
    exit(0);
  }


  if ((PID[7] = fork()) == 0){
    i = 7;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd7[1], &price_hold, sizeof(double));
    close(fd7[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[8] = fork()) == 0){
    i = 8;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd8[1], &price_hold, sizeof(double));
    close(fd8[1]);
    free(memhold);
    exit(0);
  }


  if ((PID[9] = fork()) == 0){
    i = 9;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd9[1], &price_hold, sizeof(double));
    close(fd9[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[10] = fork()) == 0){
    i = 10;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd10[1], &price_hold, sizeof(double));
    close(fd10[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[11] = fork()) == 0){
    i = 11;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd11[1], &price_hold, sizeof(double));
    close(fd11[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[12] = fork()) == 0){
    i = 12;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd12[1], &price_hold, sizeof(double));
    close(fd12[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[13] = fork()) == 0){
    i = 13;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd13[1], &price_hold, sizeof(double));
    close(fd13[1]);
    free(memhold);
    exit(0);
  }

  if ((PID[14] = fork()) == 0){
    i = 14;
    memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
    FindPrice(tickers[i], memhold);
    price_hold = convertToDouble(tickers[i], memhold);
    write(fd14[1], &price_hold, sizeof(double));
    close(fd14[1]);
    free(memhold);
    exit(0);
  }

  // if ((PID[15] = fork()) == 0){
  //   i = 15;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd15[1], &price_hold, sizeof(double));
  //   close(fd15[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[16] = fork()) == 0){
  //   i = 16;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd16[1], &price_hold, sizeof(double));
  //   close(fd16[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[17] = fork()) == 0){
  //   i = 17;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd17[1], &price_hold, sizeof(double));
  //   close(fd17[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[18] = fork()) == 0){
  //   i = 18;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd18[1], &price_hold, sizeof(double));
  //   close(fd18[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[19] = fork()) == 0){
  //   i = 19;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd19[1], &price_hold, sizeof(double));
  //   close(fd19[1]);
  //   free(memhold);
  //   exit(0);
  // }

  // if ((PID[20] = fork()) == 0){
  //   i = 20;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd20[1], &price_hold, sizeof(double));
  //   close(fd20[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[21] = fork()) == 0){
  //   i = 21;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd21[1], &price_hold, sizeof(double));
  //   close(fd21[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[22] = fork()) == 0){
  //   i = 22;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd22[1], &price_hold, sizeof(double));
  //   close(fd22[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[23] = fork()) == 0){
  //   i = 23;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd23[1], &price_hold, sizeof(double));
  //   close(fd23[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[24] = fork()) == 0){
  //   i = 24;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd24[1], &price_hold, sizeof(double));
  //   close(fd24[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[25] = fork()) == 0){
  //   i = 25;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd25[1], &price_hold, sizeof(double));
  //   close(fd25[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[26] = fork()) == 0){
  //   i = 26;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd26[1], &price_hold, sizeof(double));
  //   close(fd26[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[27] = fork()) == 0){
  //   i = 27;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd27[1], &price_hold, sizeof(double));
  //   close(fd27[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[28] = fork()) == 0){
  //   i = 28;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd28[1], &price_hold, sizeof(double));
  //   close(fd28[1]);
  //   free(memhold);
  //   exit(0);
  // }
  //
  // if ((PID[29] = fork()) == 0){
  //   i = 29;
  //   memhold = (char*)malloc(sizeof(char) * TICKER_SIZE_HOLD);
  //   FindPrice(tickers[i], memhold);
  //   price_hold = convertToDouble(tickers[i], memhold);
  //   write(fd29[1], &price_hold, sizeof(double));
  //   close(fd29[1]);
  //   free(memhold);
  //   exit(0);
  // }

  else {
      waitpid(PID[0], &status, 0);
      waitpid(PID[1], &status, 0);
      waitpid(PID[2], &status, 0);
      waitpid(PID[3], &status, 0);
      waitpid(PID[4], &status, 0);
      waitpid(PID[5], &status, 0);
      waitpid(PID[6], &status, 0);
      waitpid(PID[7], &status, 0);
      waitpid(PID[8], &status, 0);
      waitpid(PID[9], &status, 0);
      waitpid(PID[10], &status, 0);
      waitpid(PID[11], &status, 0);
      waitpid(PID[12], &status, 0);
      waitpid(PID[13], &status, 0);
      waitpid(PID[14], &status, 0);
      // waitpid(PID[15], &status, 0);
      // waitpid(PID[16], &status, 0);
      // waitpid(PID[17], &status, 0);
      // waitpid(PID[18], &status, 0);
      // waitpid(PID[19], &status, 0);
      // waitpid(PID[20], &status, 0);
      // waitpid(PID[21], &status, 0);
      // waitpid(PID[22], &status, 0);
      // waitpid(PID[23], &status, 0);
      // waitpid(PID[24], &status, 0);
      // waitpid(PID[25], &status, 0);
      // waitpid(PID[26], &status, 0);
      // waitpid(PID[27], &status, 0);
      // waitpid(PID[28], &status, 0);
      // waitpid(PID[29], &status, 0);
      // waitpid(PID[30], &status, 0);
      // waitpid(PID[31], &status, 0);
      // waitpid(PID[32], &status, 0);
      // waitpid(PID[33], &status, 0);
      // waitpid(PID[34], &status, 0);
      // waitpid(PID[35], &status, 0);
      // waitpid(PID[36], &status, 0);
      // waitpid(PID[37], &status, 0);

      read(fd0[0], &new_prices[0], sizeof(double));
      close(fd0[0]);
      read(fd1[0], &new_prices[1], sizeof(double));
      close(fd1[0]);
      read(fd2[0], &new_prices[2], sizeof(double));
      close(fd2[0]);
      read(fd3[0], &new_prices[3], sizeof(double));
      close(fd3[0]);
      read(fd4[0], &new_prices[4], sizeof(double));
      close(fd4[0]);
      read(fd5[0], &new_prices[5], sizeof(double));
      close(fd5[0]);
      read(fd6[0], &new_prices[6], sizeof(double));
      close(fd6[0]);
      read(fd7[0], &new_prices[7], sizeof(double));
      close(fd7[0]);
      read(fd8[0], &new_prices[8], sizeof(double));
      close(fd8[0]);
      read(fd9[0], &new_prices[9], sizeof(double));
      close(fd9[0]);
      read(fd10[0], &new_prices[10], sizeof(double));
      close(fd10[0]);
      read(fd11[0], &new_prices[11], sizeof(double));
      close(fd11[0]);
      read(fd12[0], &new_prices[12], sizeof(double));
      close(fd12[0]);
      read(fd13[0], &new_prices[13], sizeof(double));
      close(fd13[0]);
      read(fd14[0], &new_prices[14], sizeof(double));
      close(fd14[0]);
      // read(fd15[0], &new_prices[15], sizeof(double));
      // close(fd15[0]);
      // read(fd15[0], &new_prices[15], sizeof(double));
      // close(fd15[0]);
      // read(fd16[0], &new_prices[16], sizeof(double));
      // close(fd16[0]);
      // read(fd17[0], &new_prices[17], sizeof(double));
      // close(fd17[0]);
      // read(fd18[0], &new_prices[18], sizeof(double));
      // close(fd18[0]);
      // read(fd19[0], &new_prices[19], sizeof(double));
      // close(fd19[0]);
      // read(fd20[0], &new_prices[20], sizeof(double));
      // close(fd20[0]);
      // read(fd21[0], &new_prices[21], sizeof(double));
      // close(fd21[0]);
      // read(fd22[0], &new_prices[22], sizeof(double));
      // close(fd22[0]);
      // read(fd23[0], &new_prices[23], sizeof(double));
      // close(fd23[0]);
      // read(fd24[0], &new_prices[24], sizeof(double));
      // close(fd24[0]);
      // read(fd25[0], &new_prices[25], sizeof(double));
      // close(fd25[0]);
      // read(fd26[0], &new_prices[26], sizeof(double));
      // close(fd26[0]);
      // read(fd27[0], &new_prices[27], sizeof(double));
      // close(fd27[0]);
      // read(fd28[0], &new_prices[28], sizeof(double));
      // close(fd28[0]);
      // read(fd29[0], &new_prices[29], sizeof(double));
      // close(fd29[0]);
  }


}



/******************************************************************************
 *
 */
double convertToDouble(char* ticker, char* memhold){

  double price;

  if (strcmp(memhold, "N/A") == 0)
  {
  //  printf("%s - Error\n", ticker);
    price = -1.00;
  }

  else
  {
     ParseComma(memhold);
     sscanf(memhold, "%lf", &price);
  //   printf("%s - %.4lf\n", ticker, price);
  }

  return price;

}
