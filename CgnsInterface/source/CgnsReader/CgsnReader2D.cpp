#include <CgnsInterface/CgnsReader/CgnsReader2D.hpp>
#include <cgnslib.h>

CgnsReader2D::CgnsReader2D(const std::string& filePath) : CgnsReader(filePath) {
	this->readCoordinates();
	this->readSections();
	this->readBoundaries();
}

void CgnsReader2D::readCoordinates() {
	int one = 1;

	double coordinatesX[this->sizes[0]];
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateX", RealDouble, &one, &this->sizes[0], coordinatesX))
		throw std::runtime_error("CgnsReader2D: Could not read CoordinateX");

	double coordinatesY[this->sizes[0]];
	if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateY", RealDouble, &one, &this->sizes[0], coordinatesY))
		throw std::runtime_error("CgnsReader2D: Could not read CoordinateY");

	this->gridData->coordinates.resize(this->sizes[0], std::array<double, 3>());
	for (int i = 0; i < this->sizes[0]; i++) {
		this->gridData->coordinates[i][0] = coordinatesX[i];
		this->gridData->coordinates[i][1] = coordinatesY[i];
	}
}

void CgnsReader2D::readSections() {
	for (int sectionIndex = 1; sectionIndex <= this->numberOfSections; sectionIndex++) {
		ElementType_t elementType;
		int elementStart, elementEnd;
		int lastBoundaryElement, parentFlag;
		if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &elementType, &elementStart, &elementEnd,
								&lastBoundaryElement, &parentFlag))
			throw std::runtime_error("CgnsReader2D: Could not read section");
		int numberOfElements = elementEnd - elementStart + 1;

		if (elementType == MIXED || elementType == TRI_3 || elementType == QUAD_4) {
			RegionData region;
			region.name = std::string(this->buffer);
			region.elementsOnRegion.resize(numberOfElements);
			std::iota(region.elementsOnRegion.begin(), region.elementsOnRegion.end(), elementStart - 1);
			this->gridData->regions.emplace_back(std::move(region));
		}
		else if (elementType == BAR_2) {
			BoundaryData boundary;
			boundary.name = this->buffer;
			boundary.facetsOnBoundary.resize(numberOfElements);
			std::iota(boundary.facetsOnBoundary.begin(), boundary.facetsOnBoundary.end(), elementStart - 1);
			this->gridData->boundaries.emplace_back(std::move(boundary));
		}
		else {
			throw std::runtime_error("CgnsReader2D: Section element type not supported");
		}

		int size;
		if (cg_ElementDataSize(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &size))
			throw std::runtime_error("CgnsReader2D: Could not read element data size");

		int connectivities[size];
		if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, connectivities, nullptr))
			throw std::runtime_error("CgnsReader2D: Could not read section elements");

		int numberOfVertices;
		if (cg_npe(elementType, &numberOfVertices))
			throw std::runtime_error("CgnsReader2D: Could not read element number of vertices");

		switch (elementType) {
			case MIXED : {
				int position = 0;
				for (int e = 0; e < numberOfElements; e++) {
					cg_npe(ElementType_t(connectivities[position]), &numberOfVertices);
					std::vector<int> element(numberOfVertices);
					for (int k = 0; k < numberOfVertices; ++k)
						element[k] = connectivities[position+1+k] - 1;
					element.emplace_back(elementStart - 1 + e);
					switch(connectivities[position]) {
						case TRI_3: {
							std::array<int, 4> triangle;
							std::copy_n(std::begin(element), 4, std::begin(triangle));
							this->gridData->triangleConnectivity.emplace_back(std::move(triangle));
							break;
						}
						case QUAD_4: {
							std::array<int, 5> quadrangle;
							std::copy_n(std::begin(element), 5, std::begin(quadrangle));
							this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));
							break;
						}
						default:
							throw std::runtime_error("CgnsReader2D: Element type in MIXED section not supported");
					}
					position += numberOfVertices + 1;
				}
				break;
			}
			case TRI_3: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 4> triangle;
					for (int k = 0; k < numberOfVertices; k++)
						triangle[k] = connectivities[e*numberOfVertices+k] - 1;
					triangle.back() = elementStart - 1 + e;
					this->gridData->triangleConnectivity.emplace_back(std::move(triangle));
				}
				break;
			}
			case QUAD_4: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 5> quadrangle;
					for (int k = 0; k < numberOfVertices; k++)
						quadrangle[k] = connectivities[e*numberOfVertices+k] - 1;
					quadrangle.back() = (elementStart - 1 + e);
					this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));
				}
				break;
			}
			case BAR_2: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 3> line;
					for (int k = 0; k < numberOfVertices; k++)
						line[k] = connectivities[e*numberOfVertices+k] - 1;
					line.back() = (elementStart - 1 + e);
					this->gridData->lineConnectivity.emplace_back(std::move(line));
				}
				break;
			}
			default:
				throw std::runtime_error("CgnsReader2D: Section element type not supported");
		}
	}
}