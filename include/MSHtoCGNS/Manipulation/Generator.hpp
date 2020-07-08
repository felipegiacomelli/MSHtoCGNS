#ifndef __MANIPULATION_GENERATOR_HPP_HPP__
#define __MANIPULATION_GENERATOR_HPP_HPP__

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

class Generator {
    public:
        Generator(boost::shared_ptr<GridData> gridData, std::string scriptPath);
        Generator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree script);

        ~Generator() = default;

    protected:
        boost::shared_ptr<GridData> gridData;
        boost::property_tree::ptree script;
};

#endif
