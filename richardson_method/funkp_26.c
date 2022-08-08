// y''= p*y' + q*y = f
// Модуль вычисления функции p(x)
#include "odu_26.h"

double p(double x) {
/*1*/      //return -(2 * x) / (x * x + 1);
/*2*/      //return   4 * x / (2 * x + 1);
/*3*/      return  -2 * x / (x * x + 1);
/*4*/      //return  -1 / (x * log(x));
/*5*/      //return  (x * tan(x) - 2) / x;
/*6*/      //return  -1;
/*7*/      //return   -(x * x + 3) / x;
}