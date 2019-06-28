#ifndef __GRID_GRID_DATA_HPP__
#define __GRID_GRID_DATA_HPP__

#include <vector>
#include <array>
#include <string>

#include "MSHtoCGNS/BoostInterface/SharedPointer.hpp"

struct RegionData {
    RegionData() = default;
    RegionData(std::string name, int begin, int end) : name(name), begin(begin), end(end) {}

    std::string name;
    int begin;
    int end;
    std::vector<int> vertices;
};

struct BoundaryData {
    BoundaryData() = default;
    BoundaryData(std::string name, int begin, int end) : name(name), begin(begin), end(end) {}

    std::string name;
    int begin;
    int end;
    std::vector<int> vertices;
};

struct WellData {
    WellData() = default;
    WellData(std::string name, int begin, int end) : name(name), begin(begin), end(end) {}

    std::string name;
    int begin;
    int end;
    std::vector<int> vertices;
};

struct GridData {
    int dimension;

    std::vector<std::array<double, 3>> coordinates;

    std::vector<std::array<int, 5>> tetrahedrons;
    std::vector<std::array<int, 9>> hexahedrons;
    std::vector<std::array<int, 7>> prisms;
    std::vector<std::array<int, 6>> pyramids;
    std::vector<std::array<int, 4>> triangles;
    std::vector<std::array<int, 5>> quadrangles;
    std::vector<std::array<int, 3>> lines;
    std::vector<std::array<int, 2>> points;

    std::vector<BoundaryData> boundaries;
    std::vector<RegionData> regions;
    std::vector<WellData> wells;

    int numberOfLocalVertices;
    std::vector<int> solverIndices;
};

using GridDataPtr = boost::shared_ptr<GridData>;

#endif
