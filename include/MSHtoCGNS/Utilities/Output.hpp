#ifndef __UTILITIES_OUTPUT_HPP__
#define __UTILITIES_OUTPUT_HPP__

#include <fstream>

#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"

void output(boost::property_tree::ptree propertyTree, std::string&& path);

#endif
