#ifndef __FILE_MEND_GRID_DATA_EXTRACTOR_HPP__
#define __FILE_MEND_GRID_DATA_EXTRACTOR_HPP__

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"

struct GridDataExtractorData {
    std::vector<std::string> regions;
    std::vector<std::string> boundaries;
    std::vector<std::string> wells;
};

class GridDataExtractor {
    public:
        GridDataExtractor(boost::shared_ptr<GridData> original, std::string gridDataExtractorScript);

        GridDataExtractor(boost::shared_ptr<GridData> original, boost::property_tree::ptree propertyTree);

        ~GridDataExtractor() = default;

        boost::shared_ptr<GridData> extract;

    private:
        void initialize();
        void checkGridData();
        void readScript();
        void createExtract();
        void buildElementConnectivities();
        void extractRegions();
        void extractBoundaries();
        void extractWells();
        void extractVertices();
        void fixIndices();

        boost::shared_ptr<GridData> original;
        boost::property_tree::ptree propertyTree;

        std::vector<GridDataExtractorData> gridDataExtractorDatum;
        std::vector<std::vector<int>> elementConnectivities;
        std::set<int> vertices;
        int localIndex = 0;
};

#endif
