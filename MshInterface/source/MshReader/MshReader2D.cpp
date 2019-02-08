#include "MSHtoCGNS/MshInterface/MshReader/MshReader2D.hpp"

MshReader2D::MshReader2D(std::string filePath) : MshReader(filePath) {
    this->gridData->dimension = 2;
    this->addPhysicalEntities();
    this->addElements();
    this->addFacets();
    this->findBoundaryVertices();
    this->findRegionVertices();
}

void MshReader2D::addPhysicalEntities() {
    for (int physicalEntity = 0; physicalEntity < this->numberOfPhysicalEntities; ++physicalEntity) {
        switch (this->entitiesTypes[physicalEntity]) {
            case 0: {
                this->addBoundary(this->entitiesNames[physicalEntity], this->physicalEntitiesRange[physicalEntity].front(), this->physicalEntitiesRange[physicalEntity].back());
                break;
            }
            case 1: {
                this->addRegion(this->entitiesNames[physicalEntity], this->physicalEntitiesRange[physicalEntity].front(), this->physicalEntitiesRange[physicalEntity].back());
                break;
            }
            default:
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Physical entity " + std::to_string(this->entitiesTypes[physicalEntity]) + " not supported");
        }
    }
}

void MshReader2D::addElements() {
    for (auto& region : this->gridData->regions) {
        auto begin = this->connectivities.begin() + region.begin;
        auto end = this->connectivities.begin() + region.end;
        region.begin = this->shift;
        while (begin != end) {
            begin->push_back(this->shift++);
            switch ((*begin)[this->typeIndex]) {
                case 1: {
                    this->gridData->triangles.emplace_back(std::array<int, 4>());
                    std::copy_n(std::begin(*begin++) + this->nodeIndex, 4, std::begin(this->gridData->triangles.back()));
                    break;
                }
                case 2: {
                    this->gridData->quadrangles.emplace_back(std::array<int, 5>());
                    std::copy_n(std::begin(*begin++) + this->nodeIndex, 5, std::begin(this->gridData->quadrangles.back()));
                    break;
                }
                default:
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported element found");
            }
        }
        region.end = this->shift;
    }
}

void MshReader2D::addFacets() {
    for (auto& boundary : this->gridData->boundaries) {
        auto begin = this->connectivities.begin() + boundary.begin;
        auto end = this->connectivities.begin() + boundary.end;
        boundary.begin = this->shift;
        while (begin != end) {
            begin->push_back(this->shift++);
            switch ((*begin)[this->typeIndex]) {
                case 0: {
                    this->gridData->lines.emplace_back(std::array<int, 3>());
                    std::copy_n(std::begin(*begin++) + this->nodeIndex, 3, std::begin(this->gridData->lines.back()));
                    break;
                }
                default:
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported facet found");
            }
        }
        boundary.end = this->shift;
    }
}

void MshReader2D::findBoundaryVertices() {
    for (auto& boundary : this->gridData->boundaries) {
        std::set<int> vertices;

        for (const auto& line : this->gridData->lines)
            if (line.back() >= boundary.begin && line.back() < boundary.end)
                vertices.insert(line.cbegin(), line.cend() - 1);

        boundary.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}

void MshReader2D::findRegionVertices() {
    for (auto& region : this->gridData->regions) {
        std::set<int> vertices;

        for (const auto& triangle : this->gridData->triangles)
            if (triangle.back() >= region.begin && triangle.back() < region.end)
                vertices.insert(triangle.cbegin(), triangle.cend() - 1);

        for (const auto& quadrangle : this->gridData->quadrangles)
            if (quadrangle.back() >= region.begin && quadrangle.back() < region.end)
                vertices.insert(quadrangle.cbegin(), quadrangle.cend() - 1);

        region.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}
