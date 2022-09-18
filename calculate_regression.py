import pandas as pd
from sklearn.linear_model import LinearRegression

with open('regression.tsv') as f:
	values = [int(v) for v in f.readline().split("\t")]
	x_len = values[1]
	y_len = values[2]

data = pd.read_csv('regression.tsv', sep='\t', skiprows=[0], names=[i for i in range(x_len + y_len)])

X = data[[i for i in range(x_len)]]
y = data[[i + x_len for i in range(y_len)]]

reg = LinearRegression().fit(X, y)
print(reg.coef_, reg.intercept_)