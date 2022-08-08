//   Производится замена z = y' и уравнение второго порядка сводится к системе:
//      {  y' = z
//      {  z' = -p(x)z - q(x)y + f(x)
#include "odu_26.h"

double z(double x, double y, double z){
    return f(x) - p(x) * z - q(x) * y ;
}