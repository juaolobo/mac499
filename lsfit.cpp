#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <eigen3/Eigen/Dense>

#include "lsfit.hpp"

using namespace std;
using Eigen::MatrixXd;

LinearRegression::LinearRegression() {
	error = 0;
}
LinearRegression LinearRegression::fit(MatrixXd X, MatrixXd y) {

	weights = solve(X, y);
	MatrixXd err_vec = predict(X) - y;
	error = sqrt((err_vec.array().square()).sum())/(err_vec.cols() * err_vec.rows());

	return *this;
}

LinearRegression LinearRegression::fit(MatrixXd X, MatrixXd y, double lr) {

	weights = MatrixXd::Random(X.cols(), y.cols());
	MatrixXd dw;

	MatrixXd last_err = MatrixXd::Zero(y.rows(), y.cols());
	MatrixXd err;

	double alpha = lr;
	double de = 0;

	for (int i = 0; i < EPOCHS; i++) {

		MatrixXd pred = predict(X);
		err = pred - y;

		dw = gradient_descent(X, y, pred);
		update_params(alpha, dw);

		MatrixXd err_vec = err - last_err;

		// cost function = matrix norm
		de = sqrt((err_vec.array().square()).sum())/(err_vec.cols() * err_vec.rows());

	}
	// cout << "Weights matrix: " << endl << weights << endl;

	error = de;

	return *this;
}

LinearRegression LinearRegression::update_params(double alpha, MatrixXd dw) {

	weights -= alpha * dw;

	return *this;
}

MatrixXd LinearRegression::gradient_descent(MatrixXd X, MatrixXd y, MatrixXd y_pred) {

	MatrixXd err = y_pred - y;
	MatrixXd X_t = X.transpose();
	MatrixXd dw = (X_t * err)/(X.cols() * X.rows());

	return dw;
}

MatrixXd LinearRegression::predict(MatrixXd X) {

	MatrixXd res = X * weights;

	return res;
}

double LinearRegression::score(MatrixXd X, MatrixXd y) {

	MatrixXd y_pred = predict(X);
	MatrixXd diff = y - y_pred;
	MatrixXd y_mean = y.colwise().mean().replicate(y.rows(), 1);
	MatrixXd diff_mean = y - y_mean;

	double u = diff.array().square().sum();
	double v = diff_mean.array().square().sum();

	return (1 - u/v);
}

MatrixXd LinearRegression::solve(MatrixXd X, MatrixXd y) {

	return (X.transpose() * X).ldlt().solve(X.transpose() * y);
}