#include <chrono>

#include <BoostInterface/PropertyTree.hpp>
#include <Utilities/Print.hpp>
#include <Grid/GridData.hpp>
#include <MshInterface/MshReader/MshReader3D.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <FileMend/GridDataExtractor.hpp>
#include <Utilities/Print.hpp>

#include <FileMend/MultipleBasesCgnsCreator3D.hpp>

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

	printGridDataInformation(gridData, "original");
	printf("\t#############################\n\n");

	GridDataExtractor gridDataExtractor(gridData, std::string(SCRIPT_DIRECTORY) + "ScriptGridDataExtractor.json");

	std::vector<boost::shared_ptr<GridData>> gridDatas{gridData, gridDataExtractor.extract};
	std::vector<std::string> baseNames{"Rock", "Reservoir"};

	printGridDataInformation(gridDatas.back(), "reservoir");
	printf("\t#############################\n");

	start = std::chrono::steady_clock::now();
	CgnsCreator3D creator(gridDataExtractor.extract, outputPath.string());
	// MultipleBasesCgnsCreator3D(gridDatas, baseNames, outputPath.string());
	end = std::chrono::steady_clock::now();

	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << outputPath.string() << std::endl << std::endl;

	return 0;
}