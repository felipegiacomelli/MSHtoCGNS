#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <cgnslib.h>

CgnsReader3D::CgnsReader3D(const std::string& filePath) : CgnsReader(filePath) {
	this->gridData->dimension = this->cellDimension;
	this->readCoordinates();
	this->readSections();
	this->readBoundaries();
}

void CgnsReader3D::readCoordinates() {
	cgsize_t one = 1;
	double coordinatesX[this->zoneSizes[0]];
	double coordinatesY[this->zoneSizes[0]];
	double coordinatesZ[this->zoneSizes[0]];
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateX", RealDouble, &one, &this->zoneSizes[0], coordinatesX)) { 
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateX");
	}
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateY", RealDouble, &one, &this->zoneSizes[0], coordinatesY)) { 
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateY");
	}
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateZ", RealDouble, &one, &this->zoneSizes[0], coordinatesZ)) { 
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateZ");
	}

	this->gridData->coordinates.resize(this->zoneSizes[0], std::vector<double>(3));
	for (int i = 0; i < this->zoneSizes[0]; i++) {
		this->gridData->coordinates[i][0] = coordinatesX[i]; 
		this->gridData->coordinates[i][1] = coordinatesY[i]; 
		this->gridData->coordinates[i][2] = coordinatesZ[i]; 
	}
}

void CgnsReader3D::readSections() {
	for (int sectionIndex = 1; sectionIndex <= this->numberOfSections; sectionIndex++) {
		ElementType_t type;
		cgsize_t elementStart, elementEnd; 
		int lastBoundaryElement, parentFlag;
		if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &type, &elementStart, &elementEnd, &lastBoundaryElement, &parentFlag)) { 
			throw std::runtime_error("CgnsReader3D: Could not read section");
		}
		int numberOfElements = elementEnd - elementStart + 1;
		
		switch (type) {
			case TETRA_4: {
				int numberOfVertices = 4;	
				cgsize_t connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader3D: Could not read section elements");
				}
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> tetrahedron(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) tetrahedron[k] = connectivities[e*numberOfVertices+k] - 1;
					this->gridData->tetrahedronConnectivity.emplace_back(std::move(tetrahedron));	
				}
				break; 
			}
			case HEXA_8: {
				int numberOfVertices = 8;	
				cgsize_t connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader3D: Could not read section elements");
				}
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> hexahedron(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) hexahedron[k] = connectivities[e*numberOfVertices+k] - 1;
					this->gridData->hexahedronConnectivity.emplace_back(std::move(hexahedron));	
				}
				break; 
			}
			case TRI_3: {
				int numberOfVertices = 3;	
				cgsize_t connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader3D: Could not read section elements");
				}
				std::vector<std::vector<int>> triangleConnectivity;
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> triangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) triangle[k] = connectivities[e*numberOfVertices+k] - 1;
					triangleConnectivity.emplace_back(std::move(triangle));	
				}
				BoundaryData boundaryData;
				boundaryData.name = this->buffer;
				boundaryData.triangleConnectivity = std::move(triangleConnectivity);
				this->gridData->boundaries.emplace_back(std::move(boundaryData));
				break; 
			}
			case QUAD_4: {
				int numberOfVertices = 4;	
				cgsize_t connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader3D: Could not read section elements");
				}
				std::vector<std::vector<int>> quadrangleConnectivity;
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> quadrangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) quadrangle[k] = connectivities[e*numberOfVertices+k] - 1;
					quadrangleConnectivity.emplace_back(std::move(quadrangle));	
				}				
				BoundaryData boundaryData;
				boundaryData.name = this->buffer;
				boundaryData.quadrangleConnectivity = std::move(quadrangleConnectivity);
				this->gridData->boundaries.emplace_back(std::move(boundaryData));
				break; 
			}
			default:
				throw std::runtime_error("CgnsReader3D: Non supported element found");
		}
	}
}