#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <fstream>

#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"

void output(boost::property_tree::ptree propertyTree, std::string&& path);

#endif
