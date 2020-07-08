#ifndef __UTILITIES_PRINT_HPP__
#define __UTILITIES_PRINT_HPP__

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "MSHtoCGNS/GridData/GridData.hpp"

template<typename InputIt>
void print1D(InputIt cbegin, InputIt cend, std::string&& message) {
    std::cout << message;
    while (cbegin != cend)
        std::cout << "\t" << std::setw(3) << std::right << *cbegin++;
    std::cout << std::endl;
}

template<typename InputIt>
void print2D(InputIt cbegin, InputIt cend, std::string&& message = "", std::string&& separator = "") {
    std::cout << message;
    for (auto i = cbegin; i != cend; ++i) {
        for (auto j = i->cbegin(); j != i->cend(); ++j) {
            std::cout << separator << std::setw(3) << std::right << *j;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::string red(std::string message);

std::string yellow(std::string message);

std::string purple(std::string message);

std::string cyan(std::string message);

void printGridDataInformation(boost::shared_ptr<GridData> gD, std::string message = "");

#endif
