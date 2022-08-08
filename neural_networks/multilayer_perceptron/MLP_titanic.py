# -*- coding: utf-8 -*-
import numpy as np
import scipy.special as sp
import matplotlib.pyplot as plt
# библиотека для работы с dataframe
import pandas as pd

# активационная функция (1/(e^(-x)))
def f(x):
    return sp.expit(x)

# производная активационной функции
def f1(x):
    return x * (1 - x)

# функция инициализации весов
# на вход имеет несколько аргументов:
#    inputs - количество входных узлов
#    n_hidden1 - количество узлов 1го скрытого слоя
#    n_hidden2 - количество узлов 2го скрытого слоя
#    outputs - количество узлов выходного слоя
def init_weights(n_inputs, n_hidden1, n_hidden2, n_outputs):
    #    матрица весов от входного слоя к 1му скрытому слою
    #    матрица единиц размера [inputs х hiddens]
    W1 = np.ones((n_inputs, n_hidden1))
    #    матрица весов от 1го скрытого слоя к 2му скрытому слою
    #    матрица единиц размера [hiddens+1 х hiddens2]
    #    эта матрица имеет кол-во строк на единицу больше т.к. нужно учитывать мнимую единицу
    W2 = np.ones((n_hidden1 + 1, n_hidden2))
    #    матрица весов от 2го скрытого слоя к выходному слою
    #    матрица единиц размера [hiddens2+1 х outputs]
    #    эта матрица имеет кол-во строк на единицу больше т.к. нужно учитывать мнимую единицу
    W3 = np.ones((n_hidden2 + 1, n_outputs))
    return W1, W2, W3

# функция тренировки сети
# на вход имеет несколько аргументов:
#    inputs_list - обучающее множество (входные сигналы)
#    w1 - матрица весов от входного слоя к 1му скрытому слою
#    w2 - матрица весов от 1го скрытого слоя к 2му скрытому слою
#    w3 - матрица весов от 2го скрытого слоя к выходному слою
#    targets_list - целевое множество
#    lr - скорость обучения сети
#    error - допустимая погрешность в обучении
def train(inputs, W1, W2, W3, targets, learning_rate, eps):
    #    счетчик эпох
    era = 0
    #    глобальная ошибка
    error = 1
    #    список ошибок
    error_list = []
    # главный цикл обучения, повторяется пока глобальная ошибка больше погрешности
    while error > eps:
        #        локальная ошибка
        local_error = np.array([])
        # побочный цикл, прогоняющий данные с input_list
        # функция enumerate(matrix) возвращает индекс и значение строк
        # которая сохраняется в переменные i, value
        # i - индекс строки input_list
        # value - переменная которая хранит в себе строки матрицы input_list
        for i, value in enumerate(inputs):
            # переводит листа inputs в двумерный вид (для возможности проведения операции транспонирования)
            # targets - содержит локальный таргет для данного инпута
            value = np.array(value, ndmin=2)
            target = np.array(targets[i], ndmin=2)

            #  прямое распространение
            #  скалярное произведение строки на матрицу весов
            hidden_in1 = np.dot(value, W1)
            # применение активационной функции к вектору
            hidden_out1 = f(hidden_in1)
            # добавление в начало вектора мнимой единицы для обучения сети
            hidden_out1 = np.array(np.insert(hidden_out1, 0, [1]), ndmin=2)

            # скалярное произведение строки на матрицу весов
            hidden_in2 = np.dot(hidden_out1, W2)
            # применение активационной функции к вектору
            hidden_out2 = f(hidden_in2)
            # добавление в начало вектора мнимой единицы
            hidden_out2 = np.array(np.insert(hidden_out2, 0, [1]), ndmin=2)

            # скалярное произведение строки на матрицу весов
            final_in = np.dot(hidden_out2, W3)
            # активационная функция выходного слоя это прямая y = x, поэтому
            # здесь значение "out" равно значение "in"
            final_out = final_in

            # вычисление ошибки выходного слоя
            output_error = target - final_out
            # вычисление ошибки второго скрытого слоя
            hidden_error2 = np.dot(output_error, W3.T)
            # вычисление ошибки первого скрытого слоя
            hidden_error = np.dot(hidden_error2[:, 1:], W2.T)
            # добавление в список локальных ошибок текущую ошибку
            local_error = np.append(local_error, output_error)

            # обратного распространение ошибки
            # изменение матрицы весов 3 т.к. производная активационный функции (y = x)
            # y` = 1 в dW = lr*output_error*hidden_out2.T не умножается на эту производную
            W3 += learning_rate * output_error * hidden_out2.T
            # в методе обратного распространения ошибки исключается мнимая единичка для совпадения размерностей
            # hidden_error2[:,1:] - означает весь вектор за исключением первого элемента
            W2 += learning_rate * hidden_error2[:, 1:] * f1(hidden_out2[:, 1:]) * hidden_out1.T
            W1 += learning_rate * hidden_error[:, 1:] * f1(hidden_out1[:, 1:]) * value.T

        # глобальная ошибка - это средняя по модуля от всех локальных ошибок
        error = abs(np.mean(local_error))
        # global_error = np.sqrt(((local_error) ** 2).mean())
        # эпоха увеличивается на 1
        era += 1
        # в список ошибок добавляется глобальная ошибка
        error_list.append(error)
        # если при обучении количество эпох превысит порог 10000 то обучение прекратится
        if era > 10000: break
    # возвращает измененные веса, количество эпох, и список ошибок
    return W1, W2, W3, era, error_list

# функция для проверки обученной сети и вывода результата
def query(inputs, W1, W2, W3):
    final_out = np.array([])
    for i, value in enumerate(inputs):
        value = np.array(value, ndmin=2)

        hidden_in1 = np.dot(value, W1)
        hidden_out1 = f(hidden_in1)
        hidden_out1 = np.array(np.insert(hidden_out1, 0, [1]), ndmin=2)

        hidden_in2 = np.dot(hidden_out1, W2)
        hidden_out2 = f(hidden_in2)
        hidden_out2 = np.array(np.insert(hidden_out2, 0, [1]), ndmin=2)

        final_in = np.dot(hidden_out2, W3)
        final_out = np.append(final_out, final_in)

    return np.around(final_out)

# считываем данные с csv с помощью библиотеки pandas
# Данные о пассажирах Титаник
# данные предоставлены Яндекс курсом
# задаем столбец по которому будет вести индексирование index_col='PassengerId'
data = pd.read_csv('titanic_data.csv', index_col='PassengerId')

# столбец Survived из data
# .values означает что данные из dataframe конвертируются в numpy array
y = data['Survived'].values

# удаляем из датасета столбец Survived и конвертируем в array
# data = data.drop(columns=['Survived']).values
data = data.drop('Survived', axis=1).values

# составляем выборку обучающего множества из первых 600 строк датасета
X_train = data[0:600]

# добавляем столбец мнимых единичек для множества
X_train = np.c_[np.ones(600), X_train]

# составляем целевое множество
y_train = y[0:600]

# из оставшихся 114 строк составляем тестовое множество
X_test = data[600:714]
X_test = np.c_[np.ones(114), X_test]
y_test = y[600:714]

# скорость обучения
learning_rate = 0.5

# допустимая погрешность обучения (** - это степень)
eps = 10 ** (-9)

# количество узлов в входном слое с учетом единичке
# т.е. кол-во столбцов датасета +1 мнимая единичка
n_inputs = 7

# количество узлов в скрытом слое 1
n_hidden1 = 6
# количество узлов в скрытом слое 2
n_hidden2 = 4
# количество узлов в выходном слое
n_outputs = 1

answers = []
for n_hidden1 in range(5, 11):
    for n_hidden2 in range(4, 6):
        print('Количество узлов 1го скрытого слоя: ', n_hidden1)
        print('Количество узлов 2го скрытого слоя: ', n_hidden2)

        # инициализация весов в зависимости от количества узлов в слоях сети
        W1, W2, W3 = init_weights(n_inputs, n_hidden1, n_hidden2, n_outputs)

        W1, W2, W3, era, errors = train(X_train, W1, W2, W3, y_train, learning_rate, eps)
        print('Количество эпох: ', era)

        y_pred = query(X_test, W1, W2, W3)
        print('Ошибка обучения: ', errors[-1])
        print('Точность: , %', (sum(y_pred == y_test) / len(X_test)) * 100)
        print()
        answers.append([n_hidden1, n_hidden2, era, errors[-1], (sum(y_pred == y_test) / len(X_test)) * 100])

        # отрисовка графика кривой ошибки
        fig = plt.figure(figsize=(20, 20))
        plt.plot(np.arange(era), errors)
        plt.show()
answers