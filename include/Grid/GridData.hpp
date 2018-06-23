#ifndef GRID_GRID_DATA_HPP
#define GRID_GRID_DATA_HPP

#include <vector>
#include <array>
#include <string>
#include <BoostInterface/UniquePointer.hpp>
#include <BoostInterface/SharedPointer.hpp>

struct BoundaryData {
	std::vector<int> facetsOnBoundary;
	std::vector<int> vertices;
	std::string	name;
};

struct RegionData {
	std::vector<int> elementsOnRegion;
	std::string	name;
};

struct WellData {
	std::vector<int> elementsOnWell;
	std::vector<int> vertices;
	std::string	name;
};

struct GridData {
	GridData() = default;

	int dimension;
	int numberOfElements;

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

using GridDataShared = SharedPointer<GridData>;
using GridDataUnique = UniquePointer<GridData>;

#endif