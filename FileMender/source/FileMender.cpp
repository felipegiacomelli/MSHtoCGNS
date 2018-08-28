#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
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

	// {
	// 	auto elementConnectivities = buildElementConnectivities(gridData);
	// 	std::string regionName("BODY_INJECTOR_WEST");
	// 	std::array<double, 3> wellStart({-80.0, 60.0, 0.0});
	// 	int wellDirection = 2;
	// 	auto wellData = generateWell(elementConnectivities, gridData, regionName, wellStart, wellDirection);
	// }

	// {
	// 	auto elementConnectivities = buildElementConnectivities(gridData);
	// 	std::string regionName("BODY_INJECTOR_EAST");
	// 	std::array<double, 3> wellStart({80.0, 60.0, 0.0});
	// 	int wellDirection = 2;
	// 	auto wellData = generateWell(elementConnectivities, gridData, regionName, wellStart, wellDirection);
	// }

	// {
	// 	auto elementConnectivities = buildElementConnectivities(gridData);
	// 	std::string regionName("BODY_WELL");
	// 	std::array<double, 3> wellStart({0.0, -100.0, 30.0});
	// 	int wellDirection = 1;
	// 	auto wellData = generateWell(elementConnectivities, gridData, regionName, wellStart, wellDirection);
	// }

	start = std::chrono::steady_clock::now();
	SpecialCgnsCreator3D creator(gridData, outputPath);
	end = std::chrono::steady_clock::now();
	elapsedSeconds = end - start;
	std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
	std::cout << std::endl << "\tOutput file location       : " << creator.getFileName() << std::endl << std::endl;

	return 0;
}