#include <CgnsInterface/CgnsCreator/CgnsFile3D.hpp>
#include <cgnslib.h>

CgnsFile3D::CgnsFile3D(GridDataShared gridData, const std::string& folderPath) : CgnsFile(gridData, folderPath) {
	this->sizes[0] = this->gridData->coordinates.size();
	this->sizes[1] = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size();
	this->sizes[2] = 0;	
	this->setupFile();
	this->initialize();
}

void CgnsFile3D::defineGeometryType() {
	if (this->gridData->tetrahedronConnectivity.size() > 0 && this->gridData->hexahedronConnectivity.size() == 0) {
		this->geometry = 4;
		this->boundary = 3;
	}
	else if (this->gridData->tetrahedronConnectivity.size() == 0 && this->gridData->hexahedronConnectivity.size() > 0) {
		this->geometry = 8;
		this->boundary = 4;
	} 
	else throw std::runtime_error("CgnsFile3D: Geometry type not supported");
}

void CgnsFile3D::setupFile() {
	this->defineGeometryType();
	std::string folderName = this->folderPath + std::string("/") + std::to_string(this->sizes[0]) + std::string("v_") + std::to_string(this->sizes[1]) + "e/"; 
	createDirectory(folderName);
	this->fileName = folderName + std::string("Grid.cgns");
	cg_open(this->fileName.c_str(), CG_MODE_WRITE, &this->fileIndex);
}

void CgnsFile3D::writeCoordinates() {
	double coordinatesX[this->sizes[0]];
	double coordinatesY[this->sizes[0]];
	double coordinatesZ[this->sizes[0]];
	for (int i = 0; i < this->sizes[0]; i++) {
		coordinatesX[i] = this->gridData->coordinates[i][0];
		coordinatesY[i] = this->gridData->coordinates[i][1];
		coordinatesZ[i] = this->gridData->coordinates[i][2]; 
	}
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", coordinatesX, &this->coordinateIndices[0]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", coordinatesY, &this->coordinateIndices[1]);
	cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", coordinatesZ, &this->coordinateIndices[2]);
}

void CgnsFile3D::writeSections() {
	switch (this->geometry) {
		case 4: {
			int* connectivities = determine_array_1d<int>(this->gridData->tetrahedronConnectivity);
			for (unsigned j = 0; j < this->gridData->tetrahedronConnectivity.size()*4; j++) connectivities[j]++;
			cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", TETRA_4, 1, this->sizes[1], sizes[2], connectivities, &sectionIndices[0]);
			delete connectivities;
			break;
		}
		case 8: {
			int* connectivities = determine_array_1d<int>(this->gridData->hexahedronConnectivity);
			for (unsigned j = 0; j < this->gridData->hexahedronConnectivity.size()*8; j++) connectivities[j]++;
			cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, "Geometry", HEXA_8, 1, this->sizes[1], sizes[2], connectivities, &sectionIndices[0]);
			delete connectivities;
			break;
		}
		default: 
			throw std::runtime_error("CgnsFile3D: Geometry type not supported");
	}

	switch (this->boundary) {
		case 3: {
			int elementStart = this->sizes[1] + 1;
			int elementEnd;
			for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
				elementEnd = elementStart + this->gridData->boundaries[i].triangleConnectivity.size() - 1;
				int* connectivities = determine_array_1d<int>(this->gridData->boundaries[i].triangleConnectivity);
				for (unsigned j = 0; j < this->gridData->boundaries[i].triangleConnectivity.size()*3; j++) connectivities[j]++;
				cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), TRI_3, elementStart, elementEnd, this->sizes[2], connectivities, &this->sectionIndices[i+1]);
				delete connectivities;
				elementStart = elementEnd + 1;
			}
			break;
		}
		case 4: {
			int elementStart = this->sizes[1] + 1;
			int elementEnd;
			for (unsigned i = 0; i < this->gridData->boundaries.size(); i++) {
				elementEnd = elementStart + this->gridData->boundaries[i].quadrangleConnectivity.size() - 1;
				int* connectivities = determine_array_1d<int>(this->gridData->boundaries[i].quadrangleConnectivity);
				for (unsigned j = 0; j < this->gridData->boundaries[i].quadrangleConnectivity.size()*4; j++) connectivities[j]++;
				cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->gridData->boundaries[i].name.c_str(), QUAD_4, elementStart, elementEnd, this->sizes[2], connectivities, &this->sectionIndices[i+1]);
				delete connectivities;
				elementStart = elementEnd + 1;
			}
			break;
		}
		default: 
			throw std::runtime_error("CgnsFile3D: Boundary type not supported");
	}
}