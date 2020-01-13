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
    if (argc == 2) {
       boost::property_tree::ptree inputs;
       boost::property_tree::ptree input;
       input.put("",  argv[1]);
       inputs.push_back(std::make_pair("", input));
       propertyTree.add_child("inputs", inputs);
       propertyTree.put("output", "./");
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
        std::cout << std::endl << "\tGrid path: " << input.second.data();
        std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

        start = std::chrono::steady_clock::now();
        CgnsCreator cgnsCreator(gridData, propertyTree.get<std::string>("output"));
        end = std::chrono::steady_clock::now();
        elapsedSeconds = end - start;
        std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
        std::cout << std::endl << "\tOutput file location       : " << cgnsCreator.getFileName() << std::endl << std::endl;
    }

    return 0;
}
