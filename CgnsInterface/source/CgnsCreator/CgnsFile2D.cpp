#include <CgnsInterface/CgnsCreator/CgnsFile2D.hpp>
#include <cgnslib.h>

CgnsFile2D::CgnsFile2D(GridDataShared gridData, const std::string& folderPath) : CgnsFile(gridData, folderPath) {
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->triangleConnectivity.size() + this->gridData->quadrangleConnectivity.size();
	this->sizes[2] = 0;	
	this->setupFile();
	this->initialize();
}

void CgnsFile2D::setupFile() {
	std::string folderName = this->folderPath + std::string("/") + std::to_string(this->sizes[0]) + std::string("v_") + std::to_string(this->sizes[1]) + "e/"; 
	createDirectory(folderName);
	this->fileName = folderName + std::string("Grid.cgns");
	cg_open(this->fileName.c_str(), CG_MODE_WRITE, &this->fileIndex);
}

void CgnsFile2D::writeCoordinates() {
	double coordinatesX[this->sizes[0]];
	double coordinatesY[this->sizes[0]];
	for (int i = 0; i < this->sizes[0]; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
	}
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]);
}

void CgnsFile2D::writeSections() {
	std::vector<std::vector<int>> elementConnectivities;
	elementConnectivities.insert(elementConnectivities.end(), this->gridData->triangleConnectivity.begin(), this->gridData->triangleConnectivity.end());
	elementConnectivities.insert(elementConnectivities.end(), this->gridData->quadrangleConnectivity.begin(), this->gridData->quadrangleConnectivity.end());
	int elementStart = 1;
	int elementEnd = 0;

	for (unsigned i = 0; i < this->gridData->regions.size(); i++) {
		this->sectionIndices.emplace_back(0);
		std::vector<std::vector<int>> regionConnectivities(elementConnectivities.cbegin()+this->gridData->regions[i].elementsOnRegion.front(), 
															elementConnectivities.cbegin()+this->gridData->regions[i].elementsOnRegion.back()+1);
	 	elementEnd += regionConnectivities.size();
		switch (this->gridData->regions[i].elementType) {
			case 1:  {
				int* connectivities = determine_array_1d<int>(regionConnectivities);
				for (unsigned j = 0; j < regionConnectivities.size()*3; j++) connectivities[j]++;
				cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->regions[i].name.c_str(), TRI_3, elementStart, elementEnd, sizes[2], connectivities, &this->sectionIndices.back());
				delete connectivities;
				break;
			}
			case 2: {
				int* connectivities = determine_array_1d<int>(regionConnectivities);
				for (unsigned j = 0; j < regionConnectivities.size()*4; j++) connectivities[j]++;
				cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->regions[i].name.c_str(), QUAD_4, elementStart, elementEnd, sizes[2], connectivities, &this->sectionIndices.back());
				delete connectivities;
				break;
			}
			default:
				throw std::runtime_error("CgnsFile2D: Geometry type not supported");
		}
		elementStart = elementEnd + 1;
	}
	
	elementStart = this->sizes[1] + 1;
	for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
		this->sectionIndices.emplace_back(0);
		elementEnd = elementStart + this->gridData->boundaries[i].lineConnectivity.size() - 1;
		int* connectivities = determine_array_1d<int>(this->gridData->boundaries[i].lineConnectivity);
		for (unsigned j = 0; j < this->gridData->boundaries[i].lineConnectivity.size()*2; j++) connectivities[j]++;
		cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), BAR_2, elementStart, elementEnd, this->sizes[2], connectivities, &this->sectionIndices[i+1]);
		delete connectivities;
		elementStart = elementEnd + 1;
	}
}