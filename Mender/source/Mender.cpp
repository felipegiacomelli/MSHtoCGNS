#include <chrono>

#include "MSHtoCGNS/Utilities/Print.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"
#include "MSHtoCGNS/FileMend/CgnsReader/SpecialCgnsReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include "MSHtoCGNS/FileMend/WellGenerator.hpp"
#include "MSHtoCGNS/FileMend/GridDataExtractor.hpp"
#include "MSHtoCGNS/FileMend/RadialGridDataReordered.hpp"
#include "MSHtoCGNS/FileMend/SegmentGridExtractor.hpp"
#include "MSHtoCGNS/FileMend/CgnsCreator/MultipleBasesCgnsCreator3D.hpp"

void unifyRegions(boost::shared_ptr<GridData> gridData, std::string name) {
    RegionData region;
    region.name = name;
    region.begin = 0;
    region.end = gridData->tetrahedrons.size() + gridData->hexahedrons.size() + gridData->prisms.size() + gridData->pyramids.size();
    gridData->regions.clear();
    gridData->regions.emplace_back(std::move(region));
}

void applyRatio(boost::shared_ptr<GridData> gridData, double ratio) {
    for (auto& coordinate: gridData->coordinates)
        for (auto& position : coordinate)
            position *= ratio;
}

int main() {
    boost::property_tree::ptree menderScript;
    boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "Mender.json", menderScript);
    std::string inputPath  = menderScript.get<std::string>("path.input");
    std::string outputPath = menderScript.get<std::string>("path.output");

    auto start = std::chrono::steady_clock::now();
    SpecialCgnsReader3D specialCgnsReader3D(inputPath);
    boost::shared_ptr<GridData> gridData = specialCgnsReader3D.gridData;
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;
    std::cout << std::endl << "\tGrid path: " << inputPath;
    std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

    printGridDataInformation(gridData, purple("original gridData"));

    if (menderScript.get_child_optional("ScriptWellGenerator"))
        WellGenerator wellGenerator(gridData, menderScript.get_child("ScriptWellGenerator"));

    printGridDataInformation(gridData, purple("gridData after well generation"));

    boost::shared_ptr<GridData> radialGridData, segmentGridData;
    if (menderScript.get_child_optional("ScriptGridDataExtractor") && menderScript.get<bool>("extract")) {
        GridDataExtractor gridDataExtractor(gridData, menderScript.get_child("ScriptGridDataExtractor"));
        radialGridData = gridDataExtractor.extract;

        printGridDataInformation(radialGridData, purple("radial gridData"));

        RadialGridDataReordered radialGridDataReordered(radialGridData);

        SegmentGridExtractor segmentGridExtractor(radialGridData);
        segmentGridData = segmentGridExtractor.segment;
    }

    std::vector<boost::shared_ptr<GridData>> gridDatas{gridData};
    std::vector<std::string> baseNames{"BASE"};

    if (menderScript.get_child_optional("unifyRegions"))
        if (menderScript.get<bool>("unifyRegions"))
            unifyRegions(gridData, "BODY");

    if (radialGridData) {
        gridDatas.emplace_back(radialGridData);
        baseNames.emplace_back("CYLINDRICAL");
    }

    if (segmentGridData)
    {
        gridDatas.emplace_back(segmentGridData);
        baseNames.emplace_back("SEGMENT");
    }

    printGridDataInformation(gridData, purple("final gridData"));

    start = std::chrono::steady_clock::now();
    MultipleBasesCgnsCreator3D multipleBasesCgnsCreator3D(gridDatas, baseNames, baseNames, outputPath);
    end = std::chrono::steady_clock::now();
    elapsedSeconds = end - start;
    std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
    std::cout << std::endl << "\tOutput file location       : " << multipleBasesCgnsCreator3D.getFileName() << std::endl << std::endl;

    return 0;
}
