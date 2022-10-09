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
	X, y = make_regression(n_features=x_dim, n_targets=y_dim, n_samples=n, noise=0.3)
	Xs.append(X)
	ys.append(y)

X = np.concatenate(Xs)
y = np.concatenate(ys)

df = pd.DataFrame(zip(X, y), columns=['X', 'y'])
# df = df.sort_values("X")

X = np.array([np.array(v) for v in df['X'].values])
y = np.array([np.array(v) for v in df['y'].values])

# y = y.reshape(*y.shape, 1)
if len(y.shape) < 2:
	y = y.reshape(-1, 1)
	
if len(X.shape) < 2:
	X = X.reshape(-1, 1)

if exists('../data/regression.tsv'):
	os.remove('../data/regression.tsv')

def print_np(arr):
	s = f''
	for i in arr[:-1]:
		s += str(i) + '\t'

	s += str(arr[-1])
	return s

print(X.shape, y.shape)
with open('../data/regression.tsv', 'a') as f:
	f.write(str(X.shape[0]) + '\t' + str(X.shape[1]) + '\t' + str(y.shape[1]) + '\n')
	for i in range(len(X)):
		data = print_np(X[i]) + '\t' + print_np(y[i]) + '\n'
		f.write(data)