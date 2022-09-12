from sklearn.datasets import make_regression
import pandas as pd
import os
from os.path import exists

X, y = make_regression(n_features=2, n_targets=2, n_samples=10)

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