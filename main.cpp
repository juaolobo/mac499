#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <utility>
#include <eigen3/Eigen/Dense>

#include "constants.hpp"
#include "read_data.hpp"
#include "lsfit.hpp"
#include "seg_reg.hpp"
#include "2d_utils.hpp"

using namespace std;
using Eigen::MatrixXd;

int main(int argc, char** argv){
	
	MatrixXd X, y;

	double cost = 0.1;
	if (argc > 1) {
		cost = stof(argv[1]);
		cout << cost << endl;
	}

	int n = read_data("../data/regression.tsv", X, y);
	double OPT[n+1];
	int opt_segment[n+1];

	double** E = new double* [n+1];
	for (int i = 0; i <= n; i++)
		E[i] = new double [n+1];

	double result = segmented_regression(n, X, y, cost, OPT, E, opt_segment);
	
	vector<MatrixXd> pieces;
	vector<int> segments;
	reconstruct_solution(X, y, pieces, segments, n, OPT, E, opt_segment);

	// vector<MatrixXd> boundaries = partition_domain_2D(X, segments);

	for (int i = 0; i < n+1; i++)
		delete[] E[i];

	delete[] E;

	// regression_to_pwl(pieces, boundaries, 10000);
	return 0;
}