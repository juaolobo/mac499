#include <iostream>
#include <vector>
#include <limits>
#include "lsfit.hpp"
#include "point.hpp"
#include "lsfit.hpp"
#include "slice.hpp"

#define N 10
#define INF numeric_limits<double>::infinity()

using namespace std;


double OPT[N+1];
double E[N+1][N+1];
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

double calculate_regression(vector<Point> X, vector<Point> y, int C) {

	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= i; j++) {
			vector<Point> X_ = slice(X, i, i+j);
			vector<Point> y_ = slice(y, i, i+j);
			LSFit lsqr = LSFit(X_, y_, 0, 0);
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