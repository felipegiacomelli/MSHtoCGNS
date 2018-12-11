#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"

MshReader3D::MshReader3D(std::string filePath) : MshReader(filePath) {
    this->gridData->dimension = 3;
    this->addPhysicalEntities();
    this->addElements();
    this->addFacets();
    this->defineBoundaryVertices();
}

void MshReader3D::addPhysicalEntities() {
    for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
        switch (this->entitiesTypes[i]) {
            case 1: {
                this->gridData->boundaries.emplace_back(BoundaryData());
                this->gridData->boundaries.back().name = this->entitiesNames[i];
                this->gridData->boundaries.back().begin = this->physicalEntitiesRange[i].front();
                this->gridData->boundaries.back().end = this->physicalEntitiesRange[i].back();
                break;
            }
            case 2: {
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
    for (auto& region : this->gridData->regions) {
        auto regionBegin = this->connectivities.begin() + region.begin;
        auto regionEnd = this->connectivities.begin() + region.end;
        region.begin = this->shift;
        for (auto element = regionBegin; element != regionEnd; element++) {
            element->push_back(this->shift++);
            switch ((*element)[this->typeIndex]) {
                case 3: {
                    this->gridData->tetrahedronConnectivity.emplace_back(std::array<int, 5>());
                    std::copy_n(element->begin() + this->nodeIndex, 5, std::begin(this->gridData->tetrahedronConnectivity.back()));
                    break;
                }
                case 4: {
                    this->gridData->hexahedronConnectivity.emplace_back(std::array<int, 9>());
                    std::copy_n(element->begin() + this->nodeIndex, 9, std::begin(this->gridData->hexahedronConnectivity.back()));
                    break;
                }
                default:
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported element found");
            }
        }
        region.end = this->shift;
    }
}

void MshReader3D::addFacets() {
    for (auto& boundary : this->gridData->boundaries) {
        auto boundaryBegin = this->connectivities.begin() + boundary.begin;
        auto boundaryEnd = this->connectivities.begin() + boundary.end;
        boundary.begin = this->shift;
        for (auto facet = boundaryBegin; facet != boundaryEnd; facet++) {
            facet->push_back(this->shift++);
            switch ((*facet)[this->typeIndex]) {
                case 1: {
                    this->gridData->triangleConnectivity.emplace_back(std::array<int, 4>());
                    std::copy_n(facet->begin() + this->nodeIndex, 4, std::begin(this->gridData->triangleConnectivity.back()));
                    break;
                }
                case 2: {
                    this->gridData->quadrangleConnectivity.emplace_back(std::array<int, 5>());
                    std::copy_n(facet->begin() + this->nodeIndex, 5, std::begin(this->gridData->quadrangleConnectivity.back()));
                    break;
                }
                default:
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported facet found");
            }
        }
        boundary.end = this->shift;
    }
}

void MshReader3D::defineBoundaryVertices() {
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
