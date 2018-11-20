#ifndef WELL_GENERATOR_HPP_HPP
#define WELL_GENERATOR_HPP_HPP

#include <set>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>

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
        int lineConnectivityShift;

        int currentIndex = -1;
        std::vector<std::vector<int>> prisms;
        int numberOfElementsPerSection;
        int numberOfSegments;
        int numberOfPrisms;
};

#endif
