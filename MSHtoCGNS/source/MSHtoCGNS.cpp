#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader2D.hpp>
#include <IO/MshReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator2D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>

void output(GridDataShared gridData, std::ofstream& file) {
	int numberOfNodes = gridData->coordinates.size();
	int numberOfElements = gridData->triangleConnectivity.size() + gridData->quadrangleConnectivity.size();

	file << "GridData ###"        << std::endl;
	file << "Number of nodes:   " << std::setw(3) << std::right << numberOfNodes    << std::endl;
	file << "Number of element: " << std::setw(3) << std::right << numberOfElements << std::endl;

	file << std::endl << "Coordinates" << std::endl;
	for (auto i = gridData->coordinates.cbegin(); i != gridData->coordinates.cend(); i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++)
			file << "\t" << std::setw(3) << std::right << *j;
		file << std::endl;
	}

	if (gridData->triangleConnectivity.size() > 0) {
		file << std::endl << "Triangle connectivity" << std::endl;
		for (auto i = gridData->triangleConnectivity.cbegin(); i != gridData->triangleConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}
	if (gridData->quadrangleConnectivity.size() > 0) {
		file << std::endl << "Quadrangle connectivity" << std::endl;
		for (auto i = gridData->quadrangleConnectivity.cbegin(); i != gridData->quadrangleConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}
	if (gridData->lineConnectivity.size() > 0) {
		file << std::endl << "Line connectivity" << std::endl;
		for (auto i = gridData->lineConnectivity.cbegin(); i != gridData->lineConnectivity.cend(); i++) {
			for (auto j = i->cbegin(); j != i->cend(); j++)
				file << "\t" << std::setw(3) << std::right << *j;
			file << std::endl;
		}
	}

	file << std::endl << "Boundaries ###" << std::endl;
	for (auto i = gridData->boundaries.cbegin(); i != gridData->boundaries.cend(); i ++) {
		file << std::endl << "\t" << i->name << std::endl;
		for (auto j = i->facetsOnBoundary.cbegin(); j != i->facetsOnBoundary.cend(); j++)
			file << "\t" << *j;
		file << std::endl;

	}

	file << std::endl << "Regions ###" << std::endl;
	for (auto i = gridData->regions.cbegin(); i != gridData->regions.cend(); i ++) {
		file << std::endl << "\t" << i->name << std::endl;
		for (auto j = i->elementsOnRegion.cbegin(); j != i->elementsOnRegion.cend(); j++)
			file << "\t" << *j;
		file << std::endl;
	}
}

int main(int argc, char** argv) {
	if (argc != 2) throw std::runtime_error("Parameter is the mesh dimension.");
	unsigned dimension = std::stoul(argv[1]);

	switch (dimension) {
		case 2: {
   			boost::property_tree::ptree iroot;
    		boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "Script2D.json", iroot);
    		std::string inputPath  = iroot.get<std::string>("path.input");
    		std::string outputPath = iroot.get<std::string>("path.output");

			auto start = std::chrono::steady_clock::now();
			MshReader2D reader2D(inputPath);
			GridDataShared  gridData = reader2D.gridData;
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedSeconds = end - start;
			std::cout << std::endl << "\tGrid path: " << inputPath;
			std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

			start = std::chrono::steady_clock::now();
			CgnsCreator2D fileIndex2D(gridData, outputPath);
			end = std::chrono::steady_clock::now();
			elapsedSeconds = end - start;
			std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
			std::cout << std::endl << "\tOutput file location       : " << fileIndex2D.getFileName() << std::endl << std::endl;

			// std::ofstream file("GridData.txt", std::ofstream::out);
			// output(gridData, file);
			// file.close();

			break;
		}
		case 3: {
			boost::property_tree::ptree iroot;
			boost::property_tree::read_json(std::string(SCRIPT_DIRECTORY) + "Script3D.json", iroot);
			std::string inputPath  = iroot.get<std::string>("path.input");
			std::string outputPath = iroot.get<std::string>("path.output");

			auto start = std::chrono::steady_clock::now();
			MshReader3D reader3D(inputPath);
			GridDataShared  gridData = reader3D.gridData;
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedSeconds = end - start;
			std::cout << std::endl << "\tGrid path: " << inputPath;
			std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;

			start = std::chrono::steady_clock::now();
			CgnsCreator3D fileIndex3D(gridData, outputPath);
			end = std::chrono::steady_clock::now();
			elapsedSeconds = end - start;
			std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";
			std::cout << std::endl << "\tOutput file location       : " << fileIndex3D.getFileName() << std::endl << std::endl;

			break;
		}
		default:
			throw std::runtime_error("Dimensions must be either 2 or 3");
			break;
	}

	return 0;
}