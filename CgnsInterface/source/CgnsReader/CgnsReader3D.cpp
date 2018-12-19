#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"
#include <cgnslib.h>

CgnsReader3D::CgnsReader3D(std::string filePath, bool readInConstructor) : CgnsReader(filePath) {
    if (readInConstructor) {
        this->readCoordinates();
        this->readSections();
        this->findBoundaryVertices();
        this->findRegionVertices();
        this->findWellVertices();
    }
}

void CgnsReader3D::readCoordinates() {
    int one = 1;

    std::vector<double> coordinatesX(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateX", RealDouble, &one, this->sizes, &coordinatesX[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateX");

    std::vector<double> coordinatesY(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateY", RealDouble, &one, this->sizes, &coordinatesY[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateY");

    std::vector<double> coordinatesZ(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateZ", RealDouble, &one, this->sizes, &coordinatesZ[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateZ");

    this->gridData->coordinates.resize(this->sizes[0], std::array<double, 3>());
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
        if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &elementType, &elementStart, &elementEnd, &lastBoundaryElement, &parentFlag))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section");

        int size;
        if (cg_ElementDataSize(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &size))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element data size");

        std::vector<int> connectivities(size);
        if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], nullptr))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section elements");

        if (elementType == MIXED)
            if (ElementType_t(connectivities[0]) == TETRA_4 || ElementType_t(connectivities[0]) == HEXA_8 || ElementType_t(connectivities[0]) == PENTA_6 || ElementType_t(connectivities[0]) == PYRA_5)
                this->addRegion(std::string(this->buffer), elementStart - 1, elementEnd);
            else
                this->addBoundary(std::string(this->buffer), elementStart - 1, elementEnd);
        else if (elementType == TETRA_4 || elementType == HEXA_8 || elementType == PENTA_6 || elementType == PYRA_5)
                this->addRegion(std::string(this->buffer), elementStart - 1, elementEnd);
        else if (elementType == TRI_3 || elementType == QUAD_4)
            this->addBoundary(std::string(this->buffer), elementStart - 1, elementEnd);
        else if (elementType == BAR_2)
            this->addWell(std::string(this->buffer), elementStart - 1, elementEnd);

        int numberOfVertices;
        if (cg_npe(elementType, &numberOfVertices))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element number of vertices");

        int numberOfElements = elementEnd - elementStart + 1;

        switch (elementType) {
            case MIXED : {
                int position = 0;
                for (int e = 0; e < numberOfElements; e++) {
                    cg_npe(ElementType_t(connectivities[position]), &numberOfVertices);
                    std::vector<int> element(numberOfVertices);
                    for (int k = 0; k < numberOfVertices; ++k)
                        element[k] = connectivities[position + 1 + k] - 1;
                    element.emplace_back(elementStart - 1 + e);
                    switch (connectivities[position]) {
                        case TETRA_4: {
                            this->gridData->tetrahedronConnectivity.emplace_back(std::array<int, 5>());
                            std::copy_n(std::begin(element), 5, std::begin(this->gridData->tetrahedronConnectivity.back()));
                            break;
                        }
                        case HEXA_8: {
                            this->gridData->hexahedronConnectivity.emplace_back(std::array<int, 9>());
                            std::copy_n(std::begin(element), 9, std::begin(this->gridData->hexahedronConnectivity.back()));
                            break;
                        }
                        case PENTA_6: {
                            this->gridData->prismConnectivity.emplace_back(std::array<int, 7>());
                            std::copy_n(std::begin(element), 7, std::begin(this->gridData->prismConnectivity.back()));
                            break;
                        }
                        case PYRA_5: {
                            this->gridData->pyramidConnectivity.emplace_back(std::array<int, 6>());
                            std::copy_n(std::begin(element), 6, std::begin(this->gridData->pyramidConnectivity.back()));
                            break;
                        }
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
            case TETRA_4: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->tetrahedronConnectivity.emplace_back(std::array<int, 5>());
                    auto& tetrahedron = this->gridData->tetrahedronConnectivity.back();
                    for (int k = 0; k < numberOfVertices; k++)
                        tetrahedron[k] = connectivities[e * numberOfVertices + k] - 1;
                    tetrahedron.back() = elementStart - 1 + e;
                }
                break;
            }
            case HEXA_8: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->hexahedronConnectivity.emplace_back(std::array<int, 9>());
                    auto& hexahedron = this->gridData->hexahedronConnectivity.back();
                    for (int k = 0; k < numberOfVertices; k++)
                        hexahedron[k] = connectivities[e * numberOfVertices + k] - 1;
                    hexahedron.back() = elementStart - 1 + e;
                }
                break;
            }
            case PENTA_6: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->prismConnectivity.emplace_back(std::array<int, 7>());
                    auto& prism = this->gridData->prismConnectivity.back();
                    for (int k = 0; k < numberOfVertices; k++)
                        prism[k] = connectivities[e * numberOfVertices + k] - 1;
                    prism.back() = elementStart - 1 + e;
                }
                break;
            }
           case PYRA_5: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->pyramidConnectivity.emplace_back(std::array<int, 6>());
                    auto& pyramid = this->gridData->pyramidConnectivity.back();
                    for (int k = 0; k < numberOfVertices; k++)
                        pyramid[k] = connectivities[e * numberOfVertices + k] - 1;
                    pyramid.back() = elementStart - 1 + e;
                }
                break;
            }
            case TRI_3: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->triangleConnectivity.emplace_back(std::array<int, 4>());
                    auto& triangle = this->gridData->triangleConnectivity.back();
                    for (int k = 0; k < numberOfVertices; k++)
                        triangle[k] = connectivities[e * numberOfVertices + k] - 1;
                    triangle.back() = elementStart - 1 + e;
                }
                break;
            }
            case QUAD_4: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->quadrangleConnectivity.emplace_back(std::array<int, 5>());
                    auto& quadrangle = this->gridData->quadrangleConnectivity.back();
                    for (int k = 0; k < numberOfVertices; k++)
                        quadrangle[k] = connectivities[e * numberOfVertices + k] - 1;
                    quadrangle.back() = elementStart - 1 + e;
                }
                break;
            }
            case BAR_2: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->lineConnectivity.emplace_back(std::array<int, 3>());
                    auto& line = this->gridData->lineConnectivity.back();
                    for (int k = 0; k < numberOfVertices; k++)
                        line[k] = connectivities[e * numberOfVertices + k] - 1;
                    line.back() = elementStart - 1 + e;
                }
                break;
            }
            default:
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Section " + std::string(this->buffer) + " element type " + std::to_string(elementType) + " not supported");
        }
    }
}

void CgnsReader3D::addWell(std::string&& name, int start, int end) {
    this->gridData->wells.emplace_back(WellData());
    std::transform(name.begin(), name.end(), std::back_inserter(this->gridData->wells.back().name), ::toupper);
    this->gridData->wells.back().begin = start;
    this->gridData->wells.back().end = end;
}

void CgnsReader3D::findBoundaryVertices() {
    int numberOfBoundaries = int(this->gridData->boundaries.size());
    std::vector<std::set<int>> vertices(numberOfBoundaries);

    for (const auto& triangle : this->gridData->triangleConnectivity)
            for (int b = 0; b < numberOfBoundaries; b++)
                if (triangle.back() >= this->gridData->boundaries[b].begin && triangle.back() < this->gridData->boundaries[b].end) {
                    vertices[b].insert(triangle.cbegin(), triangle.cend() - 1);
                    break;
                }

    for (const auto& quadrangle : this->gridData->quadrangleConnectivity)
            for (int b = 0; b < numberOfBoundaries; b++)
                if (quadrangle.back() >= this->gridData->boundaries[b].begin && quadrangle.back() < this->gridData->boundaries[b].end) {
                    vertices[b].insert(quadrangle.cbegin(), quadrangle.cend() - 1);
                    break;
                }

    for (int b = 0; b < numberOfBoundaries; b++)
        this->gridData->boundaries[b].vertices = std::vector<int>(vertices[b].begin(), vertices[b].end());
}

void CgnsReader3D::findRegionVertices() {
    int numberOfRegions = int(this->gridData->regions.size());
    std::vector<std::set<int>> vertices(numberOfRegions);

    for (const auto& tetrahedron : this->gridData->tetrahedronConnectivity)
            for (int r = 0; r < numberOfRegions; r++)
                if (tetrahedron.back() >= this->gridData->regions[r].begin && tetrahedron.back() < this->gridData->regions[r].end) {
                    vertices[r].insert(tetrahedron.cbegin(), tetrahedron.cend() - 1);
                    break;
                }

    for (const auto& hexahedron : this->gridData->hexahedronConnectivity)
            for (int r = 0; r < numberOfRegions; r++)
                if (hexahedron.back() >= this->gridData->regions[r].begin && hexahedron.back() < this->gridData->regions[r].end) {
                    vertices[r].insert(hexahedron.cbegin(), hexahedron.cend() - 1);
                    break;
                }

    for (const auto& prism : this->gridData->prismConnectivity)
            for (int r = 0; r < numberOfRegions; r++)
                if (prism.back() >= this->gridData->regions[r].begin && prism.back() < this->gridData->regions[r].end) {
                    vertices[r].insert(prism.cbegin(), prism.cend() - 1);
                    break;
                }

    for (const auto& pyramid : this->gridData->pyramidConnectivity)
            for (int r = 0; r < numberOfRegions; r++)
                if (pyramid.back() >= this->gridData->regions[r].begin && pyramid.back() < this->gridData->regions[r].end) {
                    vertices[r].insert(pyramid.cbegin(), pyramid.cend() - 1);
                    break;
                }

    for (int r = 0; r < numberOfRegions; r++)
        this->gridData->regions[r].vertices = std::vector<int>(vertices[r].begin(), vertices[r].end());
}

void CgnsReader3D::findWellVertices() {
    int numberOfWells = int(this->gridData->wells.size());
    std::vector<std::set<int>> vertices(numberOfWells);

    for (const auto& line : this->gridData->lineConnectivity)
            for (int w = 0; w < numberOfWells; w++)
                if (line.back() >= this->gridData->wells[w].begin && line.back() < this->gridData->wells[w].end) {
                    vertices[w].insert(line.cbegin(), line.cend() - 1);
                    break;
                }

    for (int w = 0; w < numberOfWells; w++)
        this->gridData->wells[w].vertices = std::vector<int>(vertices[w].begin(), vertices[w].end());
}
