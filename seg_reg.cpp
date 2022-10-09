#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <eigen3/Eigen/Dense>
#include "lsfit.hpp"
#include "seg_reg.hpp"
#include "constants.hpp"

using namespace std;
using Eigen::VectorXd;
using Eigen::Vector3d;
using Eigen::MatrixXd;
using Eigen::seq;
using Eigen::all;
using Eigen::last;

int optimal_fit(int j, double* OPT, double** E) {

	double tmp, minimum = INF;
	int k = 0;
	for (int i = 1; i <= j; i++) {
		tmp = E[i][j] + OPT[i-1];
		if (minimum > tmp) {
			k = i;
			minimum = tmp;
		}
	}	

	return k;
}

double segmented_regression(int N, MatrixXd X, MatrixXd y, double C, double* OPT, double** E, int* opt_segment) {

	cout << "Calculating segmented regression for input data points. (C = " << C << ")" << endl;
	for (int j = 1; j <= N; j++)
		for (int i = 1; i <= j; i++) {

			int start = i-1;
			int end = j-1;

			MatrixXd X_ = X(seq(start, end), all);
			MatrixXd y_ = y(seq(start, end), all);

			LinearRegression lsqr = LinearRegression().fit(X_, y_);
			E[i][j] = lsqr.error;
		}

	OPT[0] = opt_segment[0] = 0;
	for (int j = 1; j <= N; j++) {
		int i = optimal_fit(j, OPT, E);
		opt_segment[j] = i;
		OPT[j] = E[i][j] + OPT[i-1] + C;
	}

	cout << "Total cost of the piece wise approximation (OPT[N]): " << OPT[N] << endl;
	return OPT[N];
}

vector<MatrixXd> reconstruct_solution(MatrixXd X, MatrixXd y, int N, double* OPT, double** E, int* opt_segment) {

	stack <int> segments;
	int i = N, j = opt_segment[N];

	while (i > 0) {
		segments.push(i);
		segments.push(j);
		i = j-1;
		j = opt_segment[i];
	}

	// save the solution for returning
	vector<MatrixXd> pieces;
	int last_j = -1, start, end;
	MatrixXd X_;
	MatrixXd y_;
	cout << "An optimal solution :" << endl;
	while (!segments.empty()) {

		i = segments.top(); segments.pop();
		j = segments.top(); segments.pop();

		if (last_j >= 0) {
			start = last_j - 1;
			end = i-1;
			X_ = X(seq(start, end), all);
			y_ = y(seq(start, end), all);
			pieces.push_back(LinearRegression().fit(X_, y_).weights);
			cout << "Segment from points " << last_j << " to " << i << endl;
		}

		cout << "Segment from points " << i << " to " << j << endl;

		start = i-1;
		end = j-1;
		X_ = X(seq(start, end), all);
		y_ = y(seq(start, end), all);
		pieces.push_back(LinearRegression().fit(X_, y_).weights);
		last_j = j;

	}

	return pieces;
}

pair<int, int> double_to_fraction(double x, int N) {

	int a = (int) x;
	int b = 1;
	int c = (int) x + 1;
	int d = 1;

	while(b <= N && d <= N) {

		double mediant = (double) (a+c)/(b+d);
		if (x == mediant) {

			if (b + d <= N)
				return make_pair((a+c), (b+d));

			else if (d > b) 
				return make_pair(c, d);

			else 
				return make_pair(a, b);
		}

		else if (x > mediant) {
			a = a+c;
			b = b+d;
		}
		else {
			c = a+c;
			d = b+d;
		}
	}

	if (b > N)
		return make_pair(c, d);

	else
		return make_pair(a, b);
}

void regression_to_pwl(vector<MatrixXd> pieces, int N) {

	string filename = "function.pwl";
	ofstream file(filename);
	file << "pwl" << endl;

	for (int k = 0; (unsigned int) k < pieces.size(); k++){

		MatrixXd weights = pieces[k];
		file << "p";

		for (int i = 0; i < weights.rows(); i++)
			for (int j = 0; j < weights.cols(); j++){
				pair<int, int> fraction = double_to_fraction(abs(weights(i, j)), N);

				if (weights(i, j) < 0)
					file << " " << -fraction.first << " " << fraction.second;
				else
					file << " " << fraction.first << " " << fraction.second;
		}
		file << endl;
	}

}

MatrixXd simplex_segmentation_2D(MatrixXd weights, MatrixXd X, MatrixXd y, int start, int end) {

	// p1, p2, p3 in [0,1]^2
	// p1 = (X(start), 0); p2 = (X(start), 1); p3 = (X(end), 0)

	MatrixXd p1 = X(start, all); p1(2) = 0;
	MatrixXd p2 = X(start, all); p2(2) = 1;
	MatrixXd p3 = X(end, all); p3(2) = 0;
	MatrixXd points(3, X.cols());
	points << p1, p2, p3;

	MatrixXd preds = points * weights;

	MatrixXd Z = points_to_plane_2D(points, preds);

	return Z;

}

MatrixXd points_to_plane_2D(MatrixXd X, MatrixXd y) {

	Vector3d p1;
	p1(seq(0, last-1)) = X(0, seq(1, last));
	p1(2) = y(0);

	Vector3d p2;
	p2(seq(0, last-1)) = X(1, seq(1, last));
	p2(2) = y(1);

	Vector3d p3;
	p3(seq(0, last-1)) = X(2, seq(1, last));
	p3(2) = y(2);

	Vector3d v = p3 - p1;
	Vector3d w = p2 - p1;

	// c = a, b, c
	Vector3d c = v.cross(w);
	double d = c.dot(p3);

	// z = (- ax - by + d)/c
	MatrixXd z(c.size(), 1);
	z << d, -c(0), -c(1);
	z = z/c(2);

	return z;
}