#ifndef SEG_REG_H
#define SEG_REG_H
#include <eigen3/Eigen/Dense>
#include <stack>

int optimal_fit(int j, double* OPT, double** E);
double segmented_regression(int N, MatrixXd X, MatrixXd y, double C, double* OPT, double** E, int* opt_segment);
stack<int> reconstruct_solution(int N, double* OPT, double** E, int* opt_segment);

#endif