#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"

MshReader3D::MshReader3D(std::string filePath) : MshReader(filePath) {
    this->gridData->dimension = 3;
    this->addPhysicalEntities();
    this->addElements();
    this->addFacets();
    this->addLines();
    this->findBoundaryVertices();
    this->findRegionVertices();
    this->findWellVertices();
}

void MshReader3D::addPhysicalEntities() {
    for (int physicalEntity = 0; physicalEntity < this->numberOfPhysicalEntities; ++physicalEntity) {
        switch (this->entitiesTypes[physicalEntity]) {
            case 0: {
                this->addWell(this->entitiesNames[physicalEntity], this->physicalEntitiesRange[physicalEntity].front(), this->physicalEntitiesRange[physicalEntity].back());
                break;
            }
            case 1: {
                this->addBoundary(this->entitiesNames[physicalEntity], this->physicalEntitiesRange[physicalEntity].front(), this->physicalEntitiesRange[physicalEntity].back());
                break;
            }
            case 2: {
                this->addRegion(this->entitiesNames[physicalEntity], this->physicalEntitiesRange[physicalEntity].front(), this->physicalEntitiesRange[physicalEntity].back());
                break;
            }
            default:
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Physical entity " + std::to_string(this->entitiesTypes[physicalEntity]) + " not supported");
        }
    }
}

void MshReader3D::addWell(std::string name, int begin, int end) {
    this->gridData->wells.emplace_back(EntityData{name, begin, end});
    this->gridData->wells.back().name = name;
    this->gridData->wells.back().begin = begin;
    this->gridData->wells.back().end = end;
}

void MshReader3D::addElements() {
    for (auto& region : this->gridData->regions) {
        auto begin = this->connectivities.begin() + region.begin;
        auto end = this->connectivities.begin() + region.end;
        region.begin = this->shift;
        while (begin != end) {
            begin->push_back(this->shift++);
            switch ((*begin)[this->typeIndex]) {
                case 3: {
                    this->gridData->tetrahedrons.emplace_back(std::array<int, 5>());
                    std::copy_n(std::begin(*begin++) + this->nodeIndex, 5, std::begin(this->gridData->tetrahedrons.back()));
                    break;
                }
                case 4: {
                    this->gridData->hexahedrons.emplace_back(std::array<int, 9>());
                    std::copy_n(std::begin(*begin++) + this->nodeIndex, 9, std::begin(this->gridData->hexahedrons.back()));
                    break;
                }
                case 5: {
                    this->gridData->prisms.emplace_back(std::array<int, 7>());
                    std::copy_n(std::begin(*begin++) + this->nodeIndex, 7, std::begin(this->gridData->prisms.back()));
                    break;
                }
                case 6: {
                    this->gridData->pyramids.emplace_back(std::array<int, 6>());
                    std::copy_n(std::begin(*begin++) + this->nodeIndex, 6, std::begin(this->gridData->pyramids.back()));
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
        auto begin = this->connectivities.begin() + boundary.begin;
        auto end = this->connectivities.begin() + boundary.end;
        boundary.begin = this->shift;
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
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported facet found");
            }
        }
        boundary.end = this->shift;
    }
}

void MshReader3D::addLines() {
    for (auto& well : this->gridData->wells) {
        auto begin = this->connectivities.begin() + well.begin;
        auto end = this->connectivities.begin() + well.end;
        well.begin = this->shift;
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
        well.end = this->shift;
    }
}

void MshReader3D::findBoundaryVertices() {
    for (auto& boundary : this->gridData->boundaries) {
        std::set<int> vertices;

        for (const auto& triangle : this->gridData->triangles)
            if (triangle.back() >= boundary.begin && triangle.back() < boundary.end)
                vertices.insert(triangle.cbegin(), triangle.cend() - 1);

        for (const auto& quadrangle : this->gridData->quadrangles)
            if (quadrangle.back() >= boundary.begin && quadrangle.back() < boundary.end)
                vertices.insert(quadrangle.cbegin(), quadrangle.cend() - 1);

        boundary.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}

void MshReader3D::findRegionVertices() {
    for (auto& region : this->gridData->regions) {
        std::set<int> vertices;

        for (const auto& tetrahedron : this->gridData->tetrahedrons)
            if (tetrahedron.back() >= region.begin && tetrahedron.back() < region.end)
                vertices.insert(tetrahedron.cbegin(), tetrahedron.cend() - 1);

        for (const auto& hexahedron : this->gridData->hexahedrons)
            if (hexahedron.back() >= region.begin && hexahedron.back() < region.end)
                vertices.insert(hexahedron.cbegin(), hexahedron.cend() - 1);

        for (const auto& prism : this->gridData->prisms)
            if (prism.back() >= region.begin && prism.back() < region.end)
                vertices.insert(prism.cbegin(), prism.cend() - 1);

        for (const auto& pyramid : this->gridData->pyramids)
            if (pyramid.back() >= region.begin && pyramid.back() < region.end)
                vertices.insert(pyramid.cbegin(), pyramid.cend() - 1);

        region.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}

void MshReader3D::findWellVertices() {
    for (auto& well : this->gridData->wells) {
        std::set<int> vertices;

        for (const auto& line : this->gridData->lines)
            if (line.back() >= well.begin && line.back() < well.end)
                vertices.insert(line.cbegin(), line.cend() - 1);

        well.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}
