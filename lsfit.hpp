#ifndef LSFIT_H
#define LSFIT_H
#include <eigen3/Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;

#define LR 0.01
#define THRESHOLD 0.01

class LinearRegression {

	public:
		LinearRegression();
		MatrixXd weights;
		double error;
		void fit(MatrixXd X, MatrixXd y);
		void update_params(double alpha, MatrixXd dw);
		MatrixXd gradient_descent(MatrixXd X, MatrixXd y, MatrixXd y_pred);
		MatrixXd predict(MatrixXd X);

};

#endif
