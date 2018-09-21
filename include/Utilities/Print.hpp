#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

template<typename T>
void print(const std::vector<T>& a, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl << "\t";
	for (auto i = a.cbegin(); i != a.cend(); i++)
		std::cout << "\t" << *i;
	std::cout << std::endl;
}

template<typename T>
void print(const std::vector<std::vector<T>>& a, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl << std::endl;
	for (auto i = a.cbegin(); i != a.cend(); i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++) {
			std::cout << "\t" << *j;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template<typename InputIt>
void print(InputIt cbegin, InputIt cend, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl << std::endl;
	for (auto i = cbegin; i != cend; i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++) {
			std::cout << "\t" << std::setw(3) << std::right <<*j;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

#endif