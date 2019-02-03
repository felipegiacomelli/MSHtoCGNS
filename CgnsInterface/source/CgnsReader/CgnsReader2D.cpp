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

bool CgnsReader2D::skipSection() {
    return false;
}

void CgnsReader2D::addEntity(int elementType) {
    if (elementType == TRI_3 || elementType == QUAD_4)
        this->addRegion(std::string(this->buffer), this->elementStart - 1, this->elementEnd);
    else if (elementType == BAR_2)
        this->addBoundary(std::string(this->buffer), this->elementStart - 1, this->elementEnd);
}

void CgnsReader2D::findBoundaryVertices() {
    std::vector<std::set<int>> vertices(this->gridData->boundaries.size());

    this->findVertices(this->gridData->lineConnectivity, this->gridData->boundaries, vertices);

    for (unsigned b = 0u; b < this->gridData->boundaries.size(); ++b)
        this->gridData->boundaries[b].vertices = std::vector<int>(vertices[b].begin(), vertices[b].end());
}

void CgnsReader2D::findRegionVertices() {
    std::vector<std::set<int>> vertices(this->gridData->regions.size());

    this->findVertices(this->gridData->triangleConnectivity, this->gridData->regions, vertices);
    this->findVertices(this->gridData->quadrangleConnectivity, this->gridData->regions, vertices);

    for (unsigned r = 0u; r < this->gridData->regions.size(); ++r)
        this->gridData->regions[r].vertices = std::vector<int>(vertices[r].begin(), vertices[r].end());
}
