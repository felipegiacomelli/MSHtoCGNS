#ifndef IO_OUTPUT_HPP
#define IO_OUTPUT_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <Grid/GridData.hpp>

void outputGridData2D(GridDataShared gridData, std::string fileName);

void outputGridData3D(GridDataShared gridData, std::string fileName);

#endif