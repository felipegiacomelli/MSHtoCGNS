#ifndef RADIAL_GRID_DATA_REORDERED
#define RADIAL_GRID_DATA_REORDERED

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>

template<class InputIt>
bool hasElement(InputIt cbegin, InputIt cend, const int& element) {
	return std::find(cbegin, cend, element) != cend;
}

class RadialGridDataReordered {
	public:
		RadialGridDataReordered(GridDataShared gridData);

		~RadialGridDataReordered();

		GridDataShared final;

		double tolerance = 1e-4;

	private:
		void checkGridData();
		void defineQuantities();
		void copyData();
		void reorder();
		void addVertices();
		void fixIndices();
		void copyVertices();
		void copyRegions();
		void copyWells();

		// void buildElementConnectivities();
		// void extractRegions();
		// void extractBoundaries();
		// void extractWells();
		// void extractVertices();

		GridDataShared gridData;

		int numberOfSegments;
		int numberOfPrismsPerSegment;
		int numberOfHexahedronsPerSegment;
		int numberOfHexahedronsPerRadius;

		std::vector<std::pair<int, std::array<double, 3>>> coordinates;
		std::vector<std::array<int, 9>> hexahedra;
		std::vector<std::array<int, 7>> prisms;

		std::vector<int> vertices;

		int vertexShift = 0;
		int elementShift = 0;
};

#endif