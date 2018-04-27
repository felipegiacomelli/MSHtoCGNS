#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader2D.hpp>
#include <IO/MshReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsFile2D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsFile3D.hpp>

#include <chrono>
#include <fstream>

int main(int argc, char** argv) {
	if (argc != 2) throw std::runtime_error("Parameter is the mesh dimension.");		
	unsigned dimension = std::stoul(argv[1]);

	switch (dimension) {
		case 2: {
   			boost::property_tree::ptree iroot;
    		boost::property_tree::read_json("../../../Zeta/Script2D.json", iroot);
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
			CgnsFile2D fileIndex2D(gridData, outputPath); 
			end = std::chrono::steady_clock::now();
			elapsedSeconds = end - start;
			std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";			
			std::cout << std::endl << "\tOutput file location       : " << fileIndex2D.getFileName() << std::endl << std::endl;			
			break;
		}
		case 3: {
			boost::property_tree::ptree iroot;
			boost::property_tree::read_json("../../../Zeta/Script3D.json", iroot);
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
			CgnsFile3D fileIndex3D(gridData, outputPath); 
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