import pandas as pd
import matplotlib.pyplot as plt

table = pd.read_csv("C:/Users/Professional/Desktop/Task-1/dataout.csv")

x = table["x"].values
target = table["y"].values
predicted = table["y_pred"].values

plt.title("Графики целевой и предсказанной функций")
plt.plot(x, predicted, label="Предсказанный")
plt.plot(x, target, label="Цель")
plt.legend()
plt.xlabel("$x$")
plt.ylabel("$f(x)$")

plt.show()