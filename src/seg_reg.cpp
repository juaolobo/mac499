#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <eigen3/Eigen/Dense>
#include "lsfit.hpp"
#include "seg_reg.hpp"
#include "constants.hpp"

using namespace std;
using Eigen::VectorXd;
using Eigen::Vector3d;
using Eigen::MatrixXd;
using Eigen::seq;
using Eigen::all;
using Eigen::last;

int optimal_fit(int j, double* OPT, double** E) {

	double tmp, minimum = INF;
	int k = 0;
	for (int i = 1; i <= j; i++) {
		tmp = E[i][j] + OPT[i-1];
		if (minimum > tmp) {
			k = i;
			minimum = tmp;
		}
	}	

	return k;
}

double segmented_regression(int N, MatrixXd X, MatrixXd y, double C, double* OPT, double** E, int* opt_segment) {

	cout << "Calculating segmented regression for input data points. (C = " << C << ")" << endl;
	for (int j = 1; j <= N; j++)
		for (int i = 1; i <= j; i++) {

			int start = i-1;
			int end = j-1;

			MatrixXd X_ = X(seq(start, end), all);
			MatrixXd y_ = y(seq(start, end), all);

			LinearRegression lsqr = LinearRegression().fit(X_, y_);
			E[i][j] = lsqr.error;
		}

	OPT[0] = opt_segment[0] = 0;
	for (int j = 1; j <= N; j++) {
		int i = optimal_fit(j, OPT, E);
		opt_segment[j] = i;
		OPT[j] = E[i][j] + OPT[i-1] + C;
	}

	cout << "Total cost of the piece wise approximation (OPT[N]): " << OPT[N] << endl;
	return OPT[N];
}

MatrixXd get_piece(MatrixXd X, MatrixXd y, int start, int end) {

	MatrixXd X_ = X(seq(start, end), all);
	MatrixXd y_ = y(seq(start, end), all);
	
	return LinearRegression().fit(X_, y_).weights;
}

void reconstruct_solution(MatrixXd X, MatrixXd y, vector<MatrixXd> &pieces, vector<int> &segments,
							int n, double* OPT, double** E, int* opt_segment) {

	stack<int> segments_stack;
	int i = n, j = opt_segment[n];

	while (i > 0) {
		segments_stack.push(i);
		segments_stack.push(j);
		i = j-1;
		j = opt_segment[i];
	}

	int last_j = -1, start, end, n_pieces = 0;

	cout << "An optimal solution :" << endl;

	// save the solution for returning
	while (!segments_stack.empty()) {

		i = segments_stack.top(); segments_stack.pop();
		j = segments_stack.top(); segments_stack.pop();

		if (last_j >= 0) {
			start = last_j - 1;
			end = i-1;
			// MatrixXd piece = get_piece(X, y, start, end);
			MatrixXd piece1 = simplex_segmentation_2D(pieces[n_pieces], X, y, start, end);
			MatrixXd piece2 = simplex_segmentation_2D(pieces[n_pieces], X, y, end, start);
			pieces.push_back(piece);
			cout << "Simplex Segments from points " << last_j << " to " << i << endl;
			cout << "piece: " << piece << endl;
			segments.push_back(end);
		}


		start = i-1;
		end = j-1;
		MatrixXd piece = get_piece(X, y, start, end);
		pieces.push_back(piece);
		segments.push_back(end);

		cout << "Segment from points " << i << " to " << j << endl;
		cout << "piece: " << piece << endl;
		last_j = j;
		n_pieces ++;

	}

}

pair<int, int> double_to_fraction(double x, int N) {

	int a = (int) x;
	int b = 1;
	int c = (int) x + 1;
	int d = 1;

	while(b <= N && d <= N) {

		double mediant = (double) (a+c)/(b+d);
		if (x == mediant) {

			if (b + d <= N)
				return make_pair((a+c), (b+d));

			else if (d > b) 
				return make_pair(c, d);

			else 
				return make_pair(a, b);
		}

		else if (x > mediant) {
			a = a+c;
			b = b+d;
		}
		else {
			c = a+c;
			d = b+d;
		}
	}

	if (b > N)
		return make_pair(c, d);

	else
		return make_pair(a, b);
}

void regression_to_pwl(vector<MatrixXd> pieces, vector<MatrixXd> boundaries, int N) {

	string filename = "function.pwl";
	ofstream file(filename);
	file << "pwl" << endl;

	for (int b = 0; (unsigned int) b < boundaries.size(); b++){

		file << "b";
		if (boundaries[b].size() > 1) {
			for (int i = 0; i < boundaries[b].size(); i++)
				file << boundaries[b](i);

			file << "0" << endl;
		}

		else {
			file << "0";
			file << boundaries[b];
			file << "0" << endl;
		}
	}

	for (int k = 0; (unsigned int) k < pieces.size(); k++){

		MatrixXd weights = pieces[k];
		file << "p";

		for (int i = 0; i < weights.rows(); i++)
			for (int j = 0; j < weights.cols(); j++){
				pair<int, int> fraction = double_to_fraction(abs(weights(i, j)), N);

				if (weights(i, j) < 0)
					file << " " << -fraction.first << " " << fraction.second;
				else
					file << " " << fraction.first << " " << fraction.second;
		}
		file << endl;
	}
}