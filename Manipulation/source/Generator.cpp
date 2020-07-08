#include "MSHtoCGNS/Manipulation/Generator.hpp"

Generator::Generator(boost::shared_ptr<GridData> gridData, std::string scriptPath) : gridData(gridData) {
    boost::property_tree::read_json(scriptPath, this->script);
}

Generator::Generator(boost::shared_ptr<GridData> gridData, boost::property_tree::ptree script) : gridData(gridData), script(script) {}
