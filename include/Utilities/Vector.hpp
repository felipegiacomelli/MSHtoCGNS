#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <vector>

template<typename T>
std::vector<T> linearize(const std::vector<std::vector<T>>& a) {
	std::vector<T> b;
	b.reserve(a.size()*a[0].size());
	for (unsigned i = 0; i < a.size(); i++)
		for (unsigned j = 0; j < a[i].size(); j++)
			b.push_back(a[i][j]);
	return b;
}

#endif