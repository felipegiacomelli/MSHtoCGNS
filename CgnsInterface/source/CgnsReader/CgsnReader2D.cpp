#include <CgnsInterface/CgnsReader/CgnsReader2D.hpp>
#include <cgnslib.h>
 
CgnsReader2D::CgnsReader2D(const std::string& filePath) : CgnsReader(filePath) {
	this->gridData->dimension = this->cellDimension;
	this->gridData->thickness = 1.0;
	this->readCoordinates();
	this->readSections();
	this->readBoundaries();
}

void CgnsReader2D::readCoordinates() {
	int one = 1;
	double coordinatesX[this->sizes[0]];
	double coordinatesY[this->sizes[0]];
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateX", RealDouble, &one, &this->sizes[0], coordinatesX)) {
		throw std::runtime_error("CgnsReader2D: Could not read CoordinateX");
	}
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateY", RealDouble, &one, &this->sizes[0], coordinatesY)) {
		throw std::runtime_error("CgnsReader2D: Could not read CoordinateY");
	}

	this->gridData->coordinates.resize(this->sizes[0], std::vector<double>(3));
	for (int i = 0; i < this->sizes[0]; i++) {
		this->gridData->coordinates[i][0] = coordinatesX[i]; 
		this->gridData->coordinates[i][1] = coordinatesY[i]; 
		this->gridData->coordinates[i][2] = 0.0; 
	}
}

void CgnsReader2D::readSections() {
	for (int sectionIndex = 1; sectionIndex <= this->numberOfSections; sectionIndex++) {
		ElementType_t type;
		int elementStart, elementEnd; 
		int lastBoundaryElement, parentFlag;
		if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &type, &elementStart, &elementEnd, &lastBoundaryElement, &parentFlag)) {
			throw std::runtime_error("CgnsReader2D: Could not read section");
		}
		int numberOfElements = elementEnd - elementStart + 1;
		
		switch (type) {
			case TRI_3: {
				int numberOfVertices = 3;	
				int connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader2D: Could not read section elements");
				}
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> triangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) triangle[k] = connectivities[e*numberOfVertices+k] - 1;
					this->gridData->triangleConnectivity.emplace_back(std::move(triangle));	
				}
				RegionData region;
				region.name = std::string(this->buffer);
				region.elementsOnRegion.resize(numberOfElements);
				std::iota(region.elementsOnRegion.begin(), region.elementsOnRegion.end(), elementStart - 1);
				this->gridData->regions.emplace_back(std::move(region));
				break; 
			}
			case QUAD_4: {
				int numberOfVertices = 4;	
				int connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader2D: Could not read section elements");
				}
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> quadrangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) quadrangle[k] = connectivities[e*numberOfVertices+k]-1;
					this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));	
				}
				RegionData region;
				region.name = std::string(this->buffer);
				region.elementsOnRegion.resize(numberOfElements);
				std::iota(region.elementsOnRegion.begin(), region.elementsOnRegion.end(), elementStart - 1);
				this->gridData->regions.emplace_back(std::move(region));
				break; 
			}
			case BAR_2: {
				int numberOfVertices = 2;	
				int connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader2D: Could not read section elements");
				}
				std::vector<std::vector<int>> lineConnectivity;
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> line(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) line[k] = connectivities[e*numberOfVertices+k] - 1;
					lineConnectivity.emplace_back(std::move(line));
				}
				BoundaryData boundaryData;
				boundaryData.name = this->buffer;
				boundaryData.lineConnectivity = std::move(lineConnectivity);
				this->gridData->boundaries.emplace_back(std::move(boundaryData));
				break; 
			}
			default:
				throw std::runtime_error("CgnsReader2D: Could not read section");
		}
	}
}