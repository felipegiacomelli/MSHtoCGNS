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

void output(boost::shared_ptr<GridData> gridData, std::string fileName);

#endif
