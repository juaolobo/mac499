#include <iostream>
#include <vector>
#include <stdlib.h>
#include "lsfit.hpp"
#include "point.hpp"

using namespace std;

LSFit::LSFit(vector<Point> X, vector<Point> y, int epochs, double alpha_) {


	vector<double> weights(X.size() + 1, 0);
	Point p(X.size());
	vector<double> err(X.size(), 0);
	bool converged = 0;
	double alpha = 0.01;

	while (!converged)

		for (int i = 0; (unsigned int) i < X.size(); i++) {
			Point x = X[i];
			Point y = y[i];

			for (int ii = 1; (unsigned int) ii < weights.size(); ii++)
				p[ii] = weights[0] + x.coordinates[ii]*weights[ii];

			double err_ = inner_prod(y-p, y-p);

			weights[0] -= alpha * err_;

			for (int ii = 1; (unsigned int) ii < weights.size(); ii++)
				weights[ii] -= alpha * err_ * x.coordinates[ii];

			if (abs(err[err.size()-1] - err_) < 0.001)
				converged = 1;

			err.push_back(err_);

		}

	for (int i = 0; (unsigned int) i < err.size(); i++)
		error += err[i]; 


}
