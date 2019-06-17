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

        auto regionBegin = this->globalConnectivities.begin() + region.begin;
        auto regionEnd = this->globalConnectivities.begin() + region.end;
        this->elementEnd += (regionEnd - regionBegin);

        ElementType_t elementType = ElementType_t(0);
        if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 4u;}))
            elementType = TETRA_4;
        else if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 8u;}))
            elementType = HEXA_8;
        else if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 6u;}))
            elementType = PENTA_6;
        else if (std::all_of(regionBegin, regionEnd, [](const auto& connectivity){return connectivity.size() == 5u;}))
            elementType = PYRA_5;
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
            std::vector<int> connectivitiesOffset{0};
            for (auto element = regionBegin; element != regionEnd; ++element) {
                switch (element->size()) {
                    case 4: {
                        element->insert(element->begin(), TETRA_4);
                        connectivitiesOffset.emplace_back(connectivitiesOffset.back() + 5);
                        break;
                    }
                    case 8: {
                        element->insert(element->begin(), HEXA_8);
                        connectivitiesOffset.emplace_back(connectivitiesOffset.back() + 9);
                        break;
                    }
                    case 5: {
                        element->insert(element->begin(), PYRA_5);
                        connectivitiesOffset.emplace_back(connectivitiesOffset.back() + 6);
                        break;
                    }
                    case 6: {
                        element->insert(element->begin(), PENTA_6);
                        connectivitiesOffset.emplace_back(connectivitiesOffset.back() + 7);
                        break;
                    }
                    default:
                        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Element type not supported");
                }
            }

            std::vector<int> connectivities;
            append(regionBegin, regionEnd, std::back_inserter(connectivities));

            if (cg_poly_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, region.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &connectivitiesOffset[0], &this->sectionIndex))
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not partial write element section " + region.name);

            this->elementStart = this->elementEnd + 1;
        }
    }
}

void CgnsCreator3D::writeBoundaries() {
    for (auto boundary : this->gridData->boundaries) {
        std::transform(boundary.name.begin(), boundary.name.end(), boundary.name.begin(), ::toupper);

        auto boundaryBegin = this->globalConnectivities.begin() + boundary.begin;
        auto boundaryEnd = this->globalConnectivities.begin() + boundary.end;
        this->elementEnd = this->elementStart + (boundaryEnd - boundaryBegin) - 1;

        ElementType_t elementType;
        if (std::all_of(boundaryBegin, boundaryEnd, [](const auto& connectivity){return connectivity.size() == 3u;}))
            elementType = TRI_3;
        else if (std::all_of(boundaryBegin, boundaryEnd, [](const auto& connectivity){return connectivity.size() == 4u;}))
            elementType = QUAD_4;
        else
            elementType = MIXED;

        if (elementType != MIXED) {
            std::vector<int> connectivities;
            append(boundaryBegin, boundaryEnd, std::back_inserter(connectivities));

            if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndex))
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write facet section " + std::to_string(this->sectionIndex));

            this->elementStart = this->elementEnd + 1;
        }
        else {
            std::vector<int> connectivitiesOffset{0};
            for (auto facet = boundaryBegin; facet != boundaryEnd; ++facet) {
                switch (facet->size()) {
                    case 3: {
                        facet->insert(facet->begin(), TRI_3);
                        connectivitiesOffset.emplace_back(connectivitiesOffset.back() + 4);
                        break;
                    }
                    case 4: {
                        facet->insert(facet->begin(), QUAD_4);
                        connectivitiesOffset.emplace_back(connectivitiesOffset.back() + 5);
                        break;
                    }
                    default:
                        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Facet type not supported");
                }
            }

            std::vector<int> connectivities;
            append(boundaryBegin, boundaryEnd, std::back_inserter(connectivities));

            if (cg_poly_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, boundary.name.c_str(), elementType, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &connectivitiesOffset[0], &this->sectionIndex))
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not partial write element section " + boundary.name);

            this->elementStart = this->elementEnd + 1;
        }
    }
}

void CgnsCreator3D::writeWells() {
    for (auto well : this->gridData->wells) {
        std::transform(well.name.begin(), well.name.end(), well.name.begin(), ::toupper);

        auto wellBegin = this->globalConnectivities.cbegin() + well.begin;
        auto wellEnd = this->globalConnectivities.cbegin() + well.end;
        this->elementEnd = this->elementStart + (wellEnd - wellBegin) - 1;

        std::vector<int> connectivities;
        append(wellBegin, wellEnd, std::back_inserter(connectivities));

        if (cg_section_write(this->fileIndex, this->baseIndex, this->zoneIndex, well.name.c_str(), BAR_2, this->elementStart, this->elementEnd, sizes[2], &connectivities[0], &this->sectionIndex))
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write well section " + std::to_string(this->sectionIndex));

        this->elementStart = this->elementEnd + 1;
    }
}
