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

// OPT[j] is the minimum cost for points p_1, p_2, ..., p_j
double OPT[N+1];
// E[i][j] is the minimum error for points p_i, p_i+1, ..., p_j
double E[N+1][N+1];
// opt_segment[j] is the 
int opt_segment[N+1];

int optimal_fit(int j) {

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

double calculate_regression(MatrixXd X, MatrixXd y, int C) {

	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= i; j++) {
			MatrixXd X_ = X(seq(i, i+j), all);
			MatrixXd y_ = y(seq(i, i+j), all);

			// MatrixXd X_ = slice(X, i, i+j);
			// MatrixXd y_ = slice(y, i, i+j);

			LinearRegression lsqr = LinearRegression();
			lsqr.fit(X_, y_);
			E[i][j] = lsqr.error;
		}

	OPT[0] = opt_segment[0] = 0;
	for (int j = 1; j <= N; j++) {
		int i = optimal_fit(j);
		opt_segment[j] = i;
		OPT[j] = E[i][j] + OPT[i-1] + C;
	}

	return OPT[N];
}