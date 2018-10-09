#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>
#include <MshInterface/Output.hpp>
#include <MshInterface/MshReader/MshReader2D.hpp>
#include <MshInterface/MshReader/MshReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator2D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>

int main(int argc, char** argv) {
	if (argc != 2)
		throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Mesh dimension must be passed as a parameter");
	unsigned dimension = std::stoul(argv[1]);

	switch (dimension) {
		case 2: {
   			boost::property_tree::ptree propertyTree;
    		boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "Script2D.json", propertyTree);
    		std::string inputPath  = propertyTree.get<std::string>("path.input");
    		std::string outputPath = propertyTree.get<std::string>("path.output");

			auto start = std::chrono::steady_clock::now();
			MshReader2D reader2D(inputPath);
			GridDataShared gridData = reader2D.gridData;
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedSeconds = end - start;
			std::cout << std::endl << "\tGrid path: " << inputPath;
			std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

			start = std::chrono::steady_clock::now();
			CgnsCreator2D creator2D(gridData, outputPath);
			end = std::chrono::steady_clock::now();
			elapsedSeconds = end - start;
			std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
			std::cout << std::endl << "\tOutput file location       : " << creator2D.getFileName() << std::endl << std::endl;

			break;
		}
		case 3: {
			boost::property_tree::ptree propertyTree;
			boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "Script3D.json", propertyTree);
			std::string inputPath  = propertyTree.get<std::string>("path.input");
			std::string outputPath = propertyTree.get<std::string>("path.output");

			auto start = std::chrono::steady_clock::now();
			MshReader3D reader3D(inputPath);
			GridDataShared gridData = reader3D.gridData;
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedSeconds = end - start;
			std::cout << std::endl << "\tGrid path: " << inputPath;
			std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

			start = std::chrono::steady_clock::now();
			CgnsCreator3D creator3D(gridData, outputPath);
			end = std::chrono::steady_clock::now();
			elapsedSeconds = end - start;
			std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
			std::cout << std::endl << "\tOutput file location       : " << creator3D.getFileName() << std::endl << std::endl;

			break;
		}
		default:
			throw std::runtime_error("Dimensions must be either 2 or 3");
	}

	return 0;
}