#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"
#include "MSHtoCGNS/MshInterface/MshReader.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"

int main(int argc, char** argv) {
    boost::property_tree::ptree propertyTree;
    if (argc > 1) {
        boost::property_tree::ptree inputs;
        boost::property_tree::ptree input;
        input.put("", boost::filesystem::absolute(argv[1]).string());
        inputs.push_back(std::make_pair("", input));
        propertyTree.add_child("inputs", inputs);
        if (argc > 2) {
            propertyTree.put("output", argv[2]);
        }
        else {
            propertyTree.put("output", std::getenv("PWD"));
        }
    }
    else {
        boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "MSHtoCGNS.json", propertyTree);
    }

    std::vector<std::string> inputs;
    for (auto input : propertyTree.get_child("inputs")) {
        auto start = std::chrono::steady_clock::now();
        MshReader mshReader(input.second.data());
        boost::shared_ptr<GridData> gridData(mshReader.gridData);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = end - start;
        std::cout << boost::str(boost::format("\n\tGrid path: %s") % input.second.data());
        std::cout << boost::str(boost::format("\n\tRead in  : %.6f s\n") % elapsedSeconds.count());

        start = std::chrono::steady_clock::now();
        CgnsCreator cgnsCreator(gridData, propertyTree.get<std::string>("output"));
        end = std::chrono::steady_clock::now();
        elapsedSeconds = end - start;
        std::cout << boost::str(boost::format("\n\tConverted to CGNS format in: %.6f s") % elapsedSeconds.count());
        std::cout << boost::str(boost::format("\n\tOutput file location       : %s\n\n") % cgnsCreator.getFileName());
    }

    return 0;
}
