#ifndef __MANIPULATION_WELL_GENERATOR_HPP_HPP__
#define __MANIPULATION_WELL_GENERATOR_HPP_HPP__

#include <set>
#include <algorithm>
#include <numeric>
#include <cmath>

#include <boost/unordered_map.hpp>

#include "MSHtoCGNS/BoostInterface/Geometry.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/Manipulation/Generator.hpp"

class WellGenerator : public Generator {
    public:
        WellGenerator(boost::shared_ptr<GridData> gridData, std::string scriptPath);
        WellGenerator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree script);

        ~WellGenerator() = default;

        double tolerance = 1.0e-3;

    private:
        void checkGridData();
        void generateWells();
        void defineQuantities();

        int linesShift;

        int currentIndex = -1;
        std::vector<std::vector<int>> prisms;
        int numberOfElementsPerSection;
        int numberOfSegments;
        int numberOfPrisms;
};

#endif
