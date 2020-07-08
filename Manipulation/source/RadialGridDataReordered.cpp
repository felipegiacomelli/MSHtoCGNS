#include "MSHtoCGNS/Manipulation/RadialGridDataReordered.hpp"
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
}

void RadialGridDataReordered::checkGridData() {
    if (this->gridData->dimension != 3)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

    const auto& cs = this->gridData->connectivities;

    this->numberOfSegments = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == BAR_2;});
    if (this->numberOfSegments == 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData lines size must not be 0");

    if (std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TRI_3;}) == 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData triangles size must not be 0");

    if (std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == QUAD_4;}) == 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData quadrangles size must not be 0");

    if (std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TETRA_4;}) != 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData tetrahedrons size must be 0");

    this->numberOfHexahedronsPerSegment = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == HEXA_8;}) / this->numberOfSegments;
    if (this->numberOfHexahedronsPerSegment == 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData hexahedrons size must NOT be 0");

    this->numberOfPrismsPerSegment = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == PENTA_6;}) / this->numberOfSegments;
    if (this->numberOfPrismsPerSegment == 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData prisms size must NOT be 0");

    if (std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == PYRA_5;}) != 0)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData pyramids size must be 0");

    if (std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 3;}) != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData regions size must be 1");

    if (std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 2;}) != 3)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData boundaries size must be 3");

    if (std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 1;}) != 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData wells size must be 1");
}

void RadialGridDataReordered::defineQuantities() {
    this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;
    this->numberOfVerticesPerSection = this->gridData->coordinates.size() / (this->numberOfSegments + 1);
}

void RadialGridDataReordered::createReordered() {
    this->reordered = boost::make_shared<GridData>();
    this->reordered->dimension = this->gridData->dimension;
    this->reordered->sections = this->gridData->sections;
}

void RadialGridDataReordered::reorderBoundaries() {
    int first = this->gridData->connectivities[this->gridData->sections.back().begin][1];
    int last  = this->gridData->connectivities[this->gridData->sections.back().end - 1][2];
    auto boundary = std::find_if(this->reordered->sections.begin(), this->reordered->sections.end(),
                                    [=](auto b){return b.dimension == 2 && !hasElement(b.vertices.cbegin(), b.vertices.cend(), first) && !hasElement(b.vertices.cbegin(), b.vertices.cend(), last);});
    std::iter_swap(this->reordered->sections.begin() + 1, boundary);

    boundary = std::find_if(this->reordered->sections.begin(), this->reordered->sections.end(),
                                [=](auto b){return b.dimension == 2 && hasElement(b.vertices.cbegin(), b.vertices.cend(), first);});
    std::iter_swap(this->reordered->sections.begin() + 2, boundary);

    boundary = std::find_if(this->reordered->sections.begin(), this->reordered->sections.end(),
                                [=](auto b){return b.dimension == 2 && hasElement(b.vertices.cbegin(), b.vertices.cend(), last);});
    std::iter_swap(this->reordered->sections.begin() + 3, boundary);
}

void RadialGridDataReordered::copyData() {
    std::copy_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), std::back_inserter(this->hexahedrons), [](const auto& c){return c[0] == HEXA_8;});
    std::copy_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), std::back_inserter(this->prisms), [](const auto& c){return c[0] == PENTA_6;});

    auto begin = this->gridData->sections[2].begin;
    auto end = this->gridData->sections[2].end;
    for (auto connectivity = this->gridData->connectivities.cbegin() + begin; connectivity != this->gridData->connectivities.cbegin() + end; ++connectivity)
        if ((*connectivity)[0] == TRI_3)
            this->triangles.emplace_back(*connectivity);
        else if ((*connectivity)[0] == QUAD_4)
            this->quadrangles.emplace_back(*connectivity);
}

void RadialGridDataReordered::reorder() {
    this->buildFirstSection();
    for (int segment = 1; segment <= this->numberOfSegments; ++segment) {
        this->vertexShift = 0;
        for (auto triangle = this->triangles.begin(); triangle != this->triangles.end(); ++triangle)
            for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
                if (hasElements(prism->cbegin() + 1, prism->cend() - 1, triangle->cbegin() + 1, triangle->cend() - 1)) {
                    this->addVertex((*prism)[4], segment);
                    this->addVertex((*prism)[5], segment);
                    this->addVertex((*prism)[6], segment);
                    this->updateTriangle(prism, triangle);
                    this->copyPrism(prism);
                    break;
                }
                else
                    ++prism;

        for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); ++quadrangle)
            for (auto hexahedron = this->hexahedrons.begin(); hexahedron != this->hexahedrons.end();)
                if (hasElements(hexahedron->cbegin() + 1, hexahedron->cend() - 1, quadrangle->cbegin() + 1, quadrangle->cend() - 1)) {
                    this->addVertex((*hexahedron)[4], segment);
                    this->addVertex((*hexahedron)[3], segment);
                    this->addVertex((*hexahedron)[7], segment);
                    this->addVertex((*hexahedron)[8], segment);
                    this->updateQuadrangle(hexahedron, quadrangle);
                    this->copyHexahedron(hexahedron);
                    break;
                }
                else
                    ++hexahedron;
    }

    for (auto section = this->reordered->sections.cbegin() + 1; section != this->reordered->sections.cend(); ++section)
        std::copy(this->gridData->connectivities.cbegin() + section->begin, this->gridData->connectivities.cbegin() + section->end, std::back_inserter(this->reordered->connectivities));
}

void RadialGridDataReordered::buildFirstSection() {
    for (auto triangle = this->triangles.begin(); triangle != this->triangles.end(); ++triangle)
        for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
            if (hasElements(prism->cbegin() + 1, prism->cend() - 1, triangle->cbegin() + 1, triangle->cend() - 1)) {
                this->addVertex((*prism)[1], 0);
                this->addVertex((*prism)[2], 0);
                this->addVertex((*prism)[3], 0);
                break;
            }
            else
                ++prism;

    for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); ++quadrangle)
        for (auto hexahedron = this->hexahedrons.begin(); hexahedron != this->hexahedrons.end();)
            if (hasElements(hexahedron->cbegin() + 1, hexahedron->cend() - 1, quadrangle->cbegin() + 1, quadrangle->cend() - 1)) {
                this->addVertex((*hexahedron)[1], 0);
                this->addVertex((*hexahedron)[2], 0);
                this->addVertex((*hexahedron)[6], 0);
                this->addVertex((*hexahedron)[5], 0);
                break;
            }
            else
                ++hexahedron;
}

void RadialGridDataReordered::addVertex(int vertex, int section) {
    if (std::find_if(this->vertices.cbegin(), this->vertices.cend(), [=](auto vh){return vh.first == vertex;}) == this->vertices.cend())
        this->vertices.emplace_back(std::make_pair(vertex, section * this->numberOfVerticesPerSection + this->vertexShift++));
}

void RadialGridDataReordered::updateTriangle(std::vector<std::vector<int>>::iterator prism, std::vector<std::vector<int>>::iterator triangle) {
    (*triangle)[1] = (*prism)[4];
    (*triangle)[2] = (*prism)[5];
    (*triangle)[3] = (*prism)[6];
}

void RadialGridDataReordered::updateQuadrangle(std::vector<std::vector<int>>::iterator hexahedron, std::vector<std::vector<int>>::iterator quadrangle) {
    (*quadrangle)[1] = (*hexahedron)[3];
    (*quadrangle)[2] = (*hexahedron)[4];
    (*quadrangle)[3] = (*hexahedron)[8];
    (*quadrangle)[4] = (*hexahedron)[7];
}

void RadialGridDataReordered::copyPrism(std::vector<std::vector<int>>::iterator prism) {
    this->reordered->connectivities.emplace_back(this->gridData->connectivities[prism->back()]);
    prism = this->prisms.erase(prism);
}

void RadialGridDataReordered::copyHexahedron(std::vector<std::vector<int>>::iterator hexahedron) {
    this->reordered->connectivities.emplace_back(this->gridData->connectivities[hexahedron->back()]);
    hexahedron = this->hexahedrons.erase(hexahedron);
}

void RadialGridDataReordered::copyVertices() {
    std::sort(this->vertices.begin(), this->vertices.end(), [](auto a, auto b){return a.second < b.second;});
    for (auto vertex : this->vertices)
        this->reordered->coordinates.emplace_back(this->gridData->coordinates[vertex.first]);
}

void RadialGridDataReordered::rectifyConnectivities() {
    boost::unordered_map<int, int> originalToReordered;
    for (auto vertex : this->vertices)
        originalToReordered[vertex.first] = vertex.second;

    for (auto& connectivity : this->reordered->connectivities)
        for (auto index = connectivity.begin() + 1; index != connectivity.end() - 1; ++index)
            *index = originalToReordered[*index];

    auto boundary = this->reordered->sections[1];
    auto quadrangle = std::find_if(this->reordered->connectivities.begin(), this->reordered->connectivities.end(), [](const auto& c){return c[0] == QUAD_4;});
    int shift = std::distance(this->reordered->connectivities.begin(), quadrangle);
    for (int segment = 0; segment < this->numberOfSegments; ++segment) {
        for (auto facet = this->reordered->connectivities.begin() + shift; facet != this->reordered->connectivities.begin() + boundary.end; ++facet) {
            auto [min, max] = std::minmax_element(facet->cbegin() + 1, facet->cend() - 1);
            if (*min >=  segment*this->numberOfVerticesPerSection && *max < (segment+2)*this->numberOfVerticesPerSection) {
                facet->back() = shift++;
                std::iter_swap(facet, quadrangle++);
            }
        }
    }

    for (auto& sections : this->reordered->sections) {
        for (auto& vertex : sections.vertices) {
            vertex = originalToReordered[vertex];
        }
        std::sort(sections.vertices.begin(), sections.vertices.end());
    }
}

void RadialGridDataReordered::fixElementIndices() {
    for (auto& connectivity : this->reordered->connectivities)
        connectivity.back() = this->elementShift++;
}
