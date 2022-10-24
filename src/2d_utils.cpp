#include "2d_utils.hpp"
#include <eigen3/Eigen/Dense>
#include <vector>

using namespace std;

MatrixXd simplex_segmentation_2D(MatrixXd weights, MatrixXd X, MatrixXd y, int start, int end) {

	/* 
	p1, p2, p3 in [0,1]^2
	p1 = (X(start), 0); p2 = (X(start), 1); p3 = (X(end), 0)
	if start > end, we are calling for the bottom half simplex
	else we want the top half one
	in other words, first case we want two point with x2 = 0
	and in the second case we want two points with x2 = 1
	*/
	MatrixXd p1 = X(start, all); p1(2) = 0;
	MatrixXd p2 = X(start, all); p2(2) = 1;

	if (start > end)
		MatrixXd p3 = X(end, all); p3(2) = 0;

	else if (start < end) 
		MatrixXd p3 = X(end, all); p3(2) = 1;		

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

	// z = f(x, y) = (- ax - by + d)/c
	MatrixXd z(c.size(), 1);
	z << d, -c(0), -c(1);
	z = z/c(2);

	return z;
}

vector<MatrixXd> partition_domain_2D(MatrixXd X, vector<int> segments) {

	vector<MatrixXd> boundaries;
	MatrixXd zero(1,1);
	zero << 0;
	boundaries.push_back(zero);

	// boundaries: 1 point should be greater than boundary i and less than boundary i+1

	for (int i = 0; i < segments.size(); i++){

		MatrixXd p = X(segments[i], all);

		if (segments[i] - segments[i-1] == 1){

			/* partition by simplex
			in the 2D case we have a line which passes through 
			q = (X(start)[1], 1)
			q2 = (X(end)[1], 0)
			creating two triangles in the domain
			X(start)[1] = x1, X(start)[2] = x2
			*/
			MatrixXd p2 = X(segments[i-1], all);

			MatrixXd q(2, 1);
			MatrixXd q2(2, 1);

			// 0-th column is a bias column for regression
			q << p(1), 1;
			q2 << p2(1), 0;

			MatrixXd slope_vector = q - q2;

			double slope = slope_vector(0)/slope_vector(1);
			MatrixXd line(3, 1);
			double c = q(0) - q(1)*slope;

			// o_0 + o_1 x_1 + o_2 x_2
			line << c, slope, 1;
			boundaries.push_back(line);
		}

		// 0-th column is a bias column for regression
		MatrixXd x_coord;
		x_coord << p(1);
		boundaries.push_back(x_coord);
	}

	return boundaries;

}
