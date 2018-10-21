#include <FileMend/CgnsReader/SpecialCgnsReader3D.hpp>
#include <cgnslib.h>

SpecialCgnsReader3D::SpecialCgnsReader3D(std::string filePath) : CgnsReader3D(filePath, false) {
	this->readCoordinates();
	this->readSections();
	this->readBoundaries();
}

void SpecialCgnsReader3D::readSections() {
	for (int sectionIndex = 1; sectionIndex <= this->numberOfSections; sectionIndex++) {
		ElementType_t elementType;
		int elementStart, elementEnd;
		int lastBoundaryElement, parentFlag;
		if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &elementType, &elementStart, &elementEnd, &lastBoundaryElement, &parentFlag))
			throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section");

		std::string sectionName(this->buffer);
		if (sectionName.substr(sectionName.length() - 3) == "_1D")
			continue;
		if (sectionName.substr(sectionName.length() - 3) == "_0D")
			continue;
		if (elementType == BAR_2)
			continue;

		int numberOfElements = elementEnd - elementStart + 1;
		elementStart = this->gridData->tetrahedronConnectivity.size() + this->gridData->hexahedronConnectivity.size() + this->gridData->prismConnectivity.size() + this->gridData->pyramidConnectivity.size() + this->gridData->triangleConnectivity.size() + this->gridData->quadrangleConnectivity.size();
		elementEnd = elementStart + numberOfElements;

		int size;
		if (cg_ElementDataSize(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &size))
			throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element data size");

		std::vector<int> connectivities(size);
		if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], nullptr))
			throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section elements");

		if (elementType == MIXED)
			if (ElementType_t(connectivities[0]) == TETRA_4 || ElementType_t(connectivities[0]) == HEXA_8 || ElementType_t(connectivities[0]) == PENTA_6 || ElementType_t(connectivities[0]) == PYRA_5)
				this->addRegion(std::string(this->buffer), elementStart, elementEnd);
			else
				this->addBoundary(std::string(this->buffer), elementStart, elementEnd);
		else if (elementType == TETRA_4 || elementType == HEXA_8 || elementType == PENTA_6 || elementType == PYRA_5)
				this->addRegion(std::string(this->buffer), elementStart, elementEnd);
		else if (elementType == TRI_3 || elementType == QUAD_4)
			this->addBoundary(std::string(this->buffer), elementStart, elementEnd);

		int numberOfVertices;
		if (cg_npe(elementType, &numberOfVertices))
			throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element number of vertices");

		switch (elementType) {
			case MIXED : {
				int position = 0;
				for (int e = 0; e < numberOfElements; e++) {
					cg_npe(ElementType_t(connectivities[position]), &numberOfVertices);
					std::vector<int> element(numberOfVertices);
					for (int k = 0; k < numberOfVertices; ++k)
						element[k] = connectivities[position+1+k] - 1;
					element.emplace_back(elementStart + e);
					switch (connectivities[position]) {
						case TETRA_4: {
							std::array<int, 5> tetrahedron;
							std::copy_n(std::begin(element), 5, std::begin(tetrahedron));
							this->gridData->tetrahedronConnectivity.emplace_back(std::move(tetrahedron));
							break;
						}
						case HEXA_8: {
							std::array<int, 9> hexahedron;
							std::copy_n(std::begin(element), 9, std::begin(hexahedron));
							this->gridData->hexahedronConnectivity.emplace_back(std::move(hexahedron));
							break;
						}
						case PENTA_6: {
							std::array<int, 7> prism;
							std::copy_n(std::begin(element), 7, std::begin(prism));
							this->gridData->prismConnectivity.emplace_back(std::move(prism));
							break;
						}
						case PYRA_5: {
							std::array<int, 6> pyramid;
							std::copy_n(std::begin(element), 6, std::begin(pyramid));
							this->gridData->pyramidConnectivity.emplace_back(std::move(pyramid));
							break;
						}
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
					}
					position += numberOfVertices + 1;
				}
				break;
			}
			case TETRA_4: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 5> tetrahedron;
					for (int k = 0; k < numberOfVertices; k++)
						tetrahedron[k] = connectivities[e*numberOfVertices+k] - 1;
					tetrahedron.back() = (elementStart + e);
					this->gridData->tetrahedronConnectivity.emplace_back(std::move(tetrahedron));
				}
				break;
			}
			case HEXA_8: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 9> hexahedron;
					for (int k = 0; k < numberOfVertices; k++)
						hexahedron[k] = connectivities[e*numberOfVertices+k] - 1;
					hexahedron.back() = (elementStart + e);
					this->gridData->hexahedronConnectivity.emplace_back(std::move(hexahedron));
				}
				break;
			}
			case PENTA_6: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 7> prism;
					for (int k = 0; k < numberOfVertices; k++)
						prism[k] = connectivities[e*numberOfVertices+k] - 1;
					prism.back() = (elementStart + e);
					this->gridData->prismConnectivity.emplace_back(std::move(prism));
				}
				break;
			}
			case PYRA_5: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 6> pyramid;
					for (int k = 0; k < numberOfVertices; k++)
						pyramid[k] = connectivities[e*numberOfVertices+k] - 1;
					pyramid.back() = (elementStart + e);
					this->gridData->pyramidConnectivity.emplace_back(std::move(pyramid));
				}
				break;
			}
			case TRI_3: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 4> triangle;
					for (int k = 0; k < numberOfVertices; k++)
						triangle[k] = connectivities[e*numberOfVertices+k] - 1;
					triangle.back() = (elementStart + e);
					this->gridData->triangleConnectivity.emplace_back(std::move(triangle));
				}
				break;
			}
			case QUAD_4: {
				for (int e = 0; e < numberOfElements; e++) {
					std::array<int, 5> quadrangle;
					for (int k = 0; k < numberOfVertices; k++)
						quadrangle[k] = connectivities[e*numberOfVertices+k] - 1;
					quadrangle.back() = (elementStart + e);
					this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));
				}
				break;
			}
			default:
				throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Section " + std::string(this->buffer) + " element type " + std::to_string(elementType) + " not supported");
		}
	}
}

void SpecialCgnsReader3D::readBoundaries() {
	for (int boundaryIndex = 1; boundaryIndex <= this->numberOfBoundaries; boundaryIndex++) {
		BCType_t boundaryConditionType;
		PointSetType_t pointSetType;
		int numberOfVertices, NormalIndex, NormalListSize, ndataset;
		DataType_t NormalDataType;
		if (cg_boco_info(this->fileIndex, this->baseIndex, this->zoneIndex, boundaryIndex, this->buffer, &boundaryConditionType, &pointSetType, &numberOfVertices, &NormalIndex, &NormalListSize, &NormalDataType, &ndataset))
			throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read boundary information");

		if (cg_goto(this->fileIndex, this->baseIndex, "Zone_t", this->zoneIndex, "ZoneBC_t", 1, "BC_t", boundaryIndex, nullptr))
			throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could go to boundary condition " + std::to_string(boundaryIndex));

		if (cg_famname_read(this->buffer))
			throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read boundary condition " + std::to_string(boundaryIndex) + " family name");

		auto boundary = std::find_if(this->gridData->boundaries.begin(), this->gridData->boundaries.end(), [this](auto b){return b.name == std::string(this->buffer);});
		if (boundary != this->gridData->boundaries.end()) {
			std::vector<int> vertices(numberOfVertices);
			if (cg_boco_read(this->fileIndex, this->baseIndex, this->zoneIndex, boundaryIndex, &vertices[0], nullptr))
				throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read boundary " + std::to_string(boundaryIndex));

			std::transform(vertices.cbegin(), vertices.cend(), std::back_inserter(boundary->vertices), [](auto x){return x - 1;});
		}
	}
}