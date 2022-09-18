from sklearn.datasets import make_regression
import pandas as pd
import os
from os.path import exists
import sys
	
if len(sys.argv) < 4:
	print("Usage: python make_regression.py <x_dim> <y_dim> <n_samples>")
	exit(0)

x_dim = int(sys.argv[1])
y_dim = int(sys.argv[2])
n = int(sys.argv[3])

X, y = make_regression(n_features=x_dim, n_targets=y_dim, n_samples=n, noise=0.3)

# y = y.reshape(*y.shape, 1)
print(y)
if len(y.shape) < 2:
	y = y.reshape(-1, 1)
	
if exists('./regression.tsv'):
	os.remove('./regression.tsv')

def print_np(arr):
	s = f''
	for i in arr[:-1]:
		s += str(i) + '\t'

	s += str(arr[-1])
	return s

with open('regression.tsv', 'a') as f:
	f.write(str(X.shape[0]) + '\t' + str(X.shape[1]) + '\t' + str(y.shape[1]) + '\n')
	for i in range(len(X)):
		data = print_np(X[i]) + '\t' + print_np(y[i]) + '\n'
		f.write(data)