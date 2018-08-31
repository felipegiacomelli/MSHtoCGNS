#include <chrono>

#include <Grid/GridData.hpp>
#include <SpecialCgnsReader3D.hpp>
#include <SpecialCgnsCreator3D.hpp>
#include <Output.hpp>
#include <WellGenerator.hpp>

int main() {
	boost::property_tree::ptree iroot;
	boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "ScriptMender.json", iroot);
	std::string inputPath  = iroot.get<std::string>("path.input");
	std::string outputPath = iroot.get<std::string>("path.output");

	auto start = std::chrono::steady_clock::now();
	SpecialCgnsReader3D reader(inputPath);
	GridDataShared  gridData = reader.gridData;
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << std::endl << "\tGrid path: " << inputPath;
	std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

	printGridDataInformation(gridData);

	WellGenerator wellGenerator(gridData, std::string(SCRIPT_DIRECTORY) + "ScriptWellGenerator.json");

	printGridDataInformation(gridData);

	start = std::chrono::steady_clock::now();
	SpecialCgnsCreator3D creator(gridData, outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

	return 0;
}