#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <BoostInterface/PropertyTree.hpp>
#include <fstream>

void output(boost::property_tree::ptree propertyTree, std::string&& path) {
    std::ofstream outputFile(path);
    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, propertyTree);
    outputFile << ss.str();
    outputFile.close();
}

#endif
