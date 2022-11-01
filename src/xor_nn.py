import torch
import torch.nn as nn

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

import sys
import argparse

def print_np(arr):
	s = f''
	for i in arr[:-1]:
		s += str(i) + '\t'

	s += str(arr[-1])
	return s

def save_training_data(path, X, y):

	X = np.array(sorted(X, key=lambda x: x[0]))
	y = np.array(sorted(y))
	with open(path, 'w') as f:
		f.write(str(X.shape[0]) + '\t' + str(X.shape[1]) + '\t' + str(y.shape[1]) + '\n')
		for i in range(len(X)):
			data = print_np(X[i]) + '\t' + print_np(y[i]) + '\n'
			f.write(data)

class XORNet(nn.Module):
	def __init__(self):
		super(XORNet, self).__init__()
		self.layers = nn.Sequential(
						nn.Linear(2, 2),
						nn.Sigmoid(),
						nn.Linear(2, 1)
						)

	def forward(self, x):
		return self.layers(x)

def train(argv):

	PATH = argv[0]
	epochs = int(argv[1])

	xor_nn = XORNet()
	loss_fn = torch.nn.MSELoss()
	optimizer = torch.optim.Adam(xor_nn.parameters(), lr=0.03)

	X = torch.Tensor([
						[0.0, 0.0],
						[0.0, 1.0],
						[1.0, 0.0],
						[1.0, 1.0],
					])

	y = torch.Tensor([
						[0.0],
						[1.0],
						[1.0],
						[0.0],
					])

	total_loss = 0
	for epoch in range(epochs):

		yhat = xor_nn.forward(X)
		loss = loss_fn(yhat, y)

		loss.backward()
		total_loss += loss

		optimizer.step()
		optimizer.zero_grad()

		if epoch % 50 == 0:     
		    print(f'Epoch: {epoch} completed, current Loss = {loss}')
		    print(f'Last prediction: {yhat.tolist()}')

	print(f'Training completed, total Loss = {total_loss}')

	torch.save(xor_nn.state_dict(), PATH)

def load(PATH):

	model = XORNet()
	model.load_state_dict(torch.load(PATH))

	return model

def main(args):
	PATH = args['file']

	if args['train']:
		train([PATH, args['train']])

	model = load(PATH)
	n = int(args['n_samples'])
	Xdata = np.random.rand(n, 2)
	Xdata[0][0] = 0.
	Xdata[-1][0] = 1.
	X = torch.Tensor(Xdata)
	yhat = model.forward(X)

	X = X.numpy()
	yhat = yhat.detach().numpy()

	if args['plot']:
		xdata = X[:, 0]
		ydata = X[:, 1]
		zdata = yhat
		fig = plt.figure()
		ax = plt.axes(projection='3d')
		ax.scatter3D(xdata, ydata, zdata, cmap='viridian')

		plt.show()

	if args['train'] or args['save']:
		save_training_data("data/xor_nn_data.tsv", X, yhat)


if __name__ == "__main__":

	args = argparse.ArgumentParser()

	# Add arguments to the parser
	args.add_argument("-t", "--train", required=False,
	   help="specify if we want to train the model or not")

	args.add_argument("-n", "--n_samples", required=False,
	   help="specify how many points to sample from the network")

	args.add_argument("-p", "--plot", required=False, nargs='?', const=1,
	   help="specify if you want to plot the function achieved by the model or not")

	args.add_argument("-f", "--file", required=True,
	   help="where to save or load the model from")

	args.add_argument("-s", "--save", required=True, nargs='?', const=1,
	   help="if you want to save training data")

	args = vars(args.parse_args())

	main(args)