#include <MSHtoCGNS/Utilities/Output.hpp>

void output(boost::property_tree::ptree propertyTree, std::string&& path) {
    std::ofstream outputFile(path);
    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, propertyTree);
    outputFile << ss.str();
    outputFile.close();
}
