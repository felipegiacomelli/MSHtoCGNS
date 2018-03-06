#include <IO/CgnsReader3D.hpp>
 
CgnsReader3D::CgnsReader3D(const std::string& filePath) :
	CgnsReader(filePath) {
	this->gridData->dimension = this->cellDimension;
	this->readCoordinates();
	this->readSections();
	this->readBoundaries();
}

void CgnsReader3D::readCoordinates() {
	this->numberOfNodes = this->zoneSizes[0];
	cgsize_t one = 1;
	double coordinatesX[this->numberOfNodes];
	double coordinatesY[this->numberOfNodes];
	double coordinatesZ[this->numberOfNodes];
	if (cg_coord_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, "CoordinateX", RealDouble, &one, &this->numberOfNodes, coordinatesX)) { 
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateX");
	}
	if (cg_coord_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, "CoordinateY", RealDouble, &one, &this->numberOfNodes, coordinatesY)) { 
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateY");
	}
	if (cg_coord_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, "CoordinateZ", RealDouble, &one, &this->numberOfNodes, coordinatesZ)) { 
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateZ");
	}

	this->gridData->coordinates.resize(this->numberOfNodes, std::vector<double>(3));
	for (int i = 0; i < this->numberOfNodes; i++) {
		this->gridData->coordinates[i][0] = coordinatesX[i]; 
		this->gridData->coordinates[i][1] = coordinatesY[i]; 
		this->gridData->coordinates[i][2] = coordinatesZ[i]; 
	}
}

void CgnsReader3D::readSections() {
	for (auto section = this->sectionIndices.cbegin(); section != this->sectionIndices.cend(); section++) {
		ElementType_t type;
		cgsize_t elementStart, elementEnd; 
		int nBdry, parentFlag;
		if (cg_section_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, buffer, &type, &elementStart, &elementEnd, &nBdry, &parentFlag)) { 
			throw std::runtime_error("CgnsReader3D: Could not read section");
		}
		int numberOfElements = elementEnd - elementStart + 1;
		
		switch (type) {
			case TETRA_4: {
				int numberOfVertices = 4;	
				std::vector<cgsize_t> connectivities(numberOfVertices*numberOfElements);
				cg_elements_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, &connectivities[0], 0);
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> tetrahedron(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) tetrahedron[k] = connectivities[e*numberOfVertices+k] - 1;
					this->gridData->tetrahedronConnectivity.emplace_back(std::move(tetrahedron));	
				}
				break; 
			}
			case HEXA_8: {
				int numberOfVertices = 8;	
				std::vector<cgsize_t> connectivities(numberOfVertices*numberOfElements);
				cg_elements_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, &connectivities[0], 0);
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> hexahedron(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) hexahedron[k] = connectivities[e*numberOfVertices+k] - 1;
					this->gridData->hexahedronConnectivity.emplace_back(std::move(hexahedron));	
				}
				break; 
			}
			case TRI_3: {
				int numberOfVertices = 3;	
				std::vector<cgsize_t> connectivities(numberOfVertices*numberOfElements);
				cg_elements_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, &connectivities[0], 0);
				std::vector<std::vector<int>> triangleConnectivity;
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> triangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) triangle[k] = connectivities[e*numberOfVertices+k] - 1;
					triangleConnectivity.emplace_back(std::move(triangle));	
				}
				BoundaryData boundaryData;
				boundaryData.name = buffer;
				boundaryData.triangleConnectivity = std::move(triangleConnectivity);
				this->gridData->boundaries.emplace_back(std::move(boundaryData));
				break; 
			}
			case QUAD_4: {
				int numberOfVertices = 4;	
				std::vector<cgsize_t> connectivities(numberOfVertices*numberOfElements);
				cg_elements_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, &connectivities[0], 0);
				std::vector<std::vector<int>> quadrangleConnectivity;
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> quadrangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) quadrangle[k] = connectivities[e*numberOfVertices+k] - 1;
					quadrangleConnectivity.emplace_back(std::move(quadrangle));	
				}				
				BoundaryData boundaryData;
				boundaryData.name = buffer;
				boundaryData.quadrangleConnectivity = std::move(quadrangleConnectivity);
				this->gridData->boundaries.emplace_back(std::move(boundaryData));
				break; 
			}
			default:
				throw std::runtime_error("CgnsReader3D: Non supported element found");
		}
	}
}

void CgnsReader3D::readBoundaries() {
	for (auto boundary = this->gridData->boundaries.begin(); boundary != this->gridData->boundaries.end(); boundary++) {
		std::set<int> vertices;
		if (boundary->triangleConnectivity.size() > 0) {
			for (auto j = boundary->triangleConnectivity.cbegin(); j != boundary->triangleConnectivity.cend(); j++) {
				for (auto k = j->cbegin(); k != j->cend(); k++) {
					vertices.insert(static_cast<int>(*k));
				}
			}
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
		else {
			for (auto j = boundary->quadrangleConnectivity.cbegin(); j != boundary->quadrangleConnectivity.cend(); j++) {
				for (auto k = j->cbegin(); k != j->cend(); k++) {
					vertices.insert(static_cast<int>(*k));
				}
			}
			boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
		}
	}
}