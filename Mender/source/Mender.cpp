#include <chrono>

#include "MSHtoCGNS/Utilities/Output.hpp"
#include "MSHtoCGNS/Utilities/Print.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"
#include "MSHtoCGNS/FileMend/CgnsReader/SpecialCgnsReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include "MSHtoCGNS/FileMend/WellGenerator.hpp"
#include "MSHtoCGNS/FileMend/GridDataExtractor.hpp"
#include "MSHtoCGNS/FileMend/RadialGridDataReordered.hpp"
#include "MSHtoCGNS/FileMend/MultipleBasesCgnsCreator3D.hpp"

void createSingleRegion(boost::shared_ptr<GridData> gridData, std::string regionName) {
    RegionData region;
    region.name = regionName;
    region.begin = 0;
    region.end = gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size();
    gridData->regions.clear();
    gridData->regions.emplace_back(std::move(region));
}

void applyRatio(boost::shared_ptr<GridData> gridData, const double& ratio) {
    for (auto& coordinate: gridData->coordinates)
        for (auto& position : coordinate)
            position *= ratio;
}

int main() {
    boost::property_tree::ptree menderScript;
    boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMender.json", menderScript);
    std::string inputPath  = menderScript.get<std::string>("path.input");
    std::string outputPath = menderScript.get<std::string>("path.output");

    auto start = std::chrono::steady_clock::now();
    SpecialCgnsReader3D reader(inputPath);
    boost::shared_ptr<GridData> gridData = reader.gridData;
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;
    std::cout << std::endl << "\tGrid path: " << inputPath;
    std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

    printGridDataInformation(gridData, red("original gridData"));

    if (menderScript.get_child_optional("ScriptWellGenerator"))
        WellGenerator wellGenerator(gridData, menderScript.get_child("ScriptWellGenerator"));

    printGridDataInformation(gridData, red("gridData after well generation"));

    boost::shared_ptr<GridData> radialGridData;
    if (menderScript.get_child_optional("ScriptGridDataExtractor")) {
        GridDataExtractor gridDataExtractor(gridData, menderScript.get_child("ScriptGridDataExtractor"));
        radialGridData = gridDataExtractor.extract;
    }

    printGridDataInformation(radialGridData, red("radial gridData"));

    RadialGridDataReordered radialGridDataReordered(radialGridData);
    radialGridData = radialGridData;

    start = std::chrono::steady_clock::now();
    MultipleBasesCgnsCreator3D creator({gridData, radialGridData}, {"Reservoir", "Well"}, outputPath);
    end = std::chrono::steady_clock::now();
    elapsedSeconds = end - start;
    std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
    std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

    return 0;
}
