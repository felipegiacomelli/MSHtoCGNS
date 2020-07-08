#include <chrono>

#include "MSHtoCGNS/Utilities/Print.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"
#include "MSHtoCGNS/Manipulation/CgnsReader/SpecialCgnsReader.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"
#include "MSHtoCGNS/Manipulation/Generator/WellGenerator.hpp"
#include "MSHtoCGNS/Manipulation/GridDataExtractor.hpp"
#include "MSHtoCGNS/Manipulation/RadialGridDataReordered.hpp"
#include "MSHtoCGNS/Manipulation/SegmentExtractor.hpp"
#include "MSHtoCGNS/Manipulation/MultipleBasesCgnsCreator.hpp"

void unifyRegions(boost::shared_ptr<GridData> gridData, std::string name) {
    auto begin = std::find_if(gridData->sections.cbegin(), gridData->sections.cend(), [](const auto& e){return e.dimension == 3;})->begin;
    auto end = std::find_if(gridData->sections.cbegin(), gridData->sections.cend(), [](const auto& e){return e.dimension == 3;})->end;
    for (auto section : gridData->sections)
        if (section.dimension == 3)
            if (section.end > end)
                end = section.end;
    gridData->sections.erase(std::remove_if(gridData->sections.begin(), gridData->sections.end(), [](const auto& e){return e.dimension == 3;}), gridData->sections.end());
    gridData->sections.insert(gridData->sections.begin(), SectionData{name, 3, begin, end, std::vector<int>{}});
}

void applyRatio(boost::shared_ptr<GridData> gridData, double ratio) {
    for (auto& coordinate: gridData->coordinates)
        for (auto& position : coordinate)
            position *= ratio;
}

int main() {
    boost::property_tree::ptree script;
    boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "Manipulator.json", script);
    std::string inputPath  = script.get<std::string>("path.input");
    std::string outputPath = script.get<std::string>("path.output");

    auto start = std::chrono::steady_clock::now();
    SpecialCgnsReader specialCgnsReader(inputPath);
    boost::shared_ptr<GridData> gridData = specialCgnsReader.gridData;
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;
    std::cout << std::endl << "\tGrid path: " << inputPath;
    std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

    printGridDataInformation(gridData, purple("original gridData"));

    if (script.get_child_optional("ScriptWellGenerator"))
        WellGenerator wellGenerator(gridData, script.get_child("ScriptWellGenerator"));

    boost::shared_ptr<GridData> radialGridData, segmentGridData;
    if (script.get_child_optional("ScriptGridDataExtractor") && script.get<bool>("extract")) {
        GridDataExtractor gridDataExtractor(gridData, script.get_child("ScriptGridDataExtractor"));
        radialGridData = gridDataExtractor.extract;

        printGridDataInformation(radialGridData, purple("radial gridData"));

        RadialGridDataReordered radialGridDataReordered(radialGridData);

        SegmentExtractor SegmentExtractor(radialGridData);
        segmentGridData = SegmentExtractor.segment;
    }

    std::vector<boost::shared_ptr<GridData>> gridDatas{gridData};
    std::vector<std::string> baseNames{"BASE"};

    if (script.get_child_optional("unifyRegions"))
        if (script.get<bool>("unifyRegions"))
            unifyRegions(gridData, "BODY");

    if (radialGridData) {
        gridDatas.emplace_back(radialGridData);
        baseNames.emplace_back("CYLINDRICAL");
    }

    if (segmentGridData) {
        gridDatas.emplace_back(segmentGridData);
        baseNames.emplace_back("SEGMENT");
    }

    printGridDataInformation(gridData, purple("final gridData"));

    start = std::chrono::steady_clock::now();
    MultipleBasesCgnsCreator multipleBasesCgnsCreator(gridDatas, baseNames, baseNames, outputPath);
    end = std::chrono::steady_clock::now();
    elapsedSeconds = end - start;
    std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
    std::cout << std::endl << "\tOutput file location       : " << multipleBasesCgnsCreator.getFileName() << std::endl << std::endl;

    return 0;
}
