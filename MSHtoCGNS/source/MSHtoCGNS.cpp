#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader2D.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator2D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include "MSHtoCGNS/Utilities/Print.hpp"

boost::shared_ptr<GridData> read(std::string path) {
    char buffer[1024];
    std::ifstream file = std::ifstream(path.c_str());
    file.seekg(0, std::ios::beg);
    while (strcmp(buffer, "$PhysicalNames") && !file.eof())
        file >> buffer;

    if (file.eof())
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no Physical Entities data in the grid file");

    int numberOfPhysicalEntities;
    file >> numberOfPhysicalEntities;
    std::vector<int> dimensions;
    for (int i = 0; i < numberOfPhysicalEntities; ++i) {
        int type;
        int index;
        std::string name;
        file >> type >> index >> name;
        dimensions.push_back(type);
    }

    switch (*std::max_element(dimensions.cbegin(), dimensions.cend())) {
        case 2: {
            MshReader2D mshReader2D(path);
            return mshReader2D.gridData;
        }
        case 3: {
            MshReader3D mshReader3D(path);
            return mshReader3D.gridData;
        }
    }

    return nullptr;
}

std::string create(boost::shared_ptr<GridData> gridData, std::string path) {
    switch (gridData->dimension) {
        case 2: {
            CgnsCreator2D cgnsCreator2D(gridData, path);
            return cgnsCreator2D.getFileName();
        }
        case 3: {
            CgnsCreator3D cgnsCreator3D(gridData, path);
            return cgnsCreator3D.getFileName();
        }
    }

    return std::string();
}

int main() {
    boost::property_tree::ptree propertyTree;
    boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMSHtoCGNS.json", propertyTree);

    std::vector<int> indices;
    for (auto index : propertyTree.get_child("indices"))
        indices.emplace_back(index.second.get_value<int>());

    std::vector<std::string> inputs;
    for (auto input : propertyTree.get_child("inputs"))
        inputs.emplace_back(input.second.data());

    std::string output = propertyTree.get<std::string>("output");

    for (int index : indices) {
        auto start = std::chrono::steady_clock::now();
        boost::shared_ptr<GridData> gridData(read(inputs[index]));
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = end - start;
        std::cout << std::endl << "\tGrid path: " << inputs[index];
        std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

        start = std::chrono::steady_clock::now();
        std::string path = create(gridData, output);
        end = std::chrono::steady_clock::now();
        elapsedSeconds = end - start;
        std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
        std::cout << std::endl << "\tOutput file location       : " << path << std::endl << std::endl;
    }

    return 0;
}
