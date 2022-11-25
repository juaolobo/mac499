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
	string filename = "data/xor_nn_data.tsv";
	if (argc > 1)
		cost = stof(argv[1]);
	
	if(argc > 2)
		filename = argv[2];

	int n = read_data(filename, X, y);
	double OPT[n+1];
	int opt_segment[n+1];

	double** E = new double* [n+1];
	for (int i = 0; i <= n; i++)
		E[i] = new double [n+1];

	double result = segmented_regression(n, X, y, cost, OPT, E, opt_segment);
	
	vector<MatrixXd> pieces;
	vector<int> segments;
	vector<bool> simplex_id = reconstruct_solution(X, y, pieces, segments, n, OPT, E, opt_segment);

	cout << pieces.size() << endl;
	vector<MatrixXd> boundaries = partition_domain_2D(X, segments);

	for (int i = 0; i < pieces.size(); i++)
		cout << pieces[i] << endl;

	for (int i = 0; i < n+1; i++)
		delete[] E[i];

	delete[] E;

	regression_to_pwl(pieces, boundaries, simplex_id, 5000);
	return 0;
}