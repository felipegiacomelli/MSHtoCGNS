#ifndef RADIAL_GRID_DATA_REORDERED
#define RADIAL_GRID_DATA_REORDERED

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>

template<class InputIt, class T>
bool hasElement(InputIt cbegin, InputIt cend, T element) {
	return std::find(cbegin, cend, element) != cend;
}

template<class InputIt>
bool hasElements(InputIt setBegin, InputIt setEnd, InputIt elementBegin, InputIt elementEnd) {
	bool close = true;

	for (auto vertex = elementBegin; vertex != elementEnd; vertex++)
		close &= hasElement(setBegin, setEnd, *vertex);

	return close;
}

template<class InputIt>
bool hasElements(InputIt cbegin, InputIt cend, const std::array<int, 4>& facetConnectivity) {
	bool close = true;

	for (int vertex : facetConnectivity)
		close &= hasElement(cbegin, cend, vertex);

	return close;
}

class RadialGridDataReordered {
	public:
		RadialGridDataReordered(GridDataShared gridData);

		~RadialGridDataReordered() = default;

		GridDataShared reordered;

		double tolerance = 1e-4;

	private:
		void checkGridData();
		void defineQuantities();
		void createReordered();
		void copyData();
		void reorder();
		void addVertex(int handle);
		void addVertexAndHandle(std::pair<int, int>&& firstVertexAndHandle, std::pair<int, int>&& secondVertexAndHandle);
		void copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron);
		void copyPrism(std::vector<std::array<int, 7>>::iterator prism);
		void fixIndices();
		void copyVertices();

		GridDataShared gridData;

		int numberOfSegments;
		int numberOfPrismsPerSegment;
		int numberOfHexahedronsPerSegment;
		int numberOfHexahedronsPerRadius;
		int numberOfVerticesPerSection;

		std::vector<std::pair<int, std::array<double, 3>>> coordinates;
		std::vector<std::array<int, 9>> hexahedra;
		std::vector<std::array<int, 7>> prisms;

		std::vector<int> vertices;
		std::vector<std::pair<int, int>> verticesAndHandles;

		int vertexShift = 0;
		int elementShift = 0;
};

#endif