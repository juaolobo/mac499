import pandas as pd
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt

filename = '../../data/shipment.tsv'
with open(filename) as f:
	values = [int(v) for v in f.readline().split("\t")]
	x_len = values[1]
	y_len = values[2]

data = pd.read_csv(filename, sep='\t', skiprows=[0], names=[i for i in range(x_len + y_len)])

data.columns = ['x', 'y']
data = data.sort_values('x')
print(data)
data.plot(x='x', y='y', kind='scatter')
plt.show()

X = data[[i for i in range(x_len)]]
y = data[[i + x_len for i in range(y_len)]]

reg = LinearRegression().fit(X, y)
print(reg.coef_, reg.intercept_)