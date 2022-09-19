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

using namespace std;
using Eigen::MatrixXd;

int main(){
	
	MatrixXd X, y;

	int n = read_data("../../data/shipment_sorted.tsv", X, y);

	double OPT[n+1];
	int opt_segment[n+1];

	double** E = new double* [n+1];
	for (int i = 0; i <= n; i++)
		E[i] = new double [n+1];

	double result = segmented_regression(n, X, y, 0.05, OPT, E, opt_segment);
	
	stack<int> segments = reconstruct_solution(n, OPT, E, opt_segment);

	for (int i = 0; i < n+1; i++)
		delete[] E[i];

	delete[] E;

	return 0;
}