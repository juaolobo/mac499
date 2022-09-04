#ifndef SLICE_H
#define SLICE_H

#include <vector>

using namespace std;
template <typename T>

vector<T> slice(vector<T> const& v, int beg, int end) {
	auto i = v.begin() + beg;
	auto j = v.begin() + end + 1;

	vector<T> sliced(i, j);

	return sliced;
}


#endif