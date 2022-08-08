#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

double p(double x);
double q(double x);
double f(double x);
double y(double x);
double z(double x, double y, double z);

void runge(double a, double c, int n, double h, double *y1, double *y2, double val);
double richardon(double h, double *tempRunge);
double pristrelka(double a, double c, double d, int n, double h, double *answer, double *tempMemory, double val);
int bisection(double a, double c, double d, int n, double h, double e, double *answer, double *tempMemory);
int odu_26(double a, double b, double c, double d, double e, int n, double *answer , double *tempMemory);