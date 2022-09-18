#include <iostream>
#include <vector>
#include <limits>
#include <eigen3/Eigen/Dense>
#include "lsfit.hpp"
#include "seg_reg.hpp"
#include "constants.hpp"

using namespace std;
using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::seq;
using Eigen::all;

int optimal_fit(int j, double* OPT, double** E) {

	double tmp, minimum = INF;
	int k;
	for (int i = 1; i <= j; i++){
		tmp = E[i][j] + OPT[i-1];
		if (minimum > tmp){
			k = i;
			minimum = tmp;
		}
	}	

	return k;
}

double segmented_regression(int N, MatrixXd X, MatrixXd y, int C, double* OPT, double** E, int* opt_segment) {

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

stack<int> reconstruct_solution(int N, double* OPT, double** E, int* opt_segment) {

	stack <int> tmp_segments;
	int i = N, j = opt_segment[N];

	while (i > 0) {
		tmp_segments.push(i);
		tmp_segments.push(j);
		i = j-1;
		j = opt_segment[i];
	}

	// save the solution for returning
	stack<int> segments = tmp_segments;

	cout << "An optimal solution :" << endl;
	while (!tmp_segments.empty())	{
		i = tmp_segments.top(); tmp_segments.pop();
		j = tmp_segments.top(); tmp_segments.pop();
		cout << "Segment from points " << i << " to " << j << endl;
	}

	return segments;
}