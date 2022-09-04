#ifndef POINT_H
#define POINT_H

#include <vector>
using namespace std;

class Point {

	public:
		Point(int n);
		vector<double> coordinates;

		Point operator[](int i){
			return this->coordinates[i];
		}

		Point operator+(Point other) {

			Point res(this->coordinates.size());
			for (int i = 0; (unsigned int) i < this->coordinates.size(); i++)
				res[i] = this->coordinates[i] + other.coordinates[i];

			return res;
		}

		Point operator-(Point other) {

			Point res(this->coordinates.size());
			for (int i = 0; (unsigned int) i < this->coordinates.size(); i++)
				res[i] = this->coordinates[i] - other.coordinates[i];

			return res;
		}

		Point operator-() {

			Point res(this->coordinates.size());
			for (int i = 0; (unsigned int) i < this->coordinates.size(); i++)
				res[i] = -this->coordinates[i];

			return res;
		}

		Point operator*(Point other) {

			Point res(this->coordinates.size());
			for (int i = 0; (unsigned int) i < this->coordinates.size(); i++)
				res[i] = this->coordinates[i] * other.coordinates[i];

			return res;
		}

		Point operator*(double other) {

			Point res(this->coordinates.size());
			for (int i = 0; (unsigned int) i < this->coordinates.size(); i++)
				res[i] = this->coordinates[i] * other;

			return res;
		}

};

double inner_prod(Point p1, Point p2);

#endif