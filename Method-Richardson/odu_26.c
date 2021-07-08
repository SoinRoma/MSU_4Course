#include "odu_26.h"
// Рунге-Кутты 4-го порядка
void runge(double a, double c, int n, double h, double *y1, double *y2, double val) {

    double k1, k2, k3, k4, x, l1, l2, l3, l4;
    y1[0] = c;
    y2[0] = val;

    for (int i = 0; i < n; i++){
        x = a + h * i;

        l1 = h * y2[i];
        k1 = h * z(x, y1[i], y2[i]);

        l2 = h * (y2[i] + k1 / 2.0);
        k2 = h * z(x + h / 2.0, y1[i] + l1 / 2.0, y2[i] + k1 / 2.0);

        l3 = h * (y2[i] + k2 / 2.0);
        k3 = h * z(x + h / 2.0, y1[i] + l2 / 2.0, y2[i] + (k2 / 2.0));

        l4 = h * (y2[i] + k3);
        k4 = h * z(x + h, y1[i] + l3, y2[i] + k3);

        y1[i+1] = y1[i]+ (l1 + 2 * (l2 + l3) + l4) / 6.0;
        y2[i+1] = y2[i]+(k1 + 2*(k2 + k3)+k4)/6.0;
    }
}

// Апрокмимация Ричардсона
double richardon(double h, double *tempRunge){

    double *tempMassiv = (double*)malloc(9000), A[10], X[3], mul = 0;
    int n = 3;
    // Строим треугольную матрицу A
    A[0] = 1;
    A[1] = h;
    A[2] = h * h;
    A[3] = 1;
    A[4] = 0.5*h;
    A[5] = 0.5*h * 0.5*h;
    A[6] = 1;
    A[7] = 0.25*h;
    A[8] = 0.25*h * 0.25*h;
    //Строим матрицы
    for ( int i = 0; i < n; i++){
        for ( int j = 0; j < n; j++) {
            if (j == 0) { //строим матрицу L
                tempMassiv[i * n] = A[i * n];
            }
            else {
                if (j > 0 && j <= i) {
                    mul = 0;
                    for ( int k = 0; k <= (j - 1); k++) {
                        mul = mul + tempMassiv[i * n + k] * tempMassiv[n * n + k * n + j];
                    }
                    tempMassiv[i * n + j] = A[i * n + j] - mul;
                    if (tempMassiv[n * n + i * n + j] == -0) {
                        tempMassiv[n * n + i * n + j] = 0;
                    }
                }
                else {
                    tempMassiv[i * n + j] = 0;
                }
            }
            if (i == 0) { //строим матрицу U
                if (tempMassiv[0] <= 0.000001 && tempMassiv[0] >= -0.000001) { // если значения слишком малы
                    return -1;
                }
                tempMassiv[n * n + j] = A[j] / tempMassiv[0];
            }
            else {
                if (j >= i && i > 0) {
                    if (i == j) {
                        if (tempMassiv[i * n + j] <= 0.000001 && tempMassiv[i * n + j] >= -0.000001) { // если значения слишком малы
                            return -1;
                        }
                        tempMassiv[n * n + i * n + j] = 1;
                    }
                    else {
                        mul = 0;
                        for ( int k = 0; k <= (i - 1); k++)
                            mul += tempMassiv[i * n + k] * tempMassiv[n * n + k * n + j];

                        if (tempMassiv[i * n + i] <= 0.000001 && tempMassiv[i * n + i] >= -0.000001) {
                            return -1;
                        }
                        tempMassiv[n * n + i * n + j] = (A[i * n + j] - mul) * (1 / tempMassiv[i * n + i]);
                        if (tempMassiv[n * n + i * n + j] == -0) {
                            tempMassiv[n * n + i * n + j] = 0;
                        }
                    }
                }
                else {
                    tempMassiv[n * n + i * n + j] = 0;
                }
            }
        }
    }
    //L*y = b
    for ( int i = 0; i < n; i++){
        mul = 0;
        for ( int j = 0; j < i; j++){
            mul = mul + tempMassiv[i * n + j] * tempMassiv[2 * n * n + j];
        }
        tempMassiv[2 * n * n + i] = (tempRunge[i] - mul) / tempMassiv[i * n + i];
    }
    //U*x = y
    for ( int i = n - 1; i >= 0; i--){
        mul = 0;
        for ( int j = n - 1; j > i; j--){
            mul += tempMassiv[n * n + i * n + j] * X[j];
        }
        X[i] = (tempMassiv[2 * n * n + i] - mul) / tempMassiv[n * n + i * n + i];
    }
    return X[0];
}

double pristrelka(double a, double c, double d, int n, double h, double *answer, double *tempMemory, double val) {

    double  *tempMassiv = (double*)malloc(9000);
    double *t1= (double*)malloc(9000), *t2 = (double*)malloc(9000), *t3 = (double*)malloc(9000);
    double  tempRunge[3];
    int k = 0;
    // Решаем задачу с 3 разными шагами.
    runge(a, c, n, h, tempMassiv, tempMemory, val); //n, h
    for ( int i = 0; i <= n; i++){
        t1[i] = tempMassiv[i];
    }
    runge(a, c, 2 * n, 0.5 * h, tempMassiv, tempMemory, val);//2*n, 0.5*h
    for (int i = 0; i <= 2*n; i++) {
        if (i % 2 == 0){
            t2[k] = tempMassiv[i];
            k++;
        }
    }
    k = 0;
    runge(a, c, 4 * n, 0.25 * h, tempMassiv, tempMemory, val);//4*n, 0.25*h
    for (int i = 0; i <= 4*n; i++){
        if (i % 4 == 0) {
            t3[k] = tempMassiv[i];
            k++;
        }
    }
    // вызываем метод ричардсона
    for (int i = 0; i < n+1; i++){
        tempRunge[0] = t1[i];
        tempRunge[1] = t2[i];
        tempRunge[2] = t3[i];
        answer[i] = richardon(h, tempRunge);
    }
    return answer[n] - d;
}

// Реализация метода пристрелки для нахождения значения параметра val
// в массиве answer находится приближение искомой функции с оптимальным параметром.
int bisection(double a, double c, double d, int n, double h, double e, double *answer, double *tempMemory){

    double left = 0, right = 0, middle = 0, f1 = 0, f2 = 0, f3 = 0, step = 1;
    left = -10;
    right = 10;

    for(;;) { // ищем интервал
        f1 = pristrelka(a, c, d, n, h, answer, tempMemory, left);
        f2 = pristrelka(a, c, d, n, h, answer, tempMemory, right);
        if (f1 * f2 > 0) {
            break;
        }

        // деление пополам
        middle = (left + right) / 2;
        f3 = pristrelka(a, c, d, n, h, answer, tempMemory, middle);
        if (f3 * f1 > 0) {
            left = middle;
        }
        if (f3 * f2 > 0) {
            right = middle;
        }
        if ((fabs(f1) < e) || (fabs(f2) < e)) {
            break;
        }
    }
}

//  Решатель
int odu_26(double a, double b, double c, double d, double e, int n, double *answer, double *tempMemory ) {

    double *test_1 = (double*)(size_t)(double*)malloc(9000 * sizeof(double));;
    double *test_2 = (double*)(size_t)(double*)malloc(9000 * sizeof(double));
    double h1 = (b - a) / (n), h2 = (b - a) / (2 * n);
    int  index1 = 0, index2 = 0, n2 = 2 * n;

    bisection(a, c, d, n, h1, e, answer, tempMemory);
    for (int i = 0; i <= n; i++){
        test_1[index1] = answer[i];
        index1 = index1 + 1;
    }
    bisection(a, c, d, n2, h2, e, answer, tempMemory);
    for (int i = 0; i <= n2; i++) {
        if (i%(n2/n) == 0) {
            test_2[index2] = answer[i];
            index2 = index2 + 1;
        }
    }
    printf("Сходится! \n");
    for (int i = 0; i <= n; i++) {
        answer[i] = test_1[i];
    }
    return n;
}