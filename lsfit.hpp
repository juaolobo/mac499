#ifndef LSFIT_H
#define LSFIT_H
#include "point.hpp"

using namespace std;

class LSFit {

	public:
		LSFit(vector<Point> X, vector<Point> y, int epochs, double alpha);
		vector<double> weights;
		double error;

};

#endif
