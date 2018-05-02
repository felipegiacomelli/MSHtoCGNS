#ifndef GRID_DATA_HPP
#define GRID_DATA_HPP

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

struct GridData {
	GridData() = default;

	int dimension;
	int numberOfElements;

	std::vector<std::vector<double>> coordinates;

	std::vector<std::vector<int>> lineConnectivity;
	std::vector<std::vector<int>> triangleConnectivity;
	std::vector<std::vector<int>> quadrangleConnectivity;
	std::vector<std::vector<int>> tetrahedronConnectivity;
	std::vector<std::vector<int>> hexahedronConnectivity;

	std::vector<BoundaryData> boundaries;
	std::vector<RegionData> regions;
};

using GridDataShared = SharedPointer<GridData>;
using GridDataUnique = UniquePointer<GridData>;

#endif