#include <chrono>

#include <Utilities/Output.hpp>
#include <Utilities/Print.hpp>
#include <Grid/GridData.hpp>
#include <FileMend/CgnsReader/MultipleBasesCgnsReader.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator.hpp>
#include <FileMend/WellGenerator.hpp>
#include <FileMend/GridDataExtractor.hpp>
#include <FileMend/RadialGridDataReordered.hpp>
#include <FileMend/MultipleBasesCgnsCreator.hpp>

void createSingleRegion(boost::shared_ptr<GridData> gridData, std::string regionName) {
    RegionData region;
    region.name = regionName;
    region.elementBegin = 0;
    region.elementEnd = gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size();
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
    MultipleBasesCgnsReader reader(inputPath);
    boost::shared_ptr<GridData> gridData = reader.gridData;
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;
    std::cout << std::endl << "\tGrid path: " << inputPath;
    std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

    printGridDataInformation(gridData, "\033[1;31m original gridData \033[0m");

    if (menderScript.get_child_optional("ScriptWellGenerator")) {
        // boost::property_tree::ptree propertyTree;
        // std::string regionName("WELL_BODY");

        // boost::property_tree::ptree wellStart;
        // boost::property_tree::ptree x, y, z;
        // x.put("",  20.0); wellStart.push_back(std::make_pair("", x));
        // y.put("", 125.0); wellStart.push_back(std::make_pair("", y));
        // z.put("",  50.0); wellStart.push_back(std::make_pair("", z));

        // std::string wellName("LINE_WELL");

        // boost::property_tree::ptree region;
        // region.put("regionName", regionName);
        // region.add_child("wellStart", wellStart);
        // region.put("wellName", wellName);

        // boost::property_tree::ptree wellRegions;
        // wellRegions.push_back(std::make_pair("", region));

        // propertyTree.add_child("wellRegions", wellRegions);

        // WellGenerator wellGenerator(gridData, propertyTree);

        WellGenerator wellGenerator(gridData, menderScript.get_child("ScriptWellGenerator"));
    }

    printGridDataInformation(gridData, "\033[1;31m gridData after well generation \033[0m");

    boost::shared_ptr<GridData> radialGridData;
    if (menderScript.get_child_optional("ScriptGridDataExtractor")) {
        // boost::property_tree::ptree propertyTree;

        // boost::property_tree::ptree sections;

        // std::vector<std::string> names{"WELL_BODY"};
        // for (auto name : names) {
        //  boost::property_tree::ptree section;
        //  section.put("", name);
        //  sections.push_back(std::make_pair("", section));
        // }
        // propertyTree.add_child("regions", sections);

        // sections.clear();

        // names = std::vector<std::string>{};
        // for (auto name : names) {
        //  boost::property_tree::ptree section;
        //  section.put("", name);
        //  sections.push_back(std::make_pair("", section));
        // }
        // propertyTree.add_child("boundaries", sections);

        // sections.clear();

        // names = std::vector<std::string>{"LINE_WELL"};
        // for (auto name : names) {
        //  boost::property_tree::ptree section;
        //  section.put("", name);
        //  sections.push_back(std::make_pair("", section));
        // }
        // propertyTree.add_child("wells", sections);

        // GridDataExtractor gridDataExtractor(gridData, propertyTree);
        // radialGridData = gridDataExtractor.extract;

        GridDataExtractor gridDataExtractor(gridData, menderScript.get_child("ScriptGridDataExtractor"));
        radialGridData = gridDataExtractor.extract;
    }

    printGridDataInformation(radialGridData, "\033[1;31m radial gridData \033[0m");

    RadialGridDataReordered radialGridDataReordered(radialGridData);
    radialGridData = radialGridData;

    start = std::chrono::steady_clock::now();
    MultipleBasesCgnsCreator creator({gridData, radialGridData}, {"Reservoir", "Well"}, outputPath);
    end = std::chrono::steady_clock::now();
    elapsedSeconds = end - start;
    std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
    std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

    return 0;
}
