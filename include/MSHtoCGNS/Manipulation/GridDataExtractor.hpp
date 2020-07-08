#ifndef __MANIPULATION_GRID_DATA_EXTRACTOR_HPP__
#define __MANIPULATION_GRID_DATA_EXTRACTOR_HPP__

#include <set>
#include <algorithm>
#include <numeric>

#include <boost/unordered_map.hpp>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

struct GridDataExtractorData {
    std::vector<std::string> sections;
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
        void extractEntities();
        void extractVertices();
        void rectifyConnectivities();

        boost::shared_ptr<GridData> original;
        boost::property_tree::ptree propertyTree;

        std::vector<GridDataExtractorData> gridDataExtractorDatum;
        std::set<int> vertices;
        int localIndex = 0;
};

#endif
