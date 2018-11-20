#include <FileMend/RadialGridDataReordered.hpp>
#include <cgnslib.h>

RadialGridDataReordered::RadialGridDataReordered(boost::shared_ptr<GridData> gridData) : gridData(gridData) {
    this->checkGridData();
    this->defineQuantities();
    this->createReordered();
    this->reorderBoundaries();
    this->copyData();
    this->reorder();
    this->copyVertices();
    this->fixVerticesIndices();
    this->fixElementIndices();
    this->fixFacetIndices();
    this->fixLineIndices();
}

void RadialGridDataReordered::checkGridData() {
    if (this->gridData->dimension != 3)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be 3 and not " + std::to_string(this->gridData->dimension));

    if (this->gridData->tetrahedronConnectivity.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData tetrahedronConnectivity size must be 0 and not " + std::to_string(this->gridData->tetrahedronConnectivity.size()));

    if (this->gridData->hexahedronConnectivity.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData hexahedronConnectivity size must not be 0");

    if (this->gridData->prismConnectivity.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData prismConnectivity size must not be 0");

    if (this->gridData->pyramidConnectivity.size() != 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData pyramidConnectivity size must be 0 and not " + std::to_string(this->gridData->pyramidConnectivity.size()));

    if (this->gridData->triangleConnectivity.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData triangleConnectivity size must not be 0");

    if (this->gridData->quadrangleConnectivity.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData quadrangleConnectivity size must not be 0");

    if (this->gridData->lineConnectivity.size() == 0u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData lineConnectivity size must not be 0");

    if (this->gridData->boundaries.size() != 3u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData boundaries size must be 1 and not " + std::to_string(this->gridData->boundaries.size()));

    if (this->gridData->regions.size() != 1u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData regions size must be 1 and not " + std::to_string(this->gridData->regions.size()));

    if (this->gridData->wells.size() != 1u)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData wells size must be 1 and not " + std::to_string(this->gridData->wells.size()));
}

void RadialGridDataReordered::defineQuantities() {
    this->numberOfSegments = this->gridData->lineConnectivity.size();
    this->numberOfPrismsPerSegment = this->gridData->prismConnectivity.size() / this->numberOfSegments;
    this->numberOfHexahedronsPerSegment = this->gridData->hexahedronConnectivity.size() / this->numberOfSegments;
    this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;
    this->numberOfVerticesPerSection = this->gridData->coordinates.size() / (this->gridData->lineConnectivity.size() + 1);
}

void RadialGridDataReordered::createReordered() {
    this->reordered = boost::make_shared<GridData>();
    this->reordered->dimension = this->gridData->dimension;

    this->reordered->triangleConnectivity = this->gridData->triangleConnectivity;
    this->reordered->quadrangleConnectivity = this->gridData->quadrangleConnectivity;
    this->reordered->lineConnectivity = this->gridData->lineConnectivity;

    this->reordered->boundaries = this->gridData->boundaries;

    this->reordered->regions = this->gridData->regions;

    this->reordered->wells = this->gridData->wells;
}

void RadialGridDataReordered::reorderBoundaries() {
    int firstVertex = this->gridData->lineConnectivity.front()[0];
    int lastVertex  = this->gridData->lineConnectivity.back()[1];
    auto boundary = std::find_if(this->reordered->boundaries.begin(), this->reordered->boundaries.end(),
                                    [=](auto b){return !hasElement(b.vertices.cbegin(), b.vertices.cend(), firstVertex) && !hasElement(b.vertices.cbegin(), b.vertices.cend(), lastVertex);});
    std::iter_swap(this->reordered->boundaries.begin(), boundary);

    boundary = std::find_if(this->reordered->boundaries.begin(), this->reordered->boundaries.end(), [=](auto b){return hasElement(b.vertices.cbegin(), b.vertices.cend(), firstVertex);});
    std::iter_swap(this->reordered->boundaries.begin() + 1, boundary);

    boundary = std::find_if(this->reordered->boundaries.begin(), this->reordered->boundaries.end(), [=](auto b){return hasElement(b.vertices.cbegin(), b.vertices.cend(), lastVertex);});
    std::iter_swap(this->reordered->boundaries.begin() + 2, boundary);
}

void RadialGridDataReordered::copyData() {
    this->hexahedra = this->gridData->hexahedronConnectivity;
    for (auto i = 0u; i < this->hexahedra.size(); i++)
        this->hexahedra[i].back() = i;

    this->prisms = this->gridData->prismConnectivity;
    for (auto i = 0u; i < this->prisms.size(); i++)
        this->prisms[i].back() = i;

    auto boundary = this->reordered->boundaries.begin() + 1;

    for (auto triangle = this->gridData->triangleConnectivity.cbegin(); triangle != this->gridData->triangleConnectivity.cend(); triangle++)
        if (triangle->back() >= boundary->facetBegin && triangle->back() < boundary->facetEnd)
            this->triangles.emplace_back(*triangle);

    for (auto quadrangle = this->gridData->quadrangleConnectivity.cbegin(); quadrangle != this->gridData->quadrangleConnectivity.cend(); quadrangle++)
        if (quadrangle->back() >= boundary->facetBegin && quadrangle->back() < boundary->facetEnd)
            this->quadrangles.emplace_back(*quadrangle);
}

void RadialGridDataReordered::reorder() {
    this->buildFirstSection();
    for (int segment = 0; segment < this->numberOfSegments; segment++) {
        this->vertexShift = 0;
        for (auto triangle = this->triangles.begin(); triangle != this->triangles.end(); triangle++)
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
                    prism++;

        for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); quadrangle++)
            for (auto hexahedron = this->hexahedra.begin(); hexahedron != this->hexahedra.end();)
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
                    hexahedron++;
    }
}

void RadialGridDataReordered::buildFirstSection() {
    for (auto triangle = this->triangles.begin(); triangle != this->triangles.end(); triangle++)
        for (auto prism = this->prisms.begin(); prism != this->prisms.end();)
            if (hasElements(prism->cbegin(), prism->cend()-1, triangle->cbegin(), triangle->cend()-1)) {
                this->addVertex((*prism)[0], 0);
                this->addVertex((*prism)[1], 0);
                this->addVertex((*prism)[2], 0);
                break;
            }
            else
                prism++;

    for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); quadrangle++)
        for (auto hexahedron = this->hexahedra.begin(); hexahedron != this->hexahedra.end();)
            if (hasElements(hexahedron->cbegin(), hexahedron->cend()-1, quadrangle->cbegin(), quadrangle->cend()-1)) {
                this->addVertex((*hexahedron)[0], 0);
                this->addVertex((*hexahedron)[1], 0);
                this->addVertex((*hexahedron)[5], 0);
                this->addVertex((*hexahedron)[4], 0);
                break;
            }
            else
                hexahedron++;
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
    this->reordered->prismConnectivity.push_back(this->gridData->prismConnectivity[prism->back()]);
    prism = this->prisms.erase(prism);
}

void RadialGridDataReordered::copyHexahedron(std::vector<std::array<int, 9>>::iterator hexahedron) {
    this->reordered->hexahedronConnectivity.push_back(this->gridData->hexahedronConnectivity[hexahedron->back()]);
    hexahedron = this->hexahedra.erase(hexahedron);
}

void RadialGridDataReordered::copyVertices() {
    std::stable_sort(this->vertices.begin(), this->vertices.end(), [](auto a, auto b) {return a.second < b.second;});
    for (auto vertex : this->vertices)
        this->reordered->coordinates.emplace_back(this->gridData->coordinates[vertex.first]);
}

void RadialGridDataReordered::fixVerticesIndices() {
    std::unordered_map<int, int> originalToFinal;
    for (auto vertex : this->vertices)
        originalToFinal[vertex.first] = vertex.second;

    for (auto& hexahedron : this->reordered->hexahedronConnectivity)
        for (auto vertex = hexahedron.begin(); vertex != hexahedron.end() - 1; vertex++)
            *vertex = originalToFinal[*vertex];

    for (auto& prism : this->reordered->prismConnectivity)
        for (auto vertex = prism.begin(); vertex != prism.end() - 1; vertex++)
            *vertex = originalToFinal[*vertex];

    for (auto& triangle : this->reordered->triangleConnectivity)
        for (auto vertex = triangle.begin(); vertex != triangle.end() - 1; vertex++)
            *vertex = originalToFinal[*vertex];

    for (auto& quadrangle : this->reordered->quadrangleConnectivity)
        for (auto vertex = quadrangle.begin(); vertex != quadrangle.end() - 1; vertex++)
            *vertex = originalToFinal[*vertex];

    for (auto& line : this->reordered->lineConnectivity)
        for (auto vertex = line.begin(); vertex != line.end() - 1; vertex++)
            *vertex = originalToFinal[*vertex];

    for (auto& boundary : this->reordered->boundaries) {
        for (auto& vertex : boundary.vertices)
            vertex = originalToFinal[vertex];
        std::stable_sort(boundary.vertices.begin(), boundary.vertices.end());
    }

    for (auto& vertex : this->reordered->wells[0].vertices)
        vertex = originalToFinal[vertex];
    std::stable_sort(this->reordered->wells[0].vertices.begin(), this->reordered->wells[0].vertices.end());
}

void RadialGridDataReordered::fixElementIndices() {
    for (int s = 0; s < this->numberOfSegments; s++) {
        for (int p = 0; p < this->numberOfPrismsPerSegment; p++)
            this->reordered->prismConnectivity[s * this->numberOfPrismsPerSegment + p].back() = this->elementShift++;

        for (int h = 0; h < this->numberOfHexahedronsPerSegment; h++)
            this->reordered->hexahedronConnectivity[s * this->numberOfHexahedronsPerSegment + h].back() = this->elementShift++;
    }
}

void RadialGridDataReordered::fixFacetIndices() {
    this->quadrangles.clear();
    this->triangles.clear();

    auto boundary = this->reordered->boundaries.begin();
    for (auto quadrangle = this->reordered->quadrangleConnectivity.cbegin(); quadrangle != this->reordered->quadrangleConnectivity.cend(); quadrangle++)
        if (quadrangle->back() >= boundary->facetBegin && quadrangle->back() < boundary->facetEnd)
            this->quadrangles.emplace_back(*quadrangle);

    boundary->facetBegin = this->elementShift;
    for (int segment = 0; segment < this->numberOfSegments; segment++) {
        for (auto quadrangle = this->quadrangles.begin(); quadrangle != this->quadrangles.end(); quadrangle++)
            if (*std::min_element(quadrangle->cbegin(), quadrangle->cend()-1) >=  segment*this->numberOfVerticesPerSection && *std::max_element(quadrangle->cbegin(), quadrangle->cend()-1) < (segment+2)*this->numberOfVerticesPerSection)
                quadrangle->back() = this->elementShift++;
    }
    boundary->facetEnd = this->elementShift;
    std::stable_sort(this->quadrangles.begin(), this->quadrangles.end(), [](const auto& a, const auto& b) {return a.back() < b.back();});

    boundary = this->reordered->boundaries.begin() + 1;
    boundary->facetBegin = this->elementShift;
    for (auto triangle = this->reordered->triangleConnectivity.cbegin(); triangle != this->reordered->triangleConnectivity.cend(); triangle++)
        if (triangle->back() >= boundary->facetBegin && triangle->back() < boundary->facetEnd) {
            this->triangles.emplace_back(*triangle);
            this->triangles.back().back() = this->elementShift++;
        }

    for (auto quadrangle = this->reordered->quadrangleConnectivity.cbegin(); quadrangle != this->reordered->quadrangleConnectivity.cend(); quadrangle++)
        if (quadrangle->back() >= boundary->facetBegin && quadrangle->back() < boundary->facetEnd) {
            this->quadrangles.emplace_back(*quadrangle);
            this->quadrangles.back().back() = this->elementShift++;
        }
    boundary->facetEnd = this->elementShift;

    boundary = this->reordered->boundaries.begin() + 2;
    boundary->facetBegin = this->elementShift;
    for (auto triangle = this->reordered->triangleConnectivity.cbegin(); triangle != this->reordered->triangleConnectivity.cend(); triangle++)
        if (triangle->back() >= boundary->facetBegin && triangle->back() < boundary->facetEnd) {
            this->triangles.emplace_back(*triangle);
            this->triangles.back().back() = this->elementShift++;
        }

    for (auto quadrangle = this->reordered->quadrangleConnectivity.cbegin(); quadrangle != this->reordered->quadrangleConnectivity.cend(); quadrangle++)
        if (quadrangle->back() >= boundary->facetBegin && quadrangle->back() < boundary->facetEnd) {
            this->quadrangles.emplace_back(*quadrangle);
            this->quadrangles.back().back() = this->elementShift++;
        }
    boundary->facetEnd = this->elementShift;

    this->reordered->quadrangleConnectivity = std::move(this->quadrangles);
    this->reordered->triangleConnectivity = std::move(this->triangles);
}

void RadialGridDataReordered::fixLineIndices() {
    this->reordered->wells[0].lineBegin = this->elementShift;
    for (int s = 0; s < this->numberOfSegments; s++)
        this->reordered->lineConnectivity[s].back() = this->elementShift++;
    this->reordered->wells[0].lineEnd = this->elementShift;
}