import torch
import torch.nn as nn

import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression

import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

import sys
import argparse



def plot_decision_boundary(model: torch.nn.Module, X: torch.Tensor, y: torch.Tensor):
    """Plots decision boundaries of model predicting on X in comparison to y.
    Source - https://madewithml.com/courses/foundations/neural-networks/ (with modifications)
    """
    # Put everything to CPU (works better with NumPy + Matplotlib)
    model.to("cpu")
    X, y = X.to("cpu"), y.to("cpu")

    # Setup prediction boundaries and grid
    x_min, x_max = X[:, 0].min() - 0.1, X[:, 0].max() + 0.1
    y_min, y_max = X[:, 1].min() - 0.1, X[:, 1].max() + 0.1
    xx, yy = np.meshgrid(np.linspace(x_min, x_max, 101), np.linspace(y_min, y_max, 101))

    # Make features
    X_to_pred_on = torch.from_numpy(np.column_stack((xx.ravel(), yy.ravel()))).float()

    # Make predictions
    model.eval()
    with torch.inference_mode():
        y_logits = model(X_to_pred_on)

    # Test for multi-class or binary and adjust logits to prediction labels
    if len(torch.unique(y)) > 2:
        y_pred = torch.softmax(y_logits, dim=1).argmax(dim=1)  # mutli-class
    else:
        y_pred = torch.round(y_logits)  # binary

    # Reshape preds and plot
    y_pred = y_pred.reshape(xx.shape).detach().numpy()

    plt.contourf(xx, yy, y_pred, cmap=plt.cm.RdYlBu, alpha=0.7)
    plt.scatter(X[:, 0], X[:, 1], c=[int(i) for i in y], s=40, cmap=plt.cm.RdYlBu)
    plt.xlabel("$X_1$")
    plt.ylabel("$X_2$")
    plt.xlim(xx.min(), xx.max())
    plt.ylim(yy.min(), yy.max())
    plt.show()

def print_np(arr):
	s = f''
	for i in arr[:-1]:
		s += str(i) + '\t'

	s += str(arr[-1])
	return s

def save_training_data(path, X, y):

	df = pd.DataFrame(X, columns=['x1', 'x2'])
	df["y"] = y
	df = df.sort_values(["x1", "y"])
	X = np.array(df[['x1','x2']].values)
	y = np.array(df['y'].values).reshape(-1, 1)

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
						nn.Linear(2, 1),
						nn.Sigmoid()
						)

	def forward(self, x):
		return self.layers(x)

	def predict(self, x):
		return torch.round(self.forward(x))

def train(argv):

	PATH = argv[0]
	epochs = int(argv[1])

	xor_nn = XORNet()
	loss_fn = torch.nn.BCEWithLogitsLoss()
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

		# yhat = xor_nn.forward(X)
		yhat = xor_nn.forward(X)
		loss = loss_fn(yhat, y)

		loss.backward()
		total_loss += loss

		optimizer.step()
		optimizer.zero_grad()

		if epoch % 50 == 0:     
		    print(f'Epoch: {epoch} completed, current Loss = {loss}')
		    # print(f'Last prediction: {yhat.tolist()}')
		    print(f'Last prediction: {xor_nn.predict(X)}')

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
	# x1 = np.linspace(0, 1, int(np.sqrt(n)))
	# x2 = np.linspace(0, 1, int(np.sqrt(n)))
	# X1, X2 = np.meshgrid(x1, x2)
	# Xdata = np.array([X1.flatten(), X2.flatten()]).T
	Xdata = np.random.rand(n, 2)
	Xdata[0][0] = 0.
	Xdata[-1][0] = 1.
	X = torch.Tensor(Xdata)
	yhat = model.forward(X)


	X = X.numpy()
	yhat = yhat.detach().numpy()

	if args['plot']:
		xdata = 	X[:, 0]
		ydata = X[:, 1]
		zdata = yhat
		fig = plt.figure()
		ax = plt.axes(projection='3d')
		ax.scatter3D(xdata, ydata, zdata, cmap='spring')
		plt.show()
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
		plot_decision_boundary(model, X, y)


	if args['train'] or args['save']:
		save_training_data("../data/xor_nn_data.tsv", X, yhat)


if __name__ == "__main__":

	args = argparse.ArgumentParser()

	# Add arguments to the parser
	args.add_argument("-t", "--train", required=False,
	   help="specify if we want to train the model or not")

	args.add_argument("-n", "--n_samples", required=True,
	   help="specify how many points to sample from the network")

	args.add_argument("-p", "--plot", required=False, nargs='?', const=1,
	   help="specify if you want to plot the function achieved by the model or not")

	args.add_argument("-f", "--file", required=True,
	   help="where to save or load the model from")

	args.add_argument("-s", "--save", required=False, nargs='?', const=1,
	   help="if you want to save training data")

	args = vars(args.parse_args())

	main(args)