#include <iostream>
#include <vector>
#include <fstream>
#include <eigen3/Eigen/Dense>

using namespace std;
using Eigen::RowVectorXd;
using Eigen::MatrixXd;

vector<double> get_values_from_line(string line) {

	vector<double> values;

	int w_size = 0, pos = 0;
	for (int i = 0; (unsigned int) i < line.size(); i++, w_size++){

		if (line[i] == '\t'){
			values.push_back(stold(line.substr(pos, w_size)));
			i++;
			pos = i;
			w_size = 0;
		}
	}

	values.push_back(stold(line.substr(pos, w_size)));

	return values;
}

int read_data(string filename, MatrixXd &X, MatrixXd &y) {

	ifstream file(filename);

	string line;
	int n, x_dim, y_dim;
	vector<double> values;

	getline(file, line);
	values = get_values_from_line(line);
	n = (int) values[0];
	x_dim = (int) values[1];
	y_dim = (int) values[2];
	X = MatrixXd(n, x_dim + 1);
	y = MatrixXd(n, y_dim);

	int line_idx = 0;
	while(getline(file, line)) {

		RowVectorXd x_point(x_dim), y_point(y_dim); 

		values = get_values_from_line(line);

		X(line_idx, 0) = 1; // add bias column
		for(int i = 0; i < x_dim; i++)
			X(line_idx, i+1) = values[i];

		for(int j = x_dim; j < x_dim + y_dim; j++) {
			int idx = j - x_dim;
			y(line_idx, idx) = values[j];
		}

		line_idx++;
	}

	return n;
}