#ifndef SEG_REG_H
#define SEG_REG_H
#include <eigen3/Eigen/Dense>
#include <stack>
#include <utility>

int optimal_fit(int j, double* OPT, double** E);
double segmented_regression(int N, MatrixXd X, MatrixXd y, double C, double* OPT, double** E, int* opt_segment);
vector<MatrixXd> reconstruct_solution(MatrixXd X, MatrixXd y, int N, double* OPT, double** E, int* opt_segment);
pair<int, int> double_to_fraction(double x, int N);
void regression_to_pwl(vector<MatrixXd> pieces, int N);
MatrixXd simplex_segmentation_2D(MatrixXd weights, MatrixXd X, MatrixXd y, int start, int end);
MatrixXd points_to_plane_2D(MatrixXd X, MatrixXd y);

#endif