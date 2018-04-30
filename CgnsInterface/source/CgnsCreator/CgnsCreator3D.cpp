#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <cgnslib.h>

CgnsCreator3D::CgnsCreator3D(GridDataShared gridData, const std::string& folderPath) : CgnsCreator(gridData, folderPath) {
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size();
	this->sizes[2] = 0;	
	this->setupFile();
	this->initialize();
}

void CgnsCreator3D::defineGeometryType() {
	if (this->gridData->tetrahedronConnectivity.size() > 0 && this->gridData->hexahedronConnectivity.size() == 0) {
		this->geometry = 4;
		this->boundary = 3;
	}
	else if (this->gridData->tetrahedronConnectivity.size() == 0 && this->gridData->hexahedronConnectivity.size() > 0) {
		this->geometry = 8;
		this->boundary = 4;
	} 
	else throw std::runtime_error("CgnsCreator3D: Geometry type not supported");
}

void CgnsCreator3D::setupFile() {
	this->defineGeometryType();
	std::string folderName = this->folderPath + std::string("/") + std::to_string(this->sizes[0]) + std::string("v_") + std::to_string(this->sizes[1]) + "e/"; 
	createDirectory(folderName);
	this->fileName = folderName + std::string("Grid.cgns");
	cg_open(this->fileName.c_str(), CG_MODE_WRITE, &this->fileIndex);
}

void CgnsCreator3D::writeCoordinates() {
	double coordinatesX[this->sizes[0]];
	double coordinatesY[this->sizes[0]];
	double coordinatesZ[this->sizes[0]];
	for (int i = 0; i < this->sizes[0]; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
		coordinatesZ[i] = this->gridData->coordinates[i][2]; 
	}

	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0])) 
		throw std::runtime_error("CgnsCreator3D: Could not write CoordinateX");
	
	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1])) 
		throw std::runtime_error("CgnsCreator3D: Could not write CoordinateY");
	
	if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", coordinatesZ, &this->coordinateIndices[2])) 
		throw std::runtime_error("CgnsCreator3D: Could not write CoordinateZ");
	
}

void CgnsCreator3D::writeRegions() {
	this->sectionIndices.emplace_back(0);
	switch (this->geometry) {
		case 4: {
			std::vector<std::vector<int>> regionConnectivities = this->gridData->tetrahedronConnectivity;
	 		for (unsigned j = 0; j < regionConnectivities.size(); j++) 
				regionConnectivities[j].pop_back(); 	
			std::vector<int> connectivities = linearize(regionConnectivities);
			for (unsigned j = 0; j < connectivities.size(); j++) 
				connectivities[j]++;
			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", TETRA_4, 
									1, this->sizes[1], sizes[2], &connectivities[0], &this->sectionIndices.back())) 
				throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(1));

			break;
		}
		case 8: {
			std::vector<std::vector<int>> regionConnectivities = this->gridData->hexahedronConnectivity;
		 	for (unsigned j = 0; j < regionConnectivities.size(); j++) 
				regionConnectivities[j].pop_back(); 	
			std::vector<int> connectivities = linearize(regionConnectivities);
			for (unsigned j = 0; j < connectivities.size(); j++) 
				connectivities[j]++;
			if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", HEXA_8, 
									1, this->sizes[1], sizes[2], &connectivities[0], &this->sectionIndices.back())) 
				throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(1));
			
			break;
		}
		default: 
			throw std::runtime_error("CgnsCreator3D: Geometry type not supported");
	}
}

void CgnsCreator3D::writeBoundaries() {
	this->sectionIndices.emplace_back(0);
	switch (this->boundary) {
		case 3: {
			int elementStart = this->sizes[1] + 1;
			int elementEnd;
			for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
				std::vector<std::vector<int>> boundaryConnectivities(this->gridData->boundaries[i].triangleConnectivity.cbegin(), this->gridData->boundaries[i].triangleConnectivity.cend());
				for (unsigned j = 0; j < boundaryConnectivities.size(); j++) 
					boundaryConnectivities[j].pop_back(); 
				elementEnd = elementStart + this->gridData->boundaries[i].triangleConnectivity.size() - 1;
				
				std::vector<int> connectivities = linearize(boundaryConnectivities);
				for (unsigned j = 0; j < connectivities.size(); j++) 
					connectivities[j]++;

				if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), TRI_3, 
										elementStart, elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndices.back())) 
					throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(i+2));
				
				elementStart = elementEnd + 1;
			}
			break;
		}
		case 4: {
			int elementStart = this->sizes[1] + 1;
			int elementEnd;
			for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
				std::vector<std::vector<int>> boundaryConnectivities(this->gridData->boundaries[i].quadrangleConnectivity.cbegin(), this->gridData->boundaries[i].quadrangleConnectivity.cend());
				for (unsigned j = 0; j < boundaryConnectivities.size(); j++) 
					boundaryConnectivities[j].pop_back(); 
				elementEnd = elementStart + this->gridData->boundaries[i].quadrangleConnectivity.size() - 1;
				
				std::vector<int> connectivities = linearize(boundaryConnectivities);
				for (unsigned j = 0; j < connectivities.size(); j++) 
					connectivities[j]++;

				if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), QUAD_4, 
										elementStart, elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndices.back())) 
					throw std::runtime_error("CgnsCreator3D: Could not write section " + std::to_string(i+2));
				
				elementStart = elementEnd + 1;
			}
			break;
		}
		default: 
			throw std::runtime_error("CgnsCreator3D: Boundary type not supported");
	}
}