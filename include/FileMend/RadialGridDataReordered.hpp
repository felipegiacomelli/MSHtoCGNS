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

template<class InputIt>
bool hasElements(InputIt cbegin, InputIt cend, const std::array<int, 4>& facetConnectivity) {
	bool close = true;

	for (const int& vertex : facetConnectivity)
		close &= hasElement(cbegin, cend, vertex);

	return close;
}

class RadialGridDataReordered {
	public:
		RadialGridDataReordered(GridDataShared gridData);

		~RadialGridDataReordered() = default;

		GridDataShared final;

		double tolerance = 1e-4;

	private:
		void checkGridData();
		void defineQuantities();
		void copyData();
		void reorder();
		void copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron);
		void copyPrism(std::vector<std::array<int, 7>>::iterator prism);
		void addVertices();
		void findVerticesOnPlane(const std::array<double, 3>& normal, const double& d);
		void addVertex(std::vector<std::pair<int, std::array<double, 3>>>::iterator vertex);
		void fixIndices();
		void copyVertices();
		void copyRegions();
		void copyWells();

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