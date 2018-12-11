#include "MSHtoCGNS/MshInterface/MshReader/MshReader2D.hpp"

MshReader2D::MshReader2D(std::string filePath) : MshReader(filePath) {
    this->gridData->dimension = 2;
    this->readPhysicalNames();
    this->addPhysicalEntities();
    this->readNodes();
    this->readElements();
    this->determineNumberOfFacets();
    this->divideConnectivities();
    this->assignElementsToRegions();
    this->assignFacetsToBoundaries();
    this->addRegions();
    this->addBoundaries();
    this->defineBoundaryVertices();
}

void MshReader2D::addPhysicalEntities() {
    for (int i = 0; i < this->numberOfPhysicalEntities; i++) {
        switch (this->entitiesTypes[i]) {
            case 0: {
                this->numberOfBoundaries++;
                this->gridData->boundaries.emplace_back(BoundaryData());
                this->gridData->boundaries.back().name = this->entitiesNames[i];
                break;
            }
            case 1: {
                this->numberOfRegions++;
                this->gridData->regions.emplace_back(RegionData());
                this->gridData->regions.back().name = this->entitiesNames[i];
                break;
            }
            default:
                throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Physical entity " + std::to_string(this->entitiesTypes[i]) + " not supported");
        }
    }
}

void MshReader2D::determineNumberOfFacets() {
    this->numberOfFacets = 0;
    for (unsigned i = 0; i < this->connectivities.size(); i++) {
        if (this->connectivities[i][this->typeIndex] != 0)
            break;
        else
            this->numberOfFacets++;
    }
}

void MshReader2D::addRegions() {
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

void MshReader2D::addBoundaries() {
    for (unsigned i = 0; i < this->boundaryFacets.size(); i++) {
        this->gridData->boundaries[i].begin = this->facets[this->boundaryFacets[i].front()].back();
        this->gridData->boundaries[i].end   = this->facets[this->boundaryFacets[i].back() ].back() + 1;
        for (unsigned j = 0; j < this->boundaryFacets[i].size(); j++) {
            int index = this->boundaryFacets[i][j];
            int type  = this->facets[index][this->typeIndex];
            std::vector<int> connectivity(this->facets[index].cbegin() + this->nodeIndex, this->facets[index].cend());
            switch (type) {
                case 0: {
                    std::array<int, 3> line;
                    std::copy_n(std::begin(connectivity), 3, std::begin(line));
                    this->gridData->lineConnectivity.emplace_back(std::move(line));
                    break;
                }
                default:
                    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Non supported facet found");
            }
        }
    }
}

void MshReader2D::defineBoundaryVertices() {
    for (auto boundary = this->gridData->boundaries.begin(); boundary < this->gridData->boundaries.end(); boundary++) {
        std::set<int> vertices;
        std::vector<std::array<int, 3>> boundaryConnectivity(this->gridData->lineConnectivity.cbegin() + boundary->begin - this->elements.size(), this->gridData->lineConnectivity.cbegin() + boundary->end - this->elements.size());
        for (unsigned j = 0; j < boundaryConnectivity.size(); j++)
            for (unsigned k = 0; k != 2u; k++)
                vertices.insert(boundaryConnectivity[j][k]);
        boundary->vertices = std::vector<int>(vertices.cbegin(), vertices.cend());
    }
}
