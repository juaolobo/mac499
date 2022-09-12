#ifndef SEG_REG_H
#define SEG_REG_H
#include <eigen3/Eigen/Dense>

int optimal_fit(int j);
double calculate_regression(MatrixXd X, MatrixXd y, int C);

#endif