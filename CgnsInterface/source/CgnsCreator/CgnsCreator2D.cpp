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

        auto regionBegin = this->globalConnectivities.begin() + region.begin;
        auto regionEnd = this->globalConnectivities.begin() + region.end;
        this->elementEnd += (regionEnd - regionBegin);

        ElementType_t elementType;
        if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 3u;}))
            elementType = TRI_3;
        else if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 4u;}))
            elementType = QUAD_4;
        else
            elementType = MIXED;

        if (elementType != MIXED) {
            std::vector<int> connectivities;
            append(regionBegin, regionEnd, std::back_inserter(connectivities));

            if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, region.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndex))
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write element section " + std::to_string(this->sectionIndex));

            this->elementStart = this->elementEnd + 1;
        }
        else {
            if (cg_section_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, region.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &this->sectionIndex))
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not partial write element section " + std::to_string(this->sectionIndex));

            for (auto element = regionBegin; element != regionEnd; ++element) {
                switch (element->size()) {
                    case 3 : {
                        element->insert(element->begin(), TRI_3);
                        break;
                    }
                    case 4: {
                        element->insert(element->begin(), QUAD_4);
                        break;
                    }
                    default:
                        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Element type not supported");
                }
            }

            std::vector<int> connectivities;
            append(regionBegin, regionEnd, std::back_inserter(connectivities));

            if (cg_elements_partial_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->sectionIndex, this->elementStart, this->elementEnd, &connectivities[0]))
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write element " + std::to_string(this->elementStart) + " in section " + std::to_string(this->sectionIndex));

            this->elementStart = this->elementEnd + 1;
        }
    }
}

void CgnsCreator2D::writeBoundaries() {
    for (auto boundary : this->gridData->boundaries) {
        std::transform(boundary.name.begin(), boundary.name.end(), boundary.name.begin(), ::toupper);

        auto boundaryBegin = this->globalConnectivities.cbegin() + boundary.begin;
        auto boundaryEnd = this->globalConnectivities.cbegin() + boundary.end;
        this->elementEnd = this->elementStart + (boundaryEnd - boundaryBegin) - 1;

        std::vector<int> connectivities;
        append(boundaryBegin, boundaryEnd, std::back_inserter(connectivities));

        if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary.name.c_str(), BAR_2, this->elementStart, this->elementEnd, this->sizes[2], &connectivities[0], &this->sectionIndex))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write facet section " + std::to_string(this->sectionIndex));

        this->elementStart = this->elementEnd + 1;
    }
}
