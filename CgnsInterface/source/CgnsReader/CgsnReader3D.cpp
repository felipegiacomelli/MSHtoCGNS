#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <cgnslib.h>

CgnsReader3D::CgnsReader3D(const std::string& filePath) : CgnsReader(filePath) {
	this->readCoordinates();
	this->readSections();
	this->readBoundaries();
}

void CgnsReader3D::readCoordinates() {
	int one = 1;
	double coordinatesX[this->sizes[0]];
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateX", RealDouble, &one, &this->sizes[0], coordinatesX))
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateX");

	double coordinatesY[this->sizes[0]];
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateY", RealDouble, &one, &this->sizes[0], coordinatesY))
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateY");

	double coordinatesZ[this->sizes[0]];
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateZ", RealDouble, &one, &this->sizes[0], coordinatesZ))
		throw std::runtime_error("CgnsReader3D: Could not read CoordinateZ");

	this->gridData->coordinates.resize(this->sizes[0], std::vector<double>(3));
	for (int i = 0; i < this->sizes[0]; i++) {
		this->gridData->coordinates[i][0] = coordinatesX[i];
		this->gridData->coordinates[i][1] = coordinatesY[i];
		this->gridData->coordinates[i][2] = coordinatesZ[i];
	}
}

void CgnsReader3D::readSections() {
	for (int sectionIndex = 1; sectionIndex <= this->numberOfSections; sectionIndex++) {
		ElementType_t elementType;
		int elementStart, elementEnd;
		int lastBoundaryElement, parentFlag;
		if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &elementType, &elementStart, &elementEnd,
								&lastBoundaryElement, &parentFlag))
			throw std::runtime_error("CgnsReader3D: Could not read section");
		int numberOfElements = elementEnd - elementStart + 1;

		if (elementType == TETRA_4 || elementType == HEXA_8) {
			RegionData region;
			region.name = std::string(this->buffer);
			region.elementsOnRegion.resize(numberOfElements);
			std::iota(region.elementsOnRegion.begin(), region.elementsOnRegion.end(), elementStart - 1);
			this->gridData->regions.emplace_back(std::move(region));
		}
		else if (elementType == TRI_3 || elementType == QUAD_4) {
			BoundaryData boundary;
			boundary.name = this->buffer;
			boundary.facetsOnBoundary.resize(numberOfElements);
			std::iota(boundary.facetsOnBoundary.begin(), boundary.facetsOnBoundary.end(), elementStart - 1);
			this->gridData->boundaries.emplace_back(std::move(boundary));
		}
		else {
			throw std::runtime_error("CgnsReader3D: Section element type not supported");
		}

		int size;
		if (cg_ElementDataSize(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &size))
			throw std::runtime_error("CgnsReader3D: Could not read element data size");

		int connectivities[size];
		if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr))
			throw std::runtime_error("CgnsReader3D: Could not read section elements");

		int numberOfVertices;
		if (cg_npe(elementType, &numberOfVertices))
			throw std::runtime_error("CgnsReader3D: Could not read element number of vertices");

		switch (elementType) {
			case TETRA_4: {
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> tetrahedron(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++)
						tetrahedron[k] = connectivities[e*numberOfVertices+k] - 1;
					tetrahedron.emplace_back(elementStart - 1 + e);
					this->gridData->tetrahedronConnectivity.emplace_back(std::move(tetrahedron));
				}
				break;
			}
			case HEXA_8: {
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> hexahedron(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++)
						hexahedron[k] = connectivities[e*numberOfVertices+k] - 1;
					hexahedron.emplace_back(elementStart - 1 + e);
					this->gridData->hexahedronConnectivity.emplace_back(std::move(hexahedron));
				}
				break;
			}
			case TRI_3: {
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> triangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++)
						triangle[k] = connectivities[e*numberOfVertices+k] - 1;
					triangle.emplace_back(elementStart - 1 + e);
					this->gridData->triangleConnectivity.emplace_back(std::move(triangle));
				}
				break;
			}
			case QUAD_4: {
				for (int e = 0; e < numberOfElements; e++) {
					std::vector<int> quadrangle(numberOfVertices);
					for (int k = 0; k < numberOfVertices; k++)
						quadrangle[k] = connectivities[e*numberOfVertices+k] - 1;
					quadrangle.emplace_back(elementStart - 1 + e);
					this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));
				}
				break;
			}
			default:
				throw std::runtime_error("CgnsReader3D: Non supported element found");
		}
	}
}