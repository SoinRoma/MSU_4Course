// Модуль вычисления функции y(a),y(b)
#include "odu_26.h"

double y(double x) {
/*1*/      //return 1 - x * x + 2 * x;
/*2*/      //return   3 * x + exp(-2 * x);
/*3*/      return  1 - x * x + 2 * x - 11 * x + (x * x * x * x) / 6.0 + (x * x) / 2.0;
/*4*/      //return   x + 2 * (log(x) + 1) + 0.5 * x * x * (1 - log(x));
/*5*/      //return   x * (1 + sin(x)) + 0.5 * x * exp(x) * (cos(x) + sin(x));
/*6*/      //return  exp(x) * sin(x);
/*7*/      //return  x * (1 + exp(0.5 * x * x)) + x * (cos(x * x) - 2 * sin(x * x));
}