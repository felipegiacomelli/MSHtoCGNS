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
        int elementStart, end;
        int lastBoundaryElement, parentFlag;
        if (cg_section_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, this->buffer, &elementType, &elementStart, &end, &lastBoundaryElement, &parentFlag))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section");

        int size;
        if (cg_ElementDataSize(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &size))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element data size");

        std::vector<int> connectivities(size);
        if (cg_elements_read(this->fileIndex, this->baseIndex, this->zoneIndex, sectionIndex, &connectivities[0], nullptr))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read section elements");

        if (elementType == MIXED)
            if (ElementType_t(connectivities[0]) == TETRA_4 || ElementType_t(connectivities[0]) == HEXA_8 || ElementType_t(connectivities[0]) == PENTA_6 || ElementType_t(connectivities[0]) == PYRA_5)
                this->addRegion(std::string(this->buffer), elementStart - 1, end);
            else
                this->addBoundary(std::string(this->buffer), elementStart - 1, end);
        else if (elementType == TETRA_4 || elementType == HEXA_8 || elementType == PENTA_6 || elementType == PYRA_5)
                this->addRegion(std::string(this->buffer), elementStart - 1, end);
        else if (elementType == TRI_3 || elementType == QUAD_4)
            this->addBoundary(std::string(this->buffer), elementStart - 1, end);
        else if (elementType == BAR_2)
            this->addWell(std::string(this->buffer), elementStart - 1, end);

        int numberOfVertices;
        if (cg_npe(elementType, &numberOfVertices))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read element number of vertices");

        int numberOfElements = end - elementStart + 1;

        switch (elementType) {
            case MIXED : {
                int position = 0;
                for (int e = 0; e < numberOfElements; e++) {
                    cg_npe(ElementType_t(connectivities[position]), &numberOfVertices);
                    std::vector<int> element(numberOfVertices);
                    for (int k = 0; k < numberOfVertices; ++k)
                        element[k] = connectivities[position+1+k] - 1;
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
                    for (int k = 0; k < numberOfVertices; k++)
                        this->gridData->tetrahedronConnectivity.back()[k] = connectivities[e * numberOfVertices + k] - 1;
                    this->gridData->tetrahedronConnectivity.back().back() = elementStart - 1 + e;
                }
                break;
            }
            case HEXA_8: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->hexahedronConnectivity.emplace_back(std::array<int, 9>());
                    for (int k = 0; k < numberOfVertices; k++)
                        this->gridData->hexahedronConnectivity.back()[k] = connectivities[e * numberOfVertices + k] - 1;
                    this->gridData->hexahedronConnectivity.back().back() = elementStart - 1 + e;
                }
                break;
            }
            case PENTA_6: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->prismConnectivity.emplace_back(std::array<int, 7>());
                    for (int k = 0; k < numberOfVertices; k++)
                        this->gridData->prismConnectivity.back()[k] = connectivities[e * numberOfVertices + k] - 1;
                    this->gridData->prismConnectivity.back().back() = elementStart - 1 + e;
                }
                break;
            }
           case PYRA_5: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->pyramidConnectivity.emplace_back(std::array<int, 6>());
                    for (int k = 0; k < numberOfVertices; k++)
                        this->gridData->pyramidConnectivity.back()[k] = connectivities[e * numberOfVertices + k] - 1;
                    this->gridData->pyramidConnectivity.back().back() = elementStart - 1 + e;
                }
                break;
            }
            case TRI_3: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->triangleConnectivity.emplace_back(std::array<int, 4>());
                    for (int k = 0; k < numberOfVertices; k++)
                        this->gridData->triangleConnectivity.back()[k] = connectivities[e * numberOfVertices + k] - 1;
                    this->gridData->triangleConnectivity.back().back() = elementStart - 1 + e;
                }
                break;
            }
            case QUAD_4: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->quadrangleConnectivity.emplace_back(std::array<int, 5>());
                    for (int k = 0; k < numberOfVertices; k++)
                        this->gridData->quadrangleConnectivity.back()[k] = connectivities[e * numberOfVertices + k] - 1;
                    this->gridData->quadrangleConnectivity.back().back() = elementStart - 1 + e;
                }
                break;
            }
            case BAR_2: {
                for (int e = 0; e < numberOfElements; e++) {
                    this->gridData->lineConnectivity.emplace_back(std::array<int, 3>());
                    for (int k = 0; k < numberOfVertices; k++)
                        this->gridData->lineConnectivity.back()[k] = connectivities[e * numberOfVertices + k] - 1;
                    this->gridData->lineConnectivity.back().back() = elementStart - 1 + e;
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
    for (auto& boundary : this->gridData->boundaries) {
        std::set<int> vertices;

        for (const auto& triangle : this->gridData->triangleConnectivity)
            if (triangle.back() >= boundary.begin && triangle.back() < boundary.end)
                vertices.insert(triangle.cbegin(), triangle.cend() - 1);

        for (const auto& quadrangle : this->gridData->quadrangleConnectivity)
            if (quadrangle.back() >= boundary.begin && quadrangle.back() < boundary.end)
                vertices.insert(quadrangle.cbegin(), quadrangle.cend() - 1);

        boundary.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}

void CgnsReader3D::findRegionVertices() {
    for (auto& region : this->gridData->regions) {
        std::set<int> vertices;

        for (const auto& tetrahedron : this->gridData->tetrahedronConnectivity)
            if (tetrahedron.back() >= region.begin && tetrahedron.back() < region.end)
                vertices.insert(tetrahedron.cbegin(), tetrahedron.cend() - 1);

        for (const auto& hexahedron : this->gridData->hexahedronConnectivity)
            if (hexahedron.back() >= region.begin && hexahedron.back() < region.end)
                vertices.insert(hexahedron.cbegin(), hexahedron.cend() - 1);

        for (const auto& prism : this->gridData->prismConnectivity)
            if (prism.back() >= region.begin && prism.back() < region.end)
                vertices.insert(prism.cbegin(), prism.cend() - 1);

        for (const auto& pyramid : this->gridData->pyramidConnectivity)
            if (pyramid.back() >= region.begin && pyramid.back() < region.end)
                vertices.insert(pyramid.cbegin(), pyramid.cend() - 1);

        region.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}

void CgnsReader3D::findWellVertices() {
    for (auto& well : this->gridData->wells) {
        std::set<int> vertices;

        for (const auto& line : this->gridData->lineConnectivity)
            if (line.back() >= well.begin && line.back() < well.end)
                vertices.insert(line.cbegin(), line.cend() - 1);

        well.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}
