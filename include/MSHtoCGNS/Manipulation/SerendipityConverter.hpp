#ifndef __MANIPULATION_SERENDIPITY_CONVERTER_HPP__
#define __MANIPULATION_SERENDIPITY_CONVERTER_HPP__

#include <set>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/Utilities/Error.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

class SerendipityConverter {
    public:
        SerendipityConverter(boost::shared_ptr<GridData> gridData);

        ~SerendipityConverter() = default;

        boost::shared_ptr<GridData> serendipity;

    protected:
        void convert();
        void setCoordinates();
        void doStuff();
        void convertToSerendipity(std::vector<int>& connectivity, std::vector<int> remove);


        boost::shared_ptr<GridData> gridData;

        std::set<int> verticesToRemove;
        std::vector<std::pair<int, std::array<double, 3>>> coordinates;
};

#endif
