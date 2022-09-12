#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <eigen3/Eigen/Dense>
#include "lsfit.hpp"
#include "read_data.hpp"
#include "constants.hpp"

using namespace std;
using Eigen::MatrixXd;

int main(){
	
	MatrixXd a, b;

	read_data("regression.tsv", a, b);

	return 0;
}