#ifndef __MANIPULATION_SEGMENT_EXTRACTOR_HPP__
#define __MANIPULATION_SEGMENT_EXTRACTOR_HPP__

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/Utilities/Algorithm.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

class SegmentExtractor {
    public:
        SegmentExtractor(boost::shared_ptr<GridData> gridData);

        ~SegmentExtractor() = default;

        boost::shared_ptr<GridData> segment;

        double tolerance = 1e-4;

    private:
        void checkGridData();
        void defineQuantities();
        void createSegmentGrid();
        void copyVertices();
        void copyElements();
        void findVertices(std::vector<SectionData>& sections );

        boost::shared_ptr<GridData> gridData;

        int numberOfSegments;
        int numberOfPrismsPerSegment;
        int numberOfHexahedronsPerSegment;
        int numberOfHexahedronsPerRadius;
        int numberOfVerticesPerSection;
};

#endif
