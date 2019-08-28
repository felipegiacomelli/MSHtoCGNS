#ifndef __MANIPULATION_GRID_DATA_EXTRACTOR_HPP__
#define __MANIPULATION_GRID_DATA_EXTRACTOR_HPP__

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

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
        void buildGlobalConnectivities();
        void extractRegions();
        void extractBoundaries();
        void extractWells();
        void extractVertices();
        void rectifyConnectivities();
        template<typename T> void rectifyConnectivity(std::unordered_map<int, int> conversionTable, std::vector<T>& connectivities) {
            for (auto& connectivity : connectivities) {
                for (auto index = connectivity.begin(); index != connectivity.end() - 1; ++index)
                    *index = conversionTable[*index];
            }
        }

        boost::shared_ptr<GridData> original;
        boost::property_tree::ptree propertyTree;

        std::vector<GridDataExtractorData> gridDataExtractorDatum;
        std::vector<std::vector<int>> global;
        std::set<int> vertices;
        int localIndex = 0;
};

#endif
