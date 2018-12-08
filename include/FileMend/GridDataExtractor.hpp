#ifndef GRID_DATA_EXTRACTOR_HPP
#define GRID_DATA_EXTRACTOR_HPP

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include "BoostInterface/Filesystem.hpp"
#include "BoostInterface/PropertyTree.hpp"
#include "Grid/GridData.hpp"

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
        void checkGridData();
        void readScript();
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
