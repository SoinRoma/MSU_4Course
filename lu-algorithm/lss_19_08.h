#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void Lxb(int n, double *L, double *B, double *Y);
void Uxy(int n, double *U, double *Y, double *X);
int determinant(int n, double *Matrix);
void MatrixObr (int n, double *Array, double *E);
int lss_19_08(int n, double *A, double *B, double *X);