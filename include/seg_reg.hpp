#ifndef SEG_REG_H
#define SEG_REG_H
#include <eigen3/Eigen/Dense>
#include <stack>
#include <utility>

int optimal_fit(int j, double* OPT, double** E);
double segmented_regression(int N, MatrixXd X, MatrixXd y, double C, double* OPT, double** E, int* opt_segment);
vector<bool> reconstruct_solution(MatrixXd X, MatrixXd y, vector<MatrixXd> &pieces, vector<int> &segments,
							int N, double* OPT, double** E, int* opt_segment);
pair<int, int> double_to_fraction(double x, int N);
MatrixXd get_piece(MatrixXd X, MatrixXd y, int start, int end);
void regression_to_pwl(vector<MatrixXd> pieces, vector<MatrixXd> boundaries, vector<bool> simplex_id, int N);

#endif