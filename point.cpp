#include "point.hpp"
using namespace std;

Point::Point(int n){

	coordinates = vector<double>(n, 0);
	
}

double inner_prod(Point p1, Point p2) {

	double res = 0;
	for (int i = 0; (unsigned int) i < p1.coordinates.size(); i++)
		res += p1.coordinates[i] * p2.coordinates[i];

	return res;
}
