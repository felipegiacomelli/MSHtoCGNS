#include <IO/CgnsReader2D.hpp>
 
CgnsReader2D::CgnsReader2D(const std::string& filePath) :
	CgnsReader(filePath) {
	this->gridData->dimension = this->cellDimension;
	this->gridData->thickness = 1.0;
	this->readCoordinates();
	this->readSections();
	this->readBoundaries();
}

void CgnsReader2D::readCoordinates() {
	this->numberOfNodes = this->zoneSizes[0];
	cgsize_t one = 1;
	double coordinatesX[this->numberOfNodes];
	double coordinatesY[this->numberOfNodes];
	if (cg_coord_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, "CoordinateX", RealDouble, &one, &this->numberOfNodes, coordinatesX)) {
		throw std::runtime_error("CgnsReader2D: Could not read CoordinateX");
	}
	if (cg_coord_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, "CoordinateY", RealDouble, &one, &this->numberOfNodes, coordinatesY)) {
		throw std::runtime_error("CgnsReader2D: Could not read CoordinateY");
	}
		
	this->gridData->coordinates.resize(this->numberOfNodes, std::vector<double>(3));
	for (int i = 0; i < this->numberOfNodes; i++) {
		this->gridData->coordinates[i][0] = coordinatesX[i]; 
		this->gridData->coordinates[i][1] = coordinatesY[i]; 
		this->gridData->coordinates[i][2] = 0.0; 
	}
}

void CgnsReader2D::readSections() {
	for (auto section = this->sectionIndices.cbegin(); section != this->sectionIndices.cend(); section++) {
		ElementType_t type;
		cgsize_t elementStart, elementEnd; 
		int nBdry, parentFlag;
		if (cg_section_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, this->buffer, &type, &elementStart, &elementEnd, &nBdry, &parentFlag)) {
			throw std::runtime_error("CgnsReader2D: Could not read section");
		}
		int numberOfElements = elementEnd - elementStart + 1;
		
		switch (type) {
			case TRI_3: {
				int numberOfVertices = 3;	
				cgsize_t connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader2D: Could not read section elements");
				}
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> triangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) triangle[k] = connectivities[e*numberOfVertices+k] - 1;
					this->gridData->triangleConnectivity.emplace_back(std::move(triangle));	
				}
				RegionData region;
				region.name = std::string(this->buffer);
				region.elementType = 1;
				region.elementsOnRegion.resize(numberOfElements);
				std::iota(region.elementsOnRegion.begin(), region.elementsOnRegion.end(), elementStart - 1);
				this->gridData->regions.emplace_back(std::move(region));
				break; 
			}
			case QUAD_4: {
				int numberOfVertices = 4;	
				cgsize_t connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, connectivities, nullptr)) {
					throw std::runtime_error("CgnsReader2D: Could not read section elements");
				}
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> quadrangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++) quadrangle[k] = connectivities[e*numberOfVertices+k]-1;
					this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));	
				}
				RegionData region;
				region.name = std::string(this->buffer);
				region.elementType = 2;
				region.elementsOnRegion.resize(numberOfElements);
				std::iota(region.elementsOnRegion.begin(), region.elementsOnRegion.end(), elementStart - 1);
				this->gridData->regions.emplace_back(std::move(region));
				break; 
			}
			case BAR_2: {
				int numberOfVertices = 2;	
				cgsize_t connectivities[numberOfVertices*numberOfElements];
				if (cg_elements_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *section, connectivities, nullptr)) {
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

void CgnsReader2D::readBoundaries() {
	if (this->boundaryIndices.size() != this->gridData->boundaries.size()) {
		throw std::runtime_error("CgnsReader2D: mismatch between number of boundary conditions and boundary connectivities");
	}
	for (auto boundary = this->boundaryIndices.cbegin(); boundary != this->boundaryIndices.cend(); boundary++) {
		BCType_t bocotype;
		PointSetType_t ptset_type;
		cgsize_t npnts, NormalListSize;
		int NormalIndex, ndataset;
		DataType_t NormalDataType;
		if (cg_boco_info(this->cgnsFile, this->cgnsBase, this->cgnsZone, *boundary, this->buffer, &bocotype, &ptset_type, &npnts, &NormalIndex, &NormalListSize, &NormalDataType, &ndataset)) {
			throw std::runtime_error("CgnsReader2D: Could not read boundary information");
		}
		std::vector<cgsize_t> pnts(npnts);
		if (cg_boco_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, *boundary, &pnts[0], nullptr)) {
			throw std::runtime_error("CgnsReader2D: Could not read boundary");
		}
		std::transform(pnts.begin(), pnts.end(), pnts.begin(), [](const cgsize_t& x){return x - 1;});
		this->gridData->boundaries[*boundary - 1].vertices = std::move(pnts);
	}
}