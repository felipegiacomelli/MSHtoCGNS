#ifndef __GRID_GRID_DATA_HPP__
#define __GRID_GRID_DATA_HPP__

#include <vector>
#include <array>
#include <string>

#include "MSHtoCGNS/BoostInterface/SharedPointer.hpp"

struct SectionData {
    SectionData() = default;

    SectionData(std::string name, int dimension, int begin, int end, std::vector<int> vertices = std::vector<int>{}) :
        name(name), dimension(dimension), begin(begin), end(end), vertices(vertices) {}

    inline bool operator==(const SectionData& other) {
        return (
            this->name == other.name &&
            this->dimension == other.dimension &&
            this->begin == other.begin &&
            this->end == other.end
        );
    }

    std::string name;
    int dimension;
    int begin;
    int end;
    std::vector<int> vertices;
};

struct GridData {
    GridData(int dimension = 0) :
        dimension(dimension) {}

    int dimension;

    std::vector<std::array<double, 3>> coordinates;
    std::vector<std::vector<int>> connectivities;
    std::vector<SectionData> sections;

    int numberOfLocalVertices;
};

using GridDataPtr = boost::shared_ptr<GridData>;

#endif
