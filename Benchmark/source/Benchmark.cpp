#include <chrono>
#include <iostream>
#include <iomanip>
#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"

int main() {
    boost::property_tree::ptree propertyTree;
    boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "../Benchmark/Script.json", propertyTree);

    std::vector<std::string> inputs;
    for (auto input : propertyTree.get_child("inputs")) {
        auto start = std::chrono::steady_clock::now();
        CgnsReader3D cgnsReader3D(input.second.data());
        boost::shared_ptr<GridData> gridData = cgnsReader3D.gridData;
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = end - start;
        std::cout << std::endl << "\tGrid path: " << input.second.data();
        std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

        start = std::chrono::steady_clock::now();
        CgnsCreator3D cgnsCreator3D(gridData, propertyTree.get<std::string>("output"));
        end = std::chrono::steady_clock::now();
        elapsedSeconds = end - start;
        std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
        std::cout << std::endl << "\tOutput file location       : " << cgnsCreator3D.getFileName() << std::endl << std::endl;
    }

    return 0;
}
