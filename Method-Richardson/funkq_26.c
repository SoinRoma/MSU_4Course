// y''= p*y' + q*y = f
// Модуль вычисления функции q(x)
#include "odu_26.h"

double q(double x) {
/*1*/      //return 2 / (x * x + 1);
/*2*/      //return   -4 / (2 * x + 1);
/*3*/      return  2 / (x * x + 1);
/*4*/      //return  1 / (x * x * log(x));
/*5*/      //return  (2 - x * tan(x)) / (x * x);
/*6*/      //return  10000;
/*7*/      //return   (x * x + 3) / (x * x);
}