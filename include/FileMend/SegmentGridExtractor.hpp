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
		void copyVertices();
		void copyElements();
		void copyFacets();
		void copyLine();
		void fixBoundaries();
		// void fixRegion();
		// void fixWell();

		boost::shared_ptr<GridData> gridData;

		int numberOfSegments;
		int numberOfPrismsPerSegment;
		int numberOfHexahedronsPerSegment;
		int numberOfHexahedronsPerRadius;
		int numberOfVerticesPerSection;

		int vertexShift = 0;
		int elementShift = 0;
};

#endif