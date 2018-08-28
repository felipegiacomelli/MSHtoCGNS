#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <Grid/GridData.hpp>

template<class InputIt>
void output(InputIt cbegin, InputIt cend, std::ofstream& file) {
	for (auto i = cbegin; i != cend; i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++)
			file << "\t" << std::setw(3) << std::right << *j;
		file << std::endl;
	}
}

template<typename T>
void print(const std::array<T, 3>& coordinate) {
	for (int i = 0; i < 3; i++)
		std::cout << "\t" << coordinate[i];
}

void output(GridDataShared gridData, std::string fileName);

void printGridDataInformation(GridDataShared gridData);

#endif