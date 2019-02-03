#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader2D.hpp"
#include <cgnslib.h>

CgnsReader2D::CgnsReader2D(std::string filePath, bool readInConstructor) : CgnsReader(filePath) {
    if (readInConstructor) {
        this->readCoordinates();
        this->readSections();
        this->findBoundaryVertices();
        this->findRegionVertices();
    }
}

void CgnsReader2D::readCoordinates() {
    std::vector<double> coordinatesX(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateX", RealDouble, &this->one, this->sizes, &coordinatesX[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateX");

    std::vector<double> coordinatesY(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateY", RealDouble, &this->one, this->sizes, &coordinatesY[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateY");

    this->gridData->coordinates.resize(this->sizes[0], std::array<double, 3>());
    for (int i = 0; i < this->sizes[0]; ++i) {
        this->gridData->coordinates[i][0] = coordinatesX[i];
        this->gridData->coordinates[i][1] = coordinatesY[i];
    }
}

void CgnsReader2D::readSections() {
    for (int sectionIndex = 1; sectionIndex <= this->numberOfSections; ++sectionIndex) {
        ElementType_t elementType;
        int elementStart, elementEnd;
        int lastBoundaryElement, parentFlag;
        if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &elementType, &elementStart, &elementEnd, &lastBoundaryElement, &parentFlag))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section");

        if (elementType == MIXED || elementType == TRI_3 || elementType == QUAD_4)
            this->addRegion(std::string(this->buffer), elementStart - 1, elementEnd);
        else if (elementType == BAR_2)
            this->addBoundary(std::string(this->buffer), elementStart - 1, elementEnd);

        int size;
        if (cg_ElementDataSize(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &size))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element data size");

        std::vector<int> connectivities(size);
        if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], nullptr))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section elements");

        int numberOfVertices;
        if (cg_npe(elementType, &numberOfVertices))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element number of vertices");

        int numberOfElements = elementEnd - elementStart + 1;

        switch (elementType) {
            case MIXED : {
                int position = 0;
                for (int e = 0; e < numberOfElements; ++e) {
                    cg_npe(ElementType_t(connectivities[position]), &numberOfVertices);
                    std::vector<int> element(numberOfVertices);
                    for (int k = 0; k < numberOfVertices; ++k)
                        element[k] = connectivities[position + 1 + k] - 1;
                    element.emplace_back(elementStart - 1 + e);
                    switch (connectivities[position]) {
                        case TRI_3: {
                            this->gridData->triangleConnectivity.emplace_back(std::array<int, 4>());
                            std::copy_n(std::begin(element), 4, std::begin(this->gridData->triangleConnectivity.back()));
                            break;
                        }
                        case QUAD_4: {
                            this->gridData->quadrangleConnectivity.emplace_back(std::array<int, 5>());
                            std::copy_n(std::begin(element), 5, std::begin(this->gridData->quadrangleConnectivity.back()));
                            break;
                        }
                    }
                    position += numberOfVertices + 1;
                }
                break;
            }
            case TRI_3: {
                for (int e = 0; e < numberOfElements; ++e) {
                    this->gridData->triangleConnectivity.emplace_back(std::array<int, 4>());
                    auto& triangle = this->gridData->triangleConnectivity.back();
                    for (int k = 0; k < numberOfVertices; ++k)
                        triangle[k] = connectivities[e * numberOfVertices + k] - 1;
                    triangle.back() = elementStart - 1 + e;
                }
                break;
            }
            case QUAD_4: {
                for (int e = 0; e < numberOfElements; ++e) {
                    this->gridData->quadrangleConnectivity.emplace_back(std::array<int, 5>());
                    auto& quadrangle = this->gridData->quadrangleConnectivity.back();
                    for (int k = 0; k < numberOfVertices; ++k)
                        quadrangle[k] = connectivities[e * numberOfVertices + k] - 1;
                    quadrangle.back() = elementStart - 1 + e;
                }
                break;
            }
            case BAR_2: {
                for (int e = 0; e < numberOfElements; ++e) {
                    this->gridData->lineConnectivity.emplace_back(std::array<int, 3>());
                    auto& line = this->gridData->lineConnectivity.back();
                    for (int k = 0; k < numberOfVertices; ++k)
                        line[k] = connectivities[e * numberOfVertices + k] - 1;
                    line.back() = elementStart - 1 + e;
                }
                break;
            }
            default:
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Section element type not supported");
        }
    }
}

void CgnsReader2D::findBoundaryVertices() {
    int numberOfBoundaries = int(this->gridData->boundaries.size());
    std::vector<std::set<int>> vertices(numberOfBoundaries);

    for (const auto& line : this->gridData->lineConnectivity)
            for (int b = 0; b < numberOfBoundaries; ++b)
                if (line.back() >= this->gridData->boundaries[b].begin && line.back() < this->gridData->boundaries[b].end) {
                    vertices[b].insert(line.cbegin(), line.cend() - 1);
                    break;
                }

    for (int b = 0; b < numberOfBoundaries; ++b)
        this->gridData->boundaries[b].vertices = std::vector<int>(vertices[b].begin(), vertices[b].end());
}

void CgnsReader2D::findRegionVertices() {
    int numberOfRegions = int(this->gridData->regions.size());
    std::vector<std::set<int>> vertices(numberOfRegions);

    for (const auto& triangle : this->gridData->triangleConnectivity)
            for (int r = 0; r < numberOfRegions; ++r)
                if (triangle.back() >= this->gridData->regions[r].begin && triangle.back() < this->gridData->regions[r].end) {
                    vertices[r].insert(triangle.cbegin(), triangle.cend() - 1);
                    break;
                }

    for (const auto& quadrangle : this->gridData->quadrangleConnectivity)
            for (int r = 0; r < numberOfRegions; ++r)
                if (quadrangle.back() >= this->gridData->regions[r].begin && quadrangle.back() < this->gridData->regions[r].end) {
                    vertices[r].insert(quadrangle.cbegin(), quadrangle.cend() - 1);
                    break;
                }

    for (int r = 0; r < numberOfRegions; ++r)
        this->gridData->regions[r].vertices = std::vector<int>(vertices[r].begin(), vertices[r].end());
}
