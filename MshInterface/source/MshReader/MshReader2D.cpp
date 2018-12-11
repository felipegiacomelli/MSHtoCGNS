#include "MSHtoCGNS/MshInterface/MshReader/MshReader2D.hpp"

MshReader2D::MshReader2D(std::string filePath) : MshReader(filePath) {
    this->gridData->dimension = 2;
    this->readPhysicalNames();
    this->readNodes();
    this->readElements();
    this->determinePhysicalEntitiesRange();
    this->addPhysicalEntities();
    this->addElements();
    this->addFacets();
    this->defineBoundaryVertices();
}

void MshReader2D::addPhysicalEntities() {
    for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
        switch (this->entitiesTypes[i]) {
            case 0: {
                this->numberOfBoundaries++;
                this->gridData->boundaries.emplace_back(BoundaryData());
                this->gridData->boundaries.back().name = this->entitiesNames[i];
                this->gridData->boundaries.back().begin = this->physicalEntitiesRange[i].front();
                this->gridData->boundaries.back().end = this->physicalEntitiesRange[i].back();
                break;
            }
            case 1: {
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

void MshReader2D::addElements() {
    for (auto& region : this->gridData->regions) {
        auto regionBegin = this->connectivities.begin() + region.begin;
        auto regionEnd = this->connectivities.begin() + region.end;
        region.begin = this->shift;
        for (auto element = regionBegin; element != regionEnd; element++) {
            element->push_back(this->shift++);
            switch ((*element)[this->typeIndex]) {
                case 1: {
                    this->gridData->triangleConnectivity.emplace_back(std::array<int, 4>());
                    std::copy_n(element->begin() + this->nodeIndex, 4, std::begin(this->gridData->triangleConnectivity.back()));
                    break;
                }
                case 2: {
                    this->gridData->quadrangleConnectivity.emplace_back(std::array<int, 5>());
                    std::copy_n(element->begin() + this->nodeIndex, 5, std::begin(this->gridData->quadrangleConnectivity.back()));
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
        auto boundaryBegin = this->connectivities.begin() + boundary.begin;
        auto boundaryEnd = this->connectivities.begin() + boundary.end;
        boundary.begin = this->shift;
        for (auto facet = boundaryBegin; facet != boundaryEnd; facet++) {
            facet->push_back(this->shift++);
            switch ((*facet)[this->typeIndex]) {
                case 0: {
                    this->gridData->lineConnectivity.emplace_back(std::array<int, 3>());
                    std::copy_n(facet->begin() + this->nodeIndex, 3, std::begin(this->gridData->lineConnectivity.back()));
                    break;
                }
                default:
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported facet found");
            }
        }
        boundary.end = this->shift;
    }
}

// void MshReader2D::defineBoundaryVertices() {
//     for (auto boundary = this->gridData->boundaries.begin(); boundary < this->gridData->boundaries.end(); boundary++) {
//         std::set<int> vertices;
//         std::vector<std::array<int, 3>> boundaryConnectivity(this->gridData->lineConnectivity.cbegin() + boundary->begin - this->elements.size(), this->gridData->lineConnectivity.cbegin() + boundary->end - this->elements.size());
//         for (unsigned j = 0; j < boundaryConnectivity.size(); j++)
//             for (unsigned k = 0; k != 2u; k++)
//                 vertices.insert(boundaryConnectivity[j][k]);
//         boundary->vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
//     }
// }

void MshReader2D::defineBoundaryVertices() {
    for (auto& boundary : this->gridData->boundaries) {
        std::set<int> vertices;

        for (const auto& line : this->gridData->lineConnectivity)
            if (line.back() >= boundary.begin && line.back() < boundary.end)
                vertices.insert(line.cbegin(), line.cend() - 1);

        boundary.vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}
