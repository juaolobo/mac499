#ifndef LSFIT_H
#define LSFIT_H
#include <eigen3/Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;

class LinearRegression {

	public:
		LinearRegression();
		MatrixXd weights;
		double error;
		LinearRegression fit(MatrixXd X, MatrixXd y);
		LinearRegression update_params(double alpha, MatrixXd dw);
		MatrixXd gradient_descent(MatrixXd X, MatrixXd y, MatrixXd y_pred);
		MatrixXd predict(MatrixXd X);
		double score(MatrixXd X, MatrixXd y);

};

#endif
