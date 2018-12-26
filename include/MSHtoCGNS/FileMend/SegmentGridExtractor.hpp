#ifndef __FILE_MEND_SEGMENT_GRID_EXTRACTOR_HPP__
#define __FILE_MEND_SEGMENT_GRID_EXTRACTOR_HPP__

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/Utilities/Algorithm.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"

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
        void fixRegion();
        void fixBoundaries();
        void fixWell();

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
