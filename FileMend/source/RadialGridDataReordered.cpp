#include "MSHtoCGNS/FileMend/RadialGridDataReordered.hpp"
#include <cgnslib.h>

RadialGridDataReordered::RadialGridDataReordered(boost::shared_ptr<GridData> gridData) : gridData(gridData) {
    this->checkGridData();
    this->defineQuantities();
    this->createReordered();
    this->reorderBoundaries();
    this->copyData();
    this->reorder();
    this->copyVertices();
    this->rectifyConnectivities();
    this->fixElementIndices();
    this->fixFacetIndices();
    this->fixLineIndices();
}

void RadialGridDataReordered::checkGridData() {
    if (this->gridData->dimension != 3)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

    if (this->gridData->tetrahedrons.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData tetrahedrons size must be 0 and not " + std::to_string(this->gridData->tetrahedrons.size()));

    if (this->gridData->hexahedrons.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData hexahedrons size must not be 0");

    if (this->gridData->prisms.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData prisms size must not be 0");

    if (this->gridData->pyramids.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData pyramids size must be 0 and not " + std::to_string(this->gridData->pyramids.size()));

    if (this->gridData->triangles.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData triangles size must not be 0");

    if (this->gridData->quadrangles.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData quadrangles size must not be 0");

    if (this->gridData->lines.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData lines size must not be 0");

    if (this->gridData->boundaries.size() != 3u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData boundaries size must be 1 and not " + std::to_string(this->gridData->boundaries.size()));

    if (this->gridData->regions.size() != 1u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData regions size must be 1 and not " + std::to_string(this->gridData->regions.size()));

    if (this->gridData->wells.size() != 1u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData wells size must be 1 and not " + std::to_string(this->gridData->wells.size()));
}

void RadialGridDataReordered::defineQuantities() {
    this->numberOfSegments = this->gridData->lines.size();
    this->numberOfPrismsPerSegment = this->gridData->prisms.size() / this->numberOfSegments;
    this->numberOfHexahedronsPerSegment = this->gridData->hexahedrons.size() / this->numberOfSegments;
    this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;
    this->numberOfVerticesPerSection = this->gridData->coordinates.size() / (this->gridData->lines.size() + 1);
}

void RadialGridDataReordered::createReordered() {
    this->reordered = boost::make_shared<GridData>();
    this->reordered->dimension = this->gridData->dimension;

    this->reordered->triangles = this->gridData->triangles;
    this->reordered->quadrangles = this->gridData->quadrangles;
    this->reordered->lines = this->gridData->lines;

    this->reordered->boundaries = this->gridData->boundaries;

    this->reordered->regions = this->gridData->regions;

    this->reordered->wells = this->gridData->wells;
}

void RadialGridDataReordered::reorderBoundaries() {
    int firstVertex = this->gridData->lines.front()[0];
    int lastVertex  = this->gridData->lines.back()[1];
    auto boundary = std::find_if(this->reordered->boundaries.begin(), this->reordered->boundaries.end(),
                                    [=](auto b){return !hasElement(b.vertices.cbegin(), b.vertices.cend(), firstVertex) && !hasElement(b.vertices.cbegin(), b.vertices.cend(), lastVertex);});
    std::iter_swap(this->reordered->boundaries.begin(), boundary);

    boundary = std::find_if(this->reordered->boundaries.begin(), this->reordered->boundaries.end(), [=](auto b){return hasElement(b.vertices.cbegin(), b.vertices.cend(), firstVertex);});
    std::iter_swap(this->reordered->boundaries.begin() + 1, boundary);

    boundary = std::find_if(this->reordered->boundaries.begin(), this->reordered->boundaries.end(), [=](auto b){return hasElement(b.vertices.cbegin(), b.vertices.cend(), lastVertex);});
    std::iter_swap(this->reordered->boundaries.begin() + 2, boundary);
}

void RadialGridDataReordered::copyData() {
    this->hexahedrons = this->gridData->hexahedrons;
    for (auto i = 0u; i < this->hexahedrons.size(); ++i)
        this->hexahedrons[i].back() = i;

    this->prisms = this->gridData->prisms;
    for (auto i = 0u; i < this->prisms.size(); ++i)
        this->prisms[i].back() = i;

    auto boundary = this->reordered->boundaries.begin() + 1;

    for (auto triangle = this->gridData->triangles.cbegin(); triangle != this->gridData->triangles.cend(); ++triangle)
        if (triangle->back() >= boundary->begin && triangle->back() < boundary->end)
            this->triangles.emplace_back(*triangle);

    for (auto quadrangle = this->gridData->quadrangles.cbegin(); quadrangle != this->gridData->quadrangles.cend(); ++quadrangle)
        if (quadrangle->back() >= boundary->begin && quadrangle->back() < boundary->end)
            this->quadrangles.emplace_back(*quadrangle);
}

void RadialGridDataReordered::reorder() {
    this->buildFirstSection();
    for (int segment = 0; segment < this->numberOfSegments; ++segment) {
        this->vertexShift = 0;
        for (auto triangle = this->triangles.begin(); triangle != this->triangles.end(); ++triangle)
            for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
                if (hasElements(prism->cbegin(), prism->cend()-1, triangle->cbegin(), triangle->cend()-1)) {
                    this->addVertex((*prism)[3], segment + 1);
                    this->addVertex((*prism)[4], segment + 1);
                    this->addVertex((*prism)[5], segment + 1);
                    this->updateTriangle(prism, triangle);
                    this->copyPrism(prism);
                    break;
                }
                else
                    ++prism;

        for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); ++quadrangle)
            for (auto hexahedron = this->hexahedrons.begin(); hexahedron != this->hexahedrons.end();)
                if (hasElements(hexahedron->cbegin(), hexahedron->cend()-1, quadrangle->cbegin(), quadrangle->cend()-1)) {
                    this->addVertex((*hexahedron)[3], segment + 1);
                    this->addVertex((*hexahedron)[2], segment + 1);
                    this->addVertex((*hexahedron)[6], segment + 1);
                    this->addVertex((*hexahedron)[7], segment + 1);
                    this->updateQuadrangle(hexahedron, quadrangle);
                    this->copyHexahedron(hexahedron);
                    break;
                }
                else
                    ++hexahedron;
    }
}

void RadialGridDataReordered::buildFirstSection() {
    for (auto triangle = this->triangles.begin(); triangle != this->triangles.end(); ++triangle)
        for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
            if (hasElements(prism->cbegin(), prism->cend()-1, triangle->cbegin(), triangle->cend()-1)) {
                this->addVertex((*prism)[0], 0);
                this->addVertex((*prism)[1], 0);
                this->addVertex((*prism)[2], 0);
                break;
            }
            else
                ++prism;

    for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); ++quadrangle)
        for (auto hexahedron = this->hexahedrons.begin(); hexahedron != this->hexahedrons.end();)
            if (hasElements(hexahedron->cbegin(), hexahedron->cend()-1, quadrangle->cbegin(), quadrangle->cend()-1)) {
                this->addVertex((*hexahedron)[0], 0);
                this->addVertex((*hexahedron)[1], 0);
                this->addVertex((*hexahedron)[5], 0);
                this->addVertex((*hexahedron)[4], 0);
                break;
            }
            else
                ++hexahedron;
}

void RadialGridDataReordered::addVertex(int vertex, int section) {
    if (std::find_if(this->vertices.cbegin(), this->vertices.cend(), [=](auto vh){return vh.first == vertex;}) == this->vertices.cend())
        this->vertices.push_back(std::make_pair(vertex, section * this->numberOfVerticesPerSection + this->vertexShift++));
}

void RadialGridDataReordered::updateTriangle(std::vector<std::array<int, 7>>::iterator prism, std::vector<std::array<int, 4>>::iterator triangle) {
    (*triangle)[0] = (*prism)[3];
    (*triangle)[1] = (*prism)[4];
    (*triangle)[2] = (*prism)[5];
}

void RadialGridDataReordered::updateQuadrangle(std::vector<std::array<int, 9>>::iterator hexahedron, std::vector<std::array<int, 5>>::iterator quadrangle) {
    (*quadrangle)[0] = (*hexahedron)[2];
    (*quadrangle)[1] = (*hexahedron)[3];
    (*quadrangle)[2] = (*hexahedron)[7];
    (*quadrangle)[3] = (*hexahedron)[6];
}

void RadialGridDataReordered::copyPrism(std::vector<std::array<int, 7>>::iterator prism) {
    this->reordered->prisms.push_back(this->gridData->prisms[prism->back()]);
    prism = this->prisms.erase(prism);
}

void RadialGridDataReordered::copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron) {
    this->reordered->hexahedrons.push_back(this->gridData->hexahedrons[hexahedron->back()]);
    hexahedron = this->hexahedrons.erase(hexahedron);
}

void RadialGridDataReordered::copyVertices() {
    std::sort(this->vertices.begin(), this->vertices.end(), [](auto a, auto b) {return a.second < b.second;});
    for (auto vertex : this->vertices)
        this->reordered->coordinates.emplace_back(this->gridData->coordinates[vertex.first]);
}

void RadialGridDataReordered::rectifyConnectivities() {
    std::unordered_map<int, int> originalToReordered;
    for (auto vertex : this->vertices)
        originalToReordered[vertex.first] = vertex.second;

    this->rectifyConnectivity(originalToReordered, this->reordered->hexahedrons);
    this->rectifyConnectivity(originalToReordered, this->reordered->prisms);
    this->rectifyConnectivity(originalToReordered, this->reordered->triangles);
    this->rectifyConnectivity(originalToReordered, this->reordered->quadrangles);
    this->rectifyConnectivity(originalToReordered, this->reordered->lines);

    for (auto& boundary : this->reordered->boundaries) {
        for (auto& vertex : boundary.vertices)
            vertex = originalToReordered[vertex];
        std::sort(boundary.vertices.begin(), boundary.vertices.end());
    }

    for (auto& vertex : this->reordered->wells[0].vertices)
        vertex = originalToReordered[vertex];
    std::sort(this->reordered->wells[0].vertices.begin(), this->reordered->wells[0].vertices.end());
}

void RadialGridDataReordered::fixElementIndices() {
    for (int s = 0; s < this->numberOfSegments; ++s) {
        for (int p = 0; p < this->numberOfPrismsPerSegment; ++p)
            this->reordered->prisms[s * this->numberOfPrismsPerSegment + p].back() = this->elementShift++;

        for (int h = 0; h < this->numberOfHexahedronsPerSegment; ++h)
            this->reordered->hexahedrons[s * this->numberOfHexahedronsPerSegment + h].back() = this->elementShift++;
    }
}

void RadialGridDataReordered::fixFacetIndices() {
    this->quadrangles.clear();
    this->triangles.clear();

    auto boundary = this->reordered->boundaries.begin();
    for (auto quadrangle = this->reordered->quadrangles.cbegin(); quadrangle != this->reordered->quadrangles.cend(); ++quadrangle)
        if (quadrangle->back() >= boundary->begin && quadrangle->back() < boundary->end)
            this->quadrangles.emplace_back(*quadrangle);

    boundary->begin = this->elementShift;
    for (int segment = 0; segment < this->numberOfSegments; ++segment) {
        for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); ++quadrangle)
            if (*std::min_element(quadrangle->cbegin(), quadrangle->cend()-1) >=  segment*this->numberOfVerticesPerSection && *std::max_element(quadrangle->cbegin(), quadrangle->cend()-1) < (segment+2)*this->numberOfVerticesPerSection)
                quadrangle->back() = this->elementShift++;
    }
    boundary->end = this->elementShift;
    std::sort(this->quadrangles.begin(), this->quadrangles.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

    boundary = this->reordered->boundaries.begin() + 1;
    boundary->begin = this->elementShift;
    for (auto triangle = this->reordered->triangles.cbegin(); triangle != this->reordered->triangles.cend(); ++triangle)
        if (triangle->back() >= boundary->begin && triangle->back() < boundary->end) {
            this->triangles.emplace_back(*triangle);
            this->triangles.back().back() = this->elementShift++;
        }

    for (auto quadrangle = this->reordered->quadrangles.cbegin(); quadrangle != this->reordered->quadrangles.cend(); ++quadrangle)
        if (quadrangle->back() >= boundary->begin && quadrangle->back() < boundary->end) {
            this->quadrangles.emplace_back(*quadrangle);
            this->quadrangles.back().back() = this->elementShift++;
        }
    boundary->end = this->elementShift;

    boundary = this->reordered->boundaries.begin() + 2;
    boundary->begin = this->elementShift;
    for (auto triangle = this->reordered->triangles.cbegin(); triangle != this->reordered->triangles.cend(); ++triangle)
        if (triangle->back() >= boundary->begin && triangle->back() < boundary->end) {
            this->triangles.emplace_back(*triangle);
            this->triangles.back().back() = this->elementShift++;
        }

    for (auto quadrangle = this->reordered->quadrangles.cbegin(); quadrangle != this->reordered->quadrangles.cend(); ++quadrangle)
        if (quadrangle->back() >= boundary->begin && quadrangle->back() < boundary->end) {
            this->quadrangles.emplace_back(*quadrangle);
            this->quadrangles.back().back() = this->elementShift++;
        }
    boundary->end = this->elementShift;

    this->reordered->quadrangles = std::move(this->quadrangles);
    this->reordered->triangles = std::move(this->triangles);
}

void RadialGridDataReordered::fixLineIndices() {
    this->reordered->wells[0].begin = this->elementShift;
    for (int s = 0; s < this->numberOfSegments; ++s)
        this->reordered->lines[s].back() = this->elementShift++;
    this->reordered->wells[0].end = this->elementShift;
}
