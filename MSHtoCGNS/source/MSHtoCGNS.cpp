#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <IO/GridReader.hpp>
#include <IO/GridReader2D.hpp>
#include <IO/GridReader3D.hpp>

#include <chrono>

void printGridData(GridData);

int main() {
	{
		std::ifstream file("../../../Script2D.txt");
		std::string line;
		std::getline(file, line);
		std::istringstream stream(line);
	
		auto start = std::chrono::steady_clock::now();
		GridReader2D reader2D(std::move(line));
		GridData gridData = reader2D.getGridData();
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedSeconds = end - start;
		
		std::cout << std::endl << "\tGrid path: " << line;			
		std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl << std::endl;			
		printGridData(gridData);
	}

	{
		std::ifstream file("../../../Script3D.txt");
		std::string line;
		std::getline(file, line);
		std::istringstream stream(line);
	
		auto start = std::chrono::steady_clock::now();
		GridReader3D reader3D(std::move(line));
		GridData gridData = reader3D.getGridData();
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedSeconds = end - start;
		
		std::cout << std::endl << "\tGrid path: " << line;			
		std::cout << std::endl << "\tRead in  : " << elapsedSeconds.count() << " s" << std::endl << std::endl;			
		printGridData(gridData);
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