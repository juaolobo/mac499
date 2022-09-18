#ifndef READ_H
#define READ_H

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <eigen3/Eigen/Dense>

using namespace std;
using Eigen::VectorXd;
using Eigen::MatrixXd;

int read_data(string filename, MatrixXd &X, MatrixXd &y);

#endif