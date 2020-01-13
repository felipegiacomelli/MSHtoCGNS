#include "MSHtoCGNS/Manipulation/SegmentExtractor.hpp"
#include <cgnslib.h>

SegmentExtractor::SegmentExtractor(boost::shared_ptr<GridData> gridData) : gridData(gridData) {
    this->checkGridData();
    this->createSegmentGrid();
    this->defineQuantities();
    this->copyVertices();
    this->copyElements();
    this->findVertices(this->segment->sections);
}

void SegmentExtractor::checkGridData() {
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

void SegmentExtractor::defineQuantities() {
    this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;
    this->numberOfVerticesPerSection = this->gridData->coordinates.size() / (this->numberOfSegments + 1);
}

void SegmentExtractor::createSegmentGrid() {
    this->segment = boost::make_shared<GridData>();
    this->segment->dimension = this->gridData->dimension;
    this->segment->sections = this->gridData->sections;
}

void SegmentExtractor::copyVertices() {
    for (int vertex = 0; vertex < 2 * this->numberOfVerticesPerSection; ++vertex)
        this->segment->coordinates.emplace_back(this->gridData->coordinates[vertex]);
}

void SegmentExtractor::copyElements() {
    int shift = 0;
    this->segment->sections[0].begin = shift;
    for (int element = 0; element < this->numberOfPrismsPerSegment + this->numberOfHexahedronsPerSegment; ++element) {
        this->segment->connectivities.emplace_back(this->gridData->connectivities[element]);
        this->segment->connectivities.back().back() = shift++;
    }
    this->segment->sections[0].end = shift;

    this->segment->sections[1].begin = shift;
    auto position = std::find_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), [](const auto& c){return c[0] == QUAD_4;});
    for (auto quadrangle = 0; quadrangle < this->numberOfPrismsPerSegment; quadrangle++) {
        this->segment->connectivities.emplace_back(*position++);
        this->segment->connectivities.back().back() = shift++;
    }
    this->segment->sections[1].end = shift;

    this->segment->sections[2].begin = shift;
    for (auto facet = this->gridData->sections[2].begin; facet < this->gridData->sections[2].end; ++facet) {
        this->segment->connectivities.emplace_back(this->gridData->connectivities[facet]);
        this->segment->connectivities.back().back() = shift++;
    }
    this->segment->sections[2].end = shift;

    this->segment->sections[3].begin = shift;
    for (auto facet = this->gridData->sections[2].begin; facet < this->gridData->sections[2].end; ++facet) {
        auto connectivity(this->gridData->connectivities[facet]);
        for (auto vertex = connectivity.begin() + 1; vertex != connectivity.end() - 1; ++vertex) {
            *vertex = *vertex + this->numberOfVerticesPerSection;
        }
        connectivity.back() = shift++;
        this->segment->connectivities.emplace_back(connectivity);
    }
    this->segment->sections[3].end = shift;

    this->segment->sections[4].begin = shift;
    this->segment->connectivities.emplace_back(std::vector<int>{BAR_2, 0, this->numberOfVerticesPerSection, shift++});
    this->segment->sections[4].end = shift;
}

void SegmentExtractor::findVertices(std::vector<SectionData>& sections) {
    for (auto& section : sections) {
        std::set<int> vertices;
        for (auto position = this->segment->connectivities.cbegin() + section.begin; position != this->segment->connectivities.cbegin() + section.end; ++position) {
            vertices.insert(position->cbegin() + 1, position->cend() - 1);
        }
        section.vertices = std::vector<int>{vertices.begin(), vertices.end()};
    }
}
