#include <chrono>

#include "MSHtoCGNS/BoostInterface/PropertyTree.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include "MSHtoCGNS/FileMend/GridDataExtractor.hpp"
#include "MSHtoCGNS/FileMend/CgnsCreator/MultipleBasesCgnsCreator3D.hpp"
#include "MSHtoCGNS/Utilities/Print.hpp"

int main() {
    boost::property_tree::ptree script;
    boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMultipleBases.json", script);
    boost::filesystem::path inputPath(script.get<std::string>("path.input"));
    boost::filesystem::path outputPath(script.get<std::string>("path.output"));

    auto start = std::chrono::steady_clock::now();

    boost::shared_ptr<GridData> gridData;
    if (inputPath.extension() == std::string(".msh")) {
        MshReader3D reader(inputPath.string());
        gridData = reader.gridData;
    }
    else if (inputPath.extension() == std::string(".cgns")) {
        CgnsReader3D reader(inputPath.string());
        gridData = reader.gridData;
    }
    else
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - file extension " + inputPath.extension().string() + " not supported");

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsedSeconds = end - start;
    std::cout << std::endl << "\tGrid path: " << inputPath.string();
    std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

    printGridDataInformation(gridData, red("original gridData"));

    boost::shared_ptr<GridData> extract;
    if (script.get_child_optional("ScriptGridDataExtractor")) {
        GridDataExtractor gridDataExtractor(gridData, script.get_child("ScriptGridDataExtractor"));
        extract = gridDataExtractor.extract;
    }

    std::vector<boost::shared_ptr<GridData>> gridDatas{gridData, extract};
    std::vector<std::string> baseNames{"Rock", "Reservoir"};
    std::vector<std::string> zoneNames{"Rock", "Reservoir"};

    printGridDataInformation(extract, red("original gridData"));

    start = std::chrono::steady_clock::now();
    MultipleBasesCgnsCreator3D(gridDatas, baseNames, zoneNames, outputPath.string());
    end = std::chrono::steady_clock::now();

    elapsedSeconds = end - start;
    std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
    std::cout << std::endl << "\tOutput file location       : " << outputPath.string() << std::endl << std::endl;

    return 0;
}
