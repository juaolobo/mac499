from sklearn.datasets import make_regression

import pandas as pd
import numpy as np

import os

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from os.path import exists
import sys

if len(sys.argv) < 5:
	print("Usage: python make_regression.py <x_dim> <y_dim> <n_samples> <n_regressions>")
	exit(0)

x_dim = int(sys.argv[1])
y_dim = int(sys.argv[2])
n = int(sys.argv[3])
n_regs = int(sys.argv[4])

Xs, ys = [], []

for i in range(n_regs):
	X, y = make_regression(n_features=x_dim, n_targets=y_dim, n_samples=n+1, noise=0.3)

	# normalize subsequent regressions to `concatenate` all the lines
	if i > 0:
		last_X0 = [x[0] for x in Xs[i-1]]
		X0 = [x[0] for x in X]
		X_min_idx = np.argmin(X0)
		X = [[x[0] + max(last_X0) - min(X0), *x[1:]] for x in X]

		if len(X) > X_min_idx+1 and X_min_idx > 0:
			X = np.concatenate([X[:X_min_idx], X[X_min_idx+1:]])
			y = np.concatenate([y[:X_min_idx], y[X_min_idx+1:]])

		elif len(X) == X_min_idx+1:
			X = X[:X_min_idx]
			y = y[:X_min_idx]

		elif X_min_idx == 0:
			X = X[1:]
			y = y[1:]

	else:
		X = X[1:]
		y = y[1:]

	Xs.append(X)
	ys.append(y)

X = np.concatenate(Xs)
y = np.concatenate(ys)

df = pd.DataFrame(X, columns=[*['X_' + str(i) for i in range(X.shape[1])]])
df = pd.concat([df, pd.Series(y, name='y')], axis=1)
df = df.sort_values("X_0", ignore_index=True);
print(df)
print(X)
X = np.array([np.array(v) for v in df[['X_' + str(i) for i in range(X.shape[1])]].values])
y = np.array([np.array(v) for v in df['y'].values])

# y = y.reshape(*y.shape, 1)
if len(y.shape) < 2:
	y = y.reshape(-1, 1)
	
if len(X.shape) < 2:
	X = X.reshape(-1, 1)

print(X.shape, y.shape)
