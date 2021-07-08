#include "lss_19_08.h"

extern int act_e, act_d;
#define addr(i, j) ((i)*n+(j))

// Матричное умножение обратной матрицы L с B = Y
void Lxb(int n, double *L, double *B, double *Y) {
    double sum = 0;
    int k = 0, p = 0;

    for (int j = 0; j < n * n; j++) {
        sum = sum + L[j] * B[k];
        k++;
        if (k == n) {
            Y[p] = sum;
            k = 0;
            sum = 0;
            p++;
            if (p == n) {
                break;
            }
        }
    }
}

// Матричное умножение обратной матрицы U с Y = X
void Uxy(int n, double *U, double *Y, double *X) {
    double sum = 0;
    int k = 0, p = 0;

    for (int j = 0; j < n * n; j++) {
        sum = sum + U[j] * Y[k];
        k++;
        if (k == n) {
            X[p] = sum;
            k = 0;
            sum = 0;
            p++;
            if (p == n) {
                break;
            }
        }
    }
}

// Нахождения определителя матрицы

int determinant(int n, double *Matrix) {

    int koef = -1, step = 1, value = 1;
    double d = 1, tmp, tmp3;

    if (n == 2) {
        d = Matrix[0] * Matrix[3] - Matrix[1] * Matrix[2];
        //printf("%lf\n ",d);
    }
    else {
        //Сначала надо проверить все ли элементы 0 под нужным нам числом
        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) { //берем элемент и элемент под ним
                if (Matrix[n * i + i] == 0) {

                    //проверяем выбранный элемент на 0, если он ноль, то надо найти строку под ним с не 0 элементом и поменять местами
                    for (int p = j; p < n; p++) {   //выбор элементов под ним
                        if ((Matrix[n * i + i] == 0) && (Matrix[p * n + i] != 0)) {   //поиск не нулевого элемента

                            for (int k = 0; k < n; k++) {  //цикл смены строки
                                tmp3 = Matrix[n * i + k];
                                Matrix[n * i + k] = Matrix[n * p + k];
                                Matrix[n * p + k] = tmp3;
                            }
                            step++;  //запоминаем шаг, для смены знака матрицы
                        }
                        else { //если же все элементы под ним 0, то матрица имеет определитель 0
                            d = 0;
                            value = 0;
                        }
                    }
                }
                if(d ==1){
                    tmp = -Matrix[j * n + i] / Matrix[n * i + i];

                    for (int k = 0; k < n; k++) {
                        Matrix[j * n + k] += Matrix[i * n + k] * tmp;
                    }
                }
            }

        }

        //Если есть не 0 то нужно поменять строки местами
        //Если поменять местами две строки (столбца) матрицы, то определитель матрицы поменяет знак.
        //Потом разделить элементы и отнять от всей строки
        printf("%lf\n ",d);
        if (d != 0) {
            for (int k = 0; k < step; k++) {
                koef = koef * (-1);
            }
            d = koef * d;
            for (int i = 0; i < n; i++) {
                d *= Matrix[i * n + i];
                //printf("%lf\n ",d);
            }

        }
    }


    if(d==0) {
        value = 0;
    }
   //printf("%lf ",d);
   //printf("%d ",value);
    return value;
}


// Нахождение обратной матрицы
void MatrixObr(int n, double *Array, double *E) {

    double tmp;
    // Инициализация единичной матрицы
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            E[addr(i, j)] = (i == j ? 1 : 0);
        }
    }
    for (int i = 0; i < n; i++) {
        // Нормировка строки (первый элемент станет = 1)
        tmp = Array[addr(i, i)];
        for (int j = n - 1; j >= 0; j--) {
            E[addr(i, j)] /= tmp;
            Array[addr(i, j)] /= tmp;
        }
        // Исключаем i-ый элемент с каждой строки кроме i-ой
        for (int j = 0; j < n; j++) {
            if (j != i) {
                tmp = Array[addr(j, i)];
                for (int k = n - 1; k >= 0; k--) {
                    E[addr(j, k)] -= E[addr(i, k)] * tmp;
                    Array[addr(j, k)] -= Array[addr(i, k)] * tmp;
                }
            }
        }
    }
}

int lss_19_08(int n, double *A, double *B, double *X) {
    double *L = NULL;     // Одномерный массив для Матрицы L
    double *U = NULL;      // Одномерный массив для атрицы U
    double *Y = NULL;      // Одномерный массив для атрицы Y
    double *EforL = NULL;        // Одномерный массив для Единичной матрицы для обратной
    double *EforU = NULL;        // Одномерный массив для Единичной матрицы для обратной

    double sum = 0;               // Значение определителя матрицы
    int value = 0;
    int v, w, x, z,det1;                    // индексы для построения

    if ((EforL = (double *) malloc((n * n + n) * sizeof(double))) == NULL) { //Выделение памяти для E
        if (act_d) printf("Couldn't allocate memory for EforL \n");
    }

    if ((EforU = (double *) malloc((n * n + n) * sizeof(double))) == NULL) { //Выделение памяти для E
        if (act_d) printf("Couldn't allocate memory for EforU \n");
    }

    if ((L = (double *) malloc((n * n) * sizeof(double))) == NULL) { //Выделение памяти для L
        if (act_d) printf("Couldn't allocate memory for L \n");
    }

    if ((U = (double *) malloc((n * n) * sizeof(double))) == NULL) { //Выделение памяти для U
        if (act_d) printf("Couldn't allocate memory for B \n");
    }


    if ((Y = (double *) malloc(n * sizeof(double))) == NULL) { //Выделение памяти для Y
        if (act_d) printf("Couldn't allocate memory for Y \n");
    }

    for (int i = 0; i < n * n; i++) { // обнуляем матрицы L и U
        L[i] = 0;
        U[i] = 0;
    }
    for (int i = 0; i < n; i++) { // обнуляем матрицу Y
        Y[i] = 0;
    }

    if (act_d) printf("Build L and U...\n");
    // Строим матрицы L и U
    for (int i = 0; i < n * n; i++) { // Первый столбец для L равен первому столбцу A
        L[i] = A[i];
        i = i + n - 1;
    }

    for (int i = 0; i < n; i++) {// первая строка для U
        U[i] = A[i] / L[0];
        U[0] = 1;
    }

    for (int i = 0; i < n * n; i = i + n + 1) { // Диагональ для U = 1
        U[i] = 1;
    }

    for (int i = n + 1; i < n * n; i = i + n + 1) {
        v = i;
        while (v < n * n) {
            sum = 0;
            for (int p = 1; p <= i % n; p++) {
                w = v - p;//vlevo
                x = i - n * p; //vverh
                sum = sum + L[w] * U[x];
            }
            L[v] = A[v] - sum;
            v = v + n;
        }

        z = i + 1; //vpravo
        while (z % n != 0) {
            sum = 0;
            for (int p = 1; p <= i % n; p++) {
                w = i - p;//vlevo po L
                x = z - n * p; //vverh
                sum = sum + L[w] * U[x];
            }
            U[z] = (A[z] - sum) / L[i];
            z = z + 1;
        }
    }


    // Вызываем нахождение обратной матрицы для L и U
    // Сначала проверяем определитель A
    if (act_d) printf("Checking if matrix is virojdena...\n");
    det1 = determinant(n, A);
    //printf("%d ",det1);
    if (det1 == 0) {
        if (act_e) printf("Error! Matrix is not virojdena!\n");
        value = -1;
    } else {
        if (act_d) printf("Matrix is not virojdena. Build revers L and U\n");
        // Делаем матрицы L и U обратными
        MatrixObr(n, L, EforL);
        MatrixObr(n, U, EforU);
        if (act_d) printf("Multiply matrix's\n");
        // Вызываем матричное умножение
        Lxb(n, EforL, B, Y);
        Uxy(n, EforU, Y, X);
        if (act_d) printf("Linear system solved successfully!\n");
    }
    return value;
}

