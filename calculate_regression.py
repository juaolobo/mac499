import pandas as pd
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt
import numpy as np

filename = '../data/regression.tsv'
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

reg = LinearRegression().fit(data['x'].values.reshape(-1, 1), data['y'])

x = np.linspace(-3, 1)
y = reg.predict(x.reshape(-1, 1))
print(reg.coef_, reg.intercept_)
plt.scatter(x, y)
plt.show()