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
		void addVertexAndHandle(std::pair<int, int>&& firstVertexAndHandle, std::pair<int, int>&& secondVertexAndHandle);
		void addVertexAndHandle(std::pair<int, int>&& vertexAndHandle);
		void updateTriangle(std::vector<std::array<int, 7>>::iterator prism, std::vector<std::array<int, 4>>::iterator triangle);
		void updateQuadrangle(std::vector<std::array<int, 9>>::iterator hexahedron, std::vector<std::array<int, 5>>::iterator quadrangle);
		void copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron);
		void copyPrism(std::vector<std::array<int, 7>>::iterator prism);
		void fixVerticesIndices();
		void fixElementIndices();
		void fixFacetIndices();
		void copyVertices();

		GridDataShared gridData;

		int numberOfSegments;
		int numberOfPrismsPerSegment;
		int numberOfHexahedronsPerSegment;
		int numberOfHexahedronsPerRadius;
		int numberOfVerticesPerSection;

		std::vector<std::array<int, 9>> hexahedra;
		std::vector<std::array<int, 7>> prisms;
		std::vector<std::array<int, 4>> triangles;
		std::vector<std::array<int, 5>> quadrangles;

		std::vector<std::pair<int, int>> verticesAndHandles;

		int vertexShift = 0;
		int elementShift = 0;
		int facetShift = 0;
};

#endif