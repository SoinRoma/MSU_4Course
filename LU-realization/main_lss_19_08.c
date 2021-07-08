#include "lss_19_08.h"

int act_d = 0, act_e = 0, act_p = 0, act_t = 0, act_h = 0, act_q = 0;

int _strlen(char const *input) {
    int length = 0;
    while (input[length] != '\0')
        length++;
    return length;
}


void writeOutputData(char *outputFile, int n, const double *X, int result)
{
    int i;
    FILE *out = fopen(outputFile, "w");
    if (result)
        fprintf(out, "%d\n", 0);
    else
    {
        fprintf(out, "%d\n", n);
        for (i = 0; i < n; i++)
            fprintf(out, "%1.9lf\n", *(X + i));
    }
}

int validateFile(char *filename) {
    FILE *f = fopen(filename, "rb");
    return f != NULL;
}

int validateParameters(int argc, char **argv) {
    int i, cnt = 0;
    if (argc == 1)
        return 0;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && _strlen(argv[i]) == 2)
            switch (argv[i][1]) {
                case 'd':
                    act_d = 1;
                    break;
                case 'e':
                    act_e = 1;
                    break;
                case 'p':
                    act_p = 1;
                    break;
                case 't':
                    act_t = 1;
                    break;
                case 'h':
                    act_h = 1;
                    break;
                case '?':
                    act_q = 1;
                default:
                    return 2;
            }
        else
            cnt++;
    }

    if (cnt > 2)
        return 1;

    return 0;
}

int readInputData(char *inputFile, double **A, double **B, int *n) {
    int i, checkInput;
    FILE *in = fopen(inputFile, "r");

    checkInput = fscanf(in, "%d", n);
    if (checkInput == EOF)
        return 6;

    if (checkInput == 0)
        return 7;

    if (*n <= 0)
        return 7;

    *A = malloc((*n) * (*n) * sizeof(double));
    *B = malloc((*n) * sizeof(double));

    for (i = 0; i < (*n) * (*n); i++) {
        checkInput = fscanf(in, "%lf", (*A + i));
        if (checkInput == EOF)
            return 8;
        if (checkInput == 0)
            return 9;
    }
    for (i = 0; i < (*n); i++) {
        checkInput = fscanf(in, "%lf", (*B + i));
        if (checkInput == EOF)
            return 8;
        if (checkInput == 0)
            return 9;
    }

    return 0;
}

void print(int n, const double *A) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%1.9lf ", *(A + i * n + j));

        printf("\n");
    }
    printf("\n");
}

void printHelp() {
    printf("Usage: lss [input_file_name] [output_file_name] [options]\n"
           "Where options include:\n"
           " -d                print debug messages [default OFF]\n"
           " -e                print errors [default OFF]\n"
           " -p                print matrix [default OFF]\n"
           " -t                print execution time [default OFF]\n"
           " -h, -?            print this and exit");
}

int main(int argc, char *argv[]) {
    int n = 0, value = 0, setInput = 0;          // Число уравнений в систиме
    double *A = NULL;     // Одномерный массив для элементов матрицы системы
    double *B = NULL;      // Одномерный массив для правой части
    double *X = NULL;      // Одномерный массив для ответа
    clock_t start, end; // время для работы программы
    //FILE *intfile = fopen("../lss_19_08_in.txt", "r");  // выходной файл;
    //FILE *outfile = fopen("../lss_19_08_out.txt", "w");  // выходной файл;
    char *inputFile = "lss_19_08_in.txt";
    char *outputFile = "lss_19_08_out.txt";


    switch (validateParameters(argc, argv)) {
        case 1:
            if (act_e) printf("ValidationError: Wrong syntax of parameters. There are more than two filenames\n");
            return 2;
        case 2:
            if (act_e)
                printf("ValidationError: Wrong syntax of parameters. There is no such parameter or you haven't"
                       " set value to one of the parameters\n");
            return 3;
        default:
            break;
    }

    if (act_q || act_h) {
        printHelp();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (!setInput) {
                if (i != 1) {
                    if (act_e) printf("ValidationError: Wrong order of parameters.\n");
                    return 4;
                }
                inputFile = argv[i];
                if (!validateFile(inputFile)) {
                    if (act_e) printf("ValidationError: There is no such file.\n");
                    return 5;
                }
                setInput = 1;
            } else {
                if (i != 2) {
                    if (act_e) printf("ValidationError: Wrong order of parameters.\n");
                    return 4;
                }
                outputFile = argv[i];
            }
        }
    }

    if (!setInput) {
        if (!validateFile(inputFile)) {
            if (act_e) printf("ValidationError: There is no such file.\n");
            return 5;
        }
    }

    switch (readInputData(inputFile, &A, &B, &n)) {
        case 6:
            if (act_e) printf("ValidationError: File is empty.\n");
            return 6;
        case 7:
            if (act_e) printf("ValidationError: n is not a positive integer.\n");
            return 7;
        case 8:
            if (act_e) printf("ValidationError: Not enough elements in the matrix.\n");
            return 8;
        case 9:
            if (act_e) printf("ValidationError: One of the elements of the matrix is not a number.\n");
            return 9;
        default:
            break;
    }


    if ((X = (double *) malloc(n * sizeof(double))) == NULL) { //Выделение памяти для X
        printf("Couldn't allocate memory for X \n");
    }


    start = clock();
    value = lss_19_08(n, A, B, X);
    end = clock();

    if (value == -1)
        if (act_e) printf("Method ne premenim.\n");
    if (value == 1)
        if (act_e) printf("Resheniya net.\n");

    if (act_p) print(n, A);
    if (act_t) printf("ime work of program is: %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);

    writeOutputData(outputFile, n, X, value);

    free(A);//освобождаем память
    free(B);//освобождаем память
    free(X);//освобождаем память
    return value;
}