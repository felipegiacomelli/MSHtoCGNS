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
    std::vector<double> coordinatesX(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateX", RealDouble, &this->one, this->sizes, &coordinatesX[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateX");

    std::vector<double> coordinatesY(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateY", RealDouble, &this->one, this->sizes, &coordinatesY[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateY");

    std::vector<double> coordinatesZ(this->sizes[0]);
    if (cg_coord_read(this->fileIndex, this->baseIndex, this->zoneIndex, "CoordinateZ", RealDouble, &this->one, this->sizes, &coordinatesZ[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read CoordinateZ");

    this->gridData->coordinates.resize(this->sizes[0], std::array<double, 3>());
    for (int i = 0; i < this->sizes[0]; ++i) {
        this->gridData->coordinates[i][0] = coordinatesX[i];
        this->gridData->coordinates[i][1] = coordinatesY[i];
        this->gridData->coordinates[i][2] = coordinatesZ[i];
    }
}

bool CgnsReader3D::skipSection() {
    return false;
}

void CgnsReader3D::addEntity(int elementType) {
    if (elementType == TETRA_4 || elementType == HEXA_8 || elementType == PENTA_6 || elementType == PYRA_5)
            this->addRegion(std::string(this->buffer), this->elementStart - 1, this->elementEnd);
    else if (elementType == TRI_3 || elementType == QUAD_4)
        this->addBoundary(std::string(this->buffer), this->elementStart - 1, this->elementEnd);
    else if (elementType == BAR_2)
        this->addWell(std::string(this->buffer), this->elementStart - 1, this->elementEnd);
}


void CgnsReader3D::addWell(std::string&& name, int start, int end) {
    this->gridData->wells.emplace_back(WellData());
    std::transform(name.begin(), name.end(), std::back_inserter(this->gridData->wells.back().name), ::toupper);
    this->gridData->wells.back().begin = start;
    this->gridData->wells.back().end = end;
}

void CgnsReader3D::findBoundaryVertices() {
    std::vector<std::set<int>> vertices(this->gridData->boundaries.size());

    this->findVertices(this->gridData->triangleConnectivity, this->gridData->boundaries, vertices);
    this->findVertices(this->gridData->quadrangleConnectivity, this->gridData->boundaries, vertices);

    for (unsigned b = 0u; b < this->gridData->boundaries.size(); ++b)
        this->gridData->boundaries[b].vertices = std::vector<int>(vertices[b].begin(), vertices[b].end());
}

void CgnsReader3D::findRegionVertices() {
    std::vector<std::set<int>> vertices(this->gridData->regions.size());

    this->findVertices(this->gridData->tetrahedronConnectivity, this->gridData->regions, vertices);
    this->findVertices(this->gridData->hexahedronConnectivity, this->gridData->regions, vertices);
    this->findVertices(this->gridData->prismConnectivity, this->gridData->regions, vertices);
    this->findVertices(this->gridData->pyramidConnectivity, this->gridData->regions, vertices);

    for (unsigned r = 0u; r < this->gridData->regions.size(); ++r)
        this->gridData->regions[r].vertices = std::vector<int>(vertices[r].begin(), vertices[r].end());
}

void CgnsReader3D::findWellVertices() {
    std::vector<std::set<int>> vertices(this->gridData->wells.size());

    this->findVertices(this->gridData->lineConnectivity, this->gridData->wells, vertices);

    for (unsigned w = 0u; w < this->gridData->wells.size(); ++w)
        this->gridData->wells[w].vertices = std::vector<int>(vertices[w].begin(), vertices[w].end());
}
