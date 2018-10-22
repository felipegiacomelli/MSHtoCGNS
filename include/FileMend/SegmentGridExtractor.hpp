#ifndef SEGMENT_GRID_EXTRACTOR_HPP
#define SEGMENT_GRID_EXTRACTOR_HPP

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Algorithm.hpp>
#include <Grid/GridData.hpp>

class SegmentGridExtractor {
	public:
		SegmentGridExtractor(boost::shared_ptr<GridData> gridData);

		~SegmentGridExtractor() = default;

		boost::shared_ptr<GridData> segmentGrid;

		double tolerance = 1e-4;

	private:
		void checkGridData();
		void defineQuantities();
		void createSegmentGrid();
		void copyElements();
		void copyFacets();
		// void buildFirstSection();
		// void addVertex(int vertex, int section);
		// void updateTriangle(std::vector<std::array<int, 7>>::iterator prism, std::vector<std::array<int, 4>>::iterator triangle);
		// void updateQuadrangle(std::vector<std::array<int, 9>>::iterator hexahedron, std::vector<std::array<int, 5>>::iterator quadrangle);
		// void copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron);
		// void copyPrism(std::vector<std::array<int, 7>>::iterator prism);
		// void copyVertices();
		// void fixBoundaries();
		// void fixRegion();
		// void fixWell();

		boost::shared_ptr<GridData> gridData;

		int numberOfSegments;
		int numberOfPrismsPerSegment;
		int numberOfHexahedronsPerSegment;
		int numberOfHexahedronsPerRadius;
		int numberOfVerticesPerSection;

		std::vector<std::array<int, 9>> hexahedra;
		std::vector<std::array<int, 7>> prisms;
		std::vector<std::array<int, 4>> triangles;
		std::vector<std::array<int, 5>> quadrangles;

		std::vector<std::pair<int, int>> vertices;

		int vertexShift = 0;
		int elementShift = 0;
		int facetShift;
		int lineShift;
};

#endif