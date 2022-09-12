#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <eigen3/Eigen/Dense>
#include "lsfit.hpp"

using namespace std;
using Eigen::MatrixXd;

LinearRegression::LinearRegression() {
	weights = MatrixXd::Random();
	error = 0;
}

void LinearRegression::fit(MatrixXd X, MatrixXd y) {

	weights = MatrixXd(X.cols(), y.cols());

	MatrixXd dw = MatrixXd();

	MatrixXd last_err;
	MatrixXd err;

	bool converged = 0;
	double alpha = 0.01;

	while (!converged) {

		MatrixXd pred = predict(X);
		err = pred - y;

		dw = gradient_descent(X, y, pred);
		update_params(alpha, dw);

		MatrixXd err_vec = last_err - err;
		// cost function
		double de = (err_vec * err_vec).sum()/(err_vec.cols() * err_vec.rows());

		converged = de < 0.01 ? 1 : 0;
	}

	error = (err * err).sum()/(err.cols()*err.rows());

}

void LinearRegression::update_params(double alpha, MatrixXd dw) {

	weights -= alpha * dw;

};

MatrixXd LinearRegression::gradient_descent(MatrixXd X, MatrixXd y, MatrixXd y_pred) {

	MatrixXd err = y_pred - y;

	MatrixXd X_t = X.transpose();
	MatrixXd dw = X_t * (err);

	return dw;
	
}

MatrixXd LinearRegression::predict(MatrixXd X) {

	MatrixXd res = X * weights;

	return res;
}