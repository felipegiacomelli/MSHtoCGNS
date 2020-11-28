#ifndef __MANIPULATION_CARTESIAN_WELL_GENERATOR_HPP__
#define __MANIPULATION_CARTESIAN_WELL_GENERATOR_HPP__

#include <set>
#include <algorithm>
#include <numeric>
#include <cmath>

#include <boost/unordered_map.hpp>

#include "MSHtoCGNS/BoostInterface/Geometry.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/Manipulation/Generator.hpp"

class CartesianWellGenerator : public Generator {
    public:
        CartesianWellGenerator(boost::shared_ptr<GridData> gridData, std::string scriptPath);
        CartesianWellGenerator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree script);

        ~CartesianWellGenerator() = default;

        double tolerance = 1.0e-3;

    private:
        void checkGridData();
        void generateWells();
};

#endif
