#ifndef UTILS_2D_H
#define UTILS_2D_H

#include <eigen3/Eigen/Dense>
#include <vector>

using namespace std;
using Eigen::MatrixXd;
using Eigen::Vector3d;
using Eigen::all;
using Eigen::last;
using Eigen::seq;

MatrixXd simplex_segmentation_2D(MatrixXd left_weights, MatrixXd right_weights, MatrixXd X, MatrixXd y, int start, int end);
MatrixXd points_to_plane_2D(MatrixXd X, MatrixXd y);
vector<MatrixXd> partition_domain_2D(MatrixXd X, vector<int> segments);

#endif