#ifndef GRID_GRID_DATA_HPP
#define GRID_GRID_DATA_HPP

#include <vector>
#include <array>
#include <string>
#include "MSHtoCGNS/BoostInterface/SharedPointer.hpp"

struct RegionData {
    std::string name;
    int begin;
    int end;
    std::vector<int> vertices;
};

struct BoundaryData {
    std::string name;
    int begin;
    int end;
    std::vector<int> vertices;
};

struct WellData {
    std::string name;
    int begin;
    int end;
    std::vector<int> vertices;
};

struct GridData {
    int dimension;

    std::vector<std::array<double, 3>> coordinates;

    std::vector<std::array<int, 3>> lineConnectivity;
    std::vector<std::array<int, 4>> triangleConnectivity;
    std::vector<std::array<int, 5>> quadrangleConnectivity;
    std::vector<std::array<int, 5>> tetrahedronConnectivity;
    std::vector<std::array<int, 9>> hexahedronConnectivity;
    std::vector<std::array<int, 7>> prismConnectivity;
    std::vector<std::array<int, 6>> pyramidConnectivity;

    std::vector<BoundaryData> boundaries;
    std::vector<RegionData> regions;
    std::vector<WellData> wells;
};

#endif
