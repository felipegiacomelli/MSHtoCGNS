#ifndef __MANIPULATION_WELL_GENERATOR_HPP_HPP__
#define __MANIPULATION_WELL_GENERATOR_HPP_HPP__

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <cmath>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

struct WellGeneratorData {
    std::string regionName;
    std::array<double, 3> wellStart;
    int wellDirection;
    std::string wellName;
};

class WellGenerator {
    public:
        WellGenerator(boost::shared_ptr<GridData> gridData, std::string wellGeneratorScript);

        WellGenerator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree propertyTree);

        ~WellGenerator() = default;

        double tolerance = 1e-3;

    private:
        void checkGridData();
        void readScript();
        void generateWells();
        void defineQuantities();
        bool isClose(const std::array<double, 3>& coordinate, const std::array<double, 3>& referencePoint);

        boost::shared_ptr<GridData> gridData;
        boost::property_tree::ptree propertyTree;

        std::vector<WellGeneratorData> wellGeneratorDatum;
        int linesShift;

        int currentIndex = -1;
        std::vector<std::vector<int>> prisms;
        int numberOfElementsPerSection;
        int numberOfSegments;
        int numberOfPrisms;
};

#endif
