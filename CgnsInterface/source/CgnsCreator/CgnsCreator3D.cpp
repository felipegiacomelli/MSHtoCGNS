#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include <cgnslib.h>

CgnsCreator3D::CgnsCreator3D(boost::shared_ptr<GridData> gridData, std::string folderPath, bool createInConstructor) : CgnsCreator(gridData, folderPath) {
    if (createInConstructor) {
        this->checkDimension();
        this->setDimensions();
        this->setupFile();
        this->initialize();
    }
}

void CgnsCreator3D::checkDimension() {
    if (this->gridData->dimension != 3)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be equal to 3 and not " + std::to_string(this->gridData->dimension));
}

void CgnsCreator3D::setDimensions() {
    this->physicalDimension = this->gridData->dimension;
    this->cellDimension = this->gridData->dimension;
    this->sizes[0] = this->gridData->coordinates.size();
    this->sizes[1] = this->gridData->tetrahedrons.size() + this->gridData->hexahedrons.size() + this->gridData->prisms.size() + this->gridData->pyramids.size();
    this->sizes[2] = 0;
}

void CgnsCreator3D::writeCoordinates() {
    std::vector<double> coordinatesX(this->sizes[0]);
    std::vector<double> coordinatesY(this->sizes[0]);
    std::vector<double> coordinatesZ(this->sizes[0]);
    for (int i = 0; i < this->sizes[0]; ++i) {
        coordinatesX[i] = this->gridData->coordinates[i][0];
        coordinatesY[i] = this->gridData->coordinates[i][1];
        coordinatesZ[i] = this->gridData->coordinates[i][2];
    }

    if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", &coordinatesX[0], &this->coordinateIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write CoordinateX");

    if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", &coordinatesY[0], &this->coordinateIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write CoordinateY");

    if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateZ", &coordinatesZ[0], &this->coordinateIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write CoordinateZ");
}

void CgnsCreator3D::writeSections() {
    this->writeRegions();
    this->writeBoundaries();
    this->writeWells();
}

void CgnsCreator3D::writeRegions() {
    for (auto region : this->gridData->regions) {
        std::transform(region.name.begin(), region.name.end(), region.name.begin(), ::toupper);

        this->setElementType(region.begin, region.end, {{4,TETRA_4}, {8,HEXA_8}, {6,PENTA_6}, {5,PYRA_5}});

        if (this->elementType != MIXED) {
            this->writeSection(region.begin, region.end, region.name);
        }
        else {
            std::vector<int> offsets{0};
            for (auto element = this->global.begin() + region.begin; element != this->global.begin() + region.end; ++element) {
                switch (element->size()) {
                    case 4: {
                        element->insert(element->begin(), TETRA_4);
                        offsets.emplace_back(offsets.back() + 5);
                        break;
                    }
                    case 8: {
                        element->insert(element->begin(), HEXA_8);
                        offsets.emplace_back(offsets.back() + 9);
                        break;
                    }
                    case 5: {
                        element->insert(element->begin(), PYRA_5);
                        offsets.emplace_back(offsets.back() + 6);
                        break;
                    }
                    case 6: {
                        element->insert(element->begin(), PENTA_6);
                        offsets.emplace_back(offsets.back() + 7);
                        break;
                    }
                    default:
                        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Element type not supported");
                }
            }
            this->writePolySection(region.begin, region.end, region.name, offsets);
        }
    }
}

void CgnsCreator3D::writeBoundaries() {
    for (auto boundary : this->gridData->boundaries) {
        std::transform(boundary.name.begin(), boundary.name.end(), boundary.name.begin(), ::toupper);

        this->setElementType(boundary.begin, boundary.end, {{3,TRI_3}, {4,QUAD_4}});

        if (this->elementType != MIXED) {
            this->writeSection(boundary.begin, boundary.end, boundary.name);
        }
        else {
            std::vector<int> offsets{0};
            for (auto facet = this->global.begin() + boundary.begin; facet != this->global.begin() + boundary.end; ++facet) {
                switch (facet->size()) {
                    case 3: {
                        facet->insert(facet->begin(), TRI_3);
                        offsets.emplace_back(offsets.back() + 4);
                        break;
                    }
                    case 4: {
                        facet->insert(facet->begin(), QUAD_4);
                        offsets.emplace_back(offsets.back() + 5);
                        break;
                    }
                    default:
                        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Facet type not supported");
                }
            }
            this->writePolySection(boundary.begin, boundary.end, boundary.name, offsets);
        }
    }
}

void CgnsCreator3D::writeWells() {
    for (auto well : this->gridData->wells) {
        std::transform(well.name.begin(), well.name.end(), well.name.begin(), ::toupper);
        this->elementType = BAR_2;
        this->writeSection(well.begin, well.end, well.name);
    }
}
