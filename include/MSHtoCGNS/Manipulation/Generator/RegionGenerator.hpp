#ifndef __MANIPULATION_REGION_GENERATOR_HPP_HPP__
#define __MANIPULATION_REGION_GENERATOR_HPP_HPP__

#include <algorithm>

#include "MSHtoCGNS/Manipulation/Generator.hpp"
#include "MSHtoCGNS/BoostInterface/String.hpp"
#include "MSHtoCGNS/BoostInterface/Geometry.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/Utilities/Algorithm.hpp"
#include "MSHtoCGNS/Utilities/Error.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

class RegionGenerator : public Generator {
    public:
        RegionGenerator(boost::shared_ptr<GridData> gridData, std::string scriptPath);
        RegionGenerator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree script);

        ~RegionGenerator() = default;

    private:
        void generateCoordinates();
        void generateRegions();
        bool isElementInRange(std::vector<std::vector<int>>::iterator connectivity, const Box& box);

        std::vector<Point> coordinates;

        int marker = -1;
};

#endif
