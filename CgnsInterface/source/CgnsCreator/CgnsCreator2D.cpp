#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator2D.hpp"
#include <cgnslib.h>

CgnsCreator2D::CgnsCreator2D(boost::shared_ptr<GridData> gridData, std::string folderPath, bool createInConstructor) : CgnsCreator(gridData, folderPath) {
    if (createInConstructor) {
        this->checkDimension();
        this->setDimensions();
        this->setupFile();
        this->initialize();
    }
}

void CgnsCreator2D::checkDimension() {
    if (this->gridData->dimension != 2)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be equal to 2 and not " + std::to_string(this->gridData->dimension));

    if (this->gridData->tetrahedrons.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Number of lines in gridData must be 0 and not " + std::to_string(this->gridData->tetrahedrons.size()));

    if (this->gridData->hexahedrons.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Number of lines in gridData must be 0 and not " + std::to_string(this->gridData->hexahedrons.size()));

    if (this->gridData->prisms.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Number of lines in gridData must be 0 and not " + std::to_string(this->gridData->prisms.size()));

    if (this->gridData->pyramids.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Number of lines in gridData must be 0 and not " + std::to_string(this->gridData->pyramids.size()));
}

void CgnsCreator2D::setDimensions() {
    this->physicalDimension = this->gridData->dimension;
    this->cellDimension = this->gridData->dimension;
    this->sizes[0] = this->gridData->coordinates.size();
    this->sizes[1] = this->gridData->triangles.size() + this->gridData->quadrangles.size();
    this->sizes[2] = 0;
}

void CgnsCreator2D::writeCoordinates() {
    std::vector<double> coordinatesX(this->sizes[0]);
    std::vector<double> coordinatesY(this->sizes[0]);
    for (int i = 0; i < this->sizes[0]; ++i) {
        coordinatesX[i] = this->gridData->coordinates[i][0];
        coordinatesY[i] = this->gridData->coordinates[i][1];
    }

    if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateX", &coordinatesX[0], &this->coordinateIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write CoordinateX");

    if (cg_coord_write(this->fileIndex, this->baseIndex, this->zoneIndex, RealDouble, "CoordinateY", &coordinatesY[0], &this->coordinateIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write CoordinateY");
}

void CgnsCreator2D::writeRegions() {
    for (auto region : this->gridData->regions) {
        std::transform(region.name.begin(), region.name.end(), region.name.begin(), ::toupper);

        this->setElementType(region.begin, region.end, {{3,TRI_3}, {4,QUAD_4}});

        if (this->elementType != MIXED) {
            this->writeSection(region.begin, region.end, region.name);
        }
        else {
            std::vector<int> offsets{0};
            for (auto element = this->global.begin() + region.begin; element != this->global.begin() + region.end; ++element) {
                switch (element->size()) {
                    case 3 : {
                        element->insert(element->begin(), TRI_3);
                        offsets.emplace_back(offsets.back() + 4);
                        break;
                    }
                    case 4: {
                        element->insert(element->begin(), QUAD_4);
                        offsets.emplace_back(offsets.back() + 5);
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

void CgnsCreator2D::writeBoundaries() {
    for (auto boundary : this->gridData->boundaries) {
        std::transform(boundary.name.begin(), boundary.name.end(), boundary.name.begin(), ::toupper);
        this->elementType = BAR_2;
        this->writeSection(boundary.begin, boundary.end, boundary.name);
    }
}
