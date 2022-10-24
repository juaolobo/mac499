def print_np(arr):
	s = f''
	for i in arr[:-1]:
		s += str(i) + '\t'

	s += str(arr[-1])
	return s

def save_training_data(path):
	
	with open(path, 'w') as f:
		f.write(str(X.shape[0]) + '\t' + str(X.shape[1]) + '\t' + str(y.shape[1]) + '\n')
		for i in range(len(X)):
			data = print_np(X[i]) + '\t' + print_np(y[i]) + '\n'
			f.write(data)