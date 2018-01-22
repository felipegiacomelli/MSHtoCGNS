#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/PropertyTree.hpp>
#include <Grid/GridData.hpp>
#include <IO/GridReader2D.hpp>
#include <IO/GridReader3D.hpp>
#include <CgnsInterface/CgnsFile2D.hpp>
#include <CgnsInterface/CgnsFile3D.hpp>

#include <chrono>
#include <fstream>

void printGridData(GridData);
void outputGridData(GridData, std::ofstream&);

int main() {
	{
   		boost::property_tree::ptree iroot;
    	boost::property_tree::read_json("../../../Zeta/Script2D.json", iroot);
    	std::string inputPath  = iroot.get<std::string>("path.input");
    	std::string outputPath = iroot.get<std::string>("path.output");

		auto start = std::chrono::steady_clock::now();
		GridReader2D reader2D(inputPath);
		GridData gridData = reader2D.getGridData();
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedSeconds = end - start;
		std::cout << std::endl << "\tGrid path: " << inputPath;			
		std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;			
		
		start = std::chrono::steady_clock::now();
		CgnsFile2D cgnsFile2D(gridData, outputPath); 
		cgnsFile2D.initialize(); 
		end = std::chrono::steady_clock::now();
		elapsedSeconds = end - start;
		std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";			
		std::cout << std::endl << "\tOutput file location       : " << cgnsFile2D.getFileName() << std::endl << std::endl;			
		
		//std::ofstream output("./2DGrid.txt");
		//outputGridData(gridData, output);
	}
	std::cout << "\t#########################################################################" << std::endl;
	{
   		boost::property_tree::ptree iroot;
    	boost::property_tree::read_json("../../../Zeta/Script3D.json", iroot);
    	std::string inputPath  = iroot.get<std::string>("path.input");
    	std::string outputPath = iroot.get<std::string>("path.output");

		auto start = std::chrono::steady_clock::now();
		GridReader3D reader3D(inputPath);
		GridData gridData = reader3D.getGridData();
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedSeconds = end - start;
		std::cout << std::endl << "\tGrid path: " << inputPath;			
		std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl;	
		
		start = std::chrono::steady_clock::now();
		CgnsFile3D cgnsFile3D(gridData, outputPath); 
		cgnsFile3D.initialize(); 
		end = std::chrono::steady_clock::now();
		elapsedSeconds = end - start;
		std::cout << std::endl << "\tConverted to CGNS format in: " << elapsedSeconds.count() << " s";			
		std::cout << std::endl << "\tOutput file location       : " << cgnsFile3D.getFileName() << std::endl << std::endl;				
		
		//std::ofstream output("./3DGrid.txt");
		//outputGridData(gridData, output);
	}
	return 0;
}

void printGridData(GridData gridData) {
		int numberOfNodes = gridData.coordinates.size();
		int numberOfElements = gridData.triangleConnectivity.size() + gridData.quadrangleConnectivity.size() + gridData.tetrahedronConnectivity.size() + gridData.hexahedronConnectivity.size() + gridData.pyramidConnectivity.size() + gridData.prismConnectivity.size(); 
		
		std::cout << std::endl;
		std::cout << "\tGridData ###"        << std::endl;
		std::cout << "\tNumber of nodes:   " << std::setw(5) << std::right << numberOfNodes    << std::endl;
		std::cout << "\tNumber of element: " << std::setw(5) << std::right << numberOfElements << std::endl;
	
		//std::cout << std::endl << "\tCoordinates" << std::endl;
		//print(gridData.coordinates);
		
		if (gridData.dimension == 2) {
			if (gridData.triangleConnectivity.size() > 0) {
				std::cout << std::endl << "\tTriangle connectivity - " << gridData.triangleConnectivity.size() << std::endl;
				// print(gridData.triangleConnectivity, "");
			}
			if (gridData.quadrangleConnectivity.size() > 0) {
				std::cout << std::endl << "\tQuadrangle connectivity - " << gridData.quadrangleConnectivity.size() << std::endl;
				//print(gridData.quadrangleConnectivity);				
			}
		}
		else {
			if (gridData.tetrahedronConnectivity.size() > 0) {
				std::cout << std::endl << "\tTetrahedron connectivity - " << gridData.tetrahedronConnectivity.size() << std::endl;
				//print(gridData.tetrahedronConnectivity);
			}
			if (gridData.hexahedronConnectivity.size() > 0) {
				std::cout << std::endl << "\tHexahedron connectivity - " << gridData.hexahedronConnectivity.size() << std::endl;
				//print(gridData.hexahedronConnectivity);				
			}
			if (gridData.pyramidConnectivity.size() > 0) {
				std::cout << std::endl << "\tPyramid connectivity - " << gridData.pyramidConnectivity.size() << std::endl;
				//print(gridData.pyramidConnectivity);
			}
			if (gridData.prismConnectivity.size() > 0) {
				std::cout << std::endl << "\tPrism connectivity - " << gridData.prismConnectivity.size() << std::endl;
				//print(gridData.prismConnectivity);				
			}				
		}
	
		std::cout << std::endl << "\tBoundaries ###" << std::endl;
		if (gridData.dimension == 2) {
			for (auto i = gridData.boundaries.cbegin(); i != gridData.boundaries.cend(); i ++) {
				std::cout << std::endl << "\t" << i->name << std::endl;
				if (i->lineConnectivity.size() > 0) std::cout << std::endl << "\t\tLine connectivity - " << i->lineConnectivity.size() << std::endl;
				// print(i->lineConnectivity, "lineConnectivity");
			}
		}
		else {
			for (auto i = gridData.boundaries.cbegin(); i != gridData.boundaries.cend(); i ++) {
				std::cout << std::endl << "\t" << i->name << std::endl;
				if (i->triangleConnectivity.size() > 0) std::cout << std::endl << "\t\tTriangle connectivity - " << i->triangleConnectivity.size() << std::endl;
				//print(i->triangleConnectivity);
				std::cout << std::endl;
				if (i->quadrangleConnectivity.size() > 0) std::cout << std::endl << "\t\tQuadrangle connectivity - " << i->quadrangleConnectivity.size() << std::endl;
				//print(i->quadrangleConnectivity);
			}
		}	
	
		std::cout << std::endl << "\tWells ###" << std::endl;
		for (auto i = gridData.wells.cbegin(); i != gridData.wells.cend(); i ++) {
			std::cout << "\t" << i->name << std::endl;
			std::cout << "\t" << i->wellNode << std::endl;
		}	
		
		std::cout << std::endl << "\tRegions ###" << std::endl;
		for (auto i = gridData.regions.cbegin(); i != gridData.regions.cend(); i ++) {
			std::cout << "\t" << i->name << std::endl;
			std::cout << "\t" << i->elementsOnRegion.size() << std::endl;
		}			
}

void outputGridData(GridData gridData, std::ofstream& file) {
		int numberOfNodes = gridData.coordinates.size();
		int numberOfElements = gridData.triangleConnectivity.size() + gridData.triangleConnectivity.size() + gridData.quadrangleConnectivity.size() + gridData.tetrahedronConnectivity.size() + gridData.hexahedronConnectivity.size() + gridData.pyramidConnectivity.size() + gridData.prismConnectivity.size(); 
		
		file << std::endl;
		file << "\tGridData ###"        << std::endl;
		file << "\tNumber of nodes:   " << std::setw(5) << std::right << numberOfNodes    << std::endl;
		file << "\tNumber of element: " << std::setw(5) << std::right << numberOfElements << std::endl;
	
		file << std::endl << "\tCoordinates" << std::endl;
		output(gridData.coordinates, file);
		
		if (gridData.dimension == 2) {
			if (gridData.triangleConnectivity.size() > 0) {
				file << std::endl << "\tTriangle connectivity" << std::endl;
				output(gridData.triangleConnectivity, file);
			}
			if (gridData.quadrangleConnectivity.size() > 0) {
				file << std::endl << "\tQuadrangle connectivity" << std::endl;
				output(gridData.quadrangleConnectivity, file);				
			}
		}
		else {
			if (gridData.tetrahedronConnectivity.size() > 0) {
				file << std::endl << "\tTetrahedron connectivity" << std::endl;
				output(gridData.tetrahedronConnectivity, file);
			}
			if (gridData.hexahedronConnectivity.size() > 0) {
				file << std::endl << "\tHexahedron connectivity" << std::endl;
				output(gridData.hexahedronConnectivity, file);				
			}
			if (gridData.pyramidConnectivity.size() > 0) {
				file << std::endl << "\tPyramid connectivity" << std::endl;
				output(gridData.pyramidConnectivity, file);
			}
			if (gridData.prismConnectivity.size() > 0) {
				file << std::endl << "\tPrism connectivity" << std::endl;
				output(gridData.prismConnectivity, file);				
			}				
		}
	
		file << std::endl << "\tBoundaries ###" << std::endl;
		if (gridData.dimension == 2) {
			for (auto i = gridData.boundaries.cbegin(); i != gridData.boundaries.cend(); i ++) {
				file << std::endl << "\t" << i->name << std::endl;
				output(i->lineConnectivity, file);
			}
		}
		else {
			for (auto i = gridData.boundaries.cbegin(); i != gridData.boundaries.cend(); i ++) {
				file << std::endl << "\t" << i->name << std::endl;
				output(i->triangleConnectivity, file);
				file << std::endl;
				output(i->quadrangleConnectivity, file);
			}
		}	
	
		file << std::endl << "\tWells ###" << std::endl;
		for (auto i = gridData.wells.cbegin(); i != gridData.wells.cend(); i ++) {
			file << "\t" << i->name << std::endl;
			file << "\t" << i->wellNode << std::endl;
		}	
		
		file << std::endl << "\tRegions ###" << std::endl;
		for (auto i = gridData.regions.cbegin(); i != gridData.regions.cend(); i ++) {
			file << "\t" << i->name << std::endl;
			file << "\t" << i->elementsOnRegion.size() << std::endl;
		}			
}	
