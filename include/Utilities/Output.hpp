#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

template<typename T>
void output(const std::vector<T>& a, std::ofstream& file) {
	file << std::fixed << std::setprecision(3);
	for (auto i = a.cbegin(); i != a.cend(); i ++) 
		file << "\t" << std::setw(6) << std::right << *i;		
	file << std::endl;
}

template<typename T>
void output(const std::vector<std::vector<T>>& a, std::ofstream& file) {
	file << std::fixed << std::setprecision(3) << std::right;
	for (auto i = a.cbegin(); i != a.cend(); i ++) {
		for (auto j = i->cbegin(); j != i->cend(); j ++) {
			file << "\t" << std::setw(6) << std::right << *j;
		}
		file << std::endl;
	}
}

#endif