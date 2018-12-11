#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"

MshReader3D::MshReader3D(std::string filePath) : MshReader(filePath) {
    this->gridData->dimension = 3;
    this->readPhysicalNames();
    this->readNodes();
    this->readElements();
    this->determinePhysicalEntitiesRange();
    this->addPhysicalEntities();
    this->addElements();
    this->addFacets();
    this->defineBoundaryVertices();
}

void MshReader3D::addPhysicalEntities() {
    for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
        switch (this->entitiesTypes[i]) {
            case 1: {
                this->numberOfBoundaries++;
                this->gridData->boundaries.emplace_back(BoundaryData());
                this->gridData->boundaries.back().name = this->entitiesNames[i];
                this->gridData->boundaries.back().begin = this->physicalEntitiesRange[i].front();
                this->gridData->boundaries.back().end = this->physicalEntitiesRange[i].back();
                break;
            }
            case 2: {
                this->numberOfRegions++;
                this->gridData->regions.emplace_back(RegionData());
                this->gridData->regions.back().name = this->entitiesNames[i];
                this->gridData->regions.back().begin = this->physicalEntitiesRange[i].front();
                this->gridData->regions.back().end = this->physicalEntitiesRange[i].back();
                break;
            }
            default:
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Physical entity " + std::to_string(this->entitiesTypes[i]) + " not supported");
        }
    }
}

void MshReader3D::addElements() {
    for (unsigned i = 0; i < this->regionElements.size(); i++) {
        this->gridData->regions[i].begin = this->regionElements[i].front();
        this->gridData->regions[i].end   = this->regionElements[i].back() + 1;
        for (unsigned j = 0; j < this->regionElements[i].size(); j++) {
            int index = this->regionElements[i][j];
            int type  = this->elements[index][this->typeIndex];
            std::vector<int> connectivity(this->elements[index].cbegin() + this->nodeIndex, this->elements[index].cend());
            switch (type) {
                case 1: {
                    std::array<int, 4> triangle;
                    std::copy_n(std::begin(connectivity), 4, std::begin(triangle));
                    this->gridData->triangleConnectivity.emplace_back(std::move(triangle));
                    break;
                }
                case 2: {
                    std::array<int, 5> quadrangle;
                    std::copy_n(std::begin(connectivity), 5, std::begin(quadrangle));
                    this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));
                    break;
                }
                default:
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported element found");
            }
        }
    }
}

void MshReader3D::addFacets() {
    for (unsigned i = 0; i < this->regionElements.size(); i++) {
        this->gridData->regions[i].begin = this->regionElements[i].front();
        this->gridData->regions[i].end   = this->regionElements[i].back() + 1;
        for (unsigned j = 0; j < this->regionElements[i].size(); j++) {
            int index = this->regionElements[i][j];
            int type  = this->elements[index][this->typeIndex];
            std::vector<int> connectivity(this->elements[index].cbegin() + this->nodeIndex, this->elements[index].cend());
            switch (type) {
                case 1: {
                    std::array<int, 4> triangle;
                    std::copy_n(std::begin(connectivity), 4, std::begin(triangle));
                    this->gridData->triangleConnectivity.emplace_back(std::move(triangle));
                    break;
                }
                case 2: {
                    std::array<int, 5> quadrangle;
                    std::copy_n(std::begin(connectivity), 5, std::begin(quadrangle));
                    this->gridData->quadrangleConnectivity.emplace_back(std::move(quadrangle));
                    break;
                }
                default:
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported element found");
            }
        }
    }
}

void MshReader3D::defineBoundaryVertices() {
    for (auto boundary = this->gridData->boundaries.begin(); boundary != this->gridData->boundaries.end(); boundary++) {
        std::set<int> vertices;
        if (this->gridData->triangleConnectivity.size() > 0) {
            std::vector<std::array<int, 4>> boundaryConnectivity(this->gridData->triangleConnectivity.cbegin() + boundary->begin - this->elements.size(), this->gridData->triangleConnectivity.cbegin() + boundary->end - this->elements.size());
            for (unsigned j = 0; j < boundaryConnectivity.size(); j++)
                for (unsigned k = 0; k != 3u; k++)
                    vertices.insert(boundaryConnectivity[j][k]);
            boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
        }
        else {
            std::vector<std::array<int, 5>> boundaryConnectivity(this->gridData->quadrangleConnectivity.cbegin() + boundary->begin - this->elements.size(), this->gridData->quadrangleConnectivity.cbegin() + boundary->end - this->elements.size());
            for (unsigned j = 0; j < boundaryConnectivity.size(); j++)
                for (unsigned k = 0; k != 4u; k++)
                    vertices.insert(boundaryConnectivity[j][k]);
            boundary->vertices = std::vector<int>(vertices.begin(), vertices.end());
        }
    }
}
