#ifndef __GRID_GRID_DATA_HPP__
#define __GRID_GRID_DATA_HPP__

#include <vector>
#include <array>
#include <string>

#include "MSHtoCGNS/BoostInterface/SharedPointer.hpp"

struct SectionData {
    SectionData() = default;

    SectionData(std::string name, int dimension, int begin, int end, std::vector<int> vertices) :
        name(name), dimension(dimension), begin(begin), end(end), vertices(vertices) {}

    std::string name;
    int dimension;
    int begin;
    int end;
    std::vector<int> vertices;
};

struct GridData {
    int dimension;

    std::vector<std::array<double, 3>> coordinates;
    std::vector<std::vector<int>> connectivities;
    std::vector<SectionData> sections;

    int numberOfLocalVertices;
};

using GridDataPtr = boost::shared_ptr<GridData>;

#endif
