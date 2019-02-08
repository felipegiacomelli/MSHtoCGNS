#include "MSHtoCGNS/FileMend/SegmentGridExtractor.hpp"
#include <cgnslib.h>

SegmentGridExtractor::SegmentGridExtractor(boost::shared_ptr<GridData> gridData) : gridData(gridData) {
    this->checkGridData();
    this->createSegmentGrid();
    this->defineQuantities();
    this->copyVertices();
    this->copyElements();
    this->copyFacets();
    this->copyLine();
    this->fixRegion();
    this->fixBoundaries();
    this->fixWell();
}

void SegmentGridExtractor::checkGridData() {
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

void SegmentGridExtractor::defineQuantities() {
    this->numberOfSegments = this->gridData->lines.size();
    this->numberOfPrismsPerSegment = this->gridData->prisms.size() / this->numberOfSegments;
    this->numberOfHexahedronsPerSegment = this->gridData->hexahedrons.size() / this->numberOfSegments;
    this->numberOfHexahedronsPerRadius = this->numberOfHexahedronsPerSegment / this->numberOfPrismsPerSegment;
    this->numberOfVerticesPerSection = this->gridData->coordinates.size() / (this->gridData->lines.size() + 1);
}

void SegmentGridExtractor::createSegmentGrid() {
    this->segmentGrid = boost::make_shared<GridData>();
    this->segmentGrid->dimension = this->gridData->dimension;

    this->segmentGrid->boundaries = this->gridData->boundaries;

    this->segmentGrid->regions = this->gridData->regions;

    this->segmentGrid->wells = this->gridData->wells;
}

void SegmentGridExtractor::copyVertices() {
    for (int vertex = 0; vertex < 2 * this->numberOfVerticesPerSection; ++vertex)
        this->segmentGrid->coordinates.emplace_back(this->gridData->coordinates[vertex]);
}

void SegmentGridExtractor::copyElements() {
    for (int prism = 0; prism < this->numberOfPrismsPerSegment; ++prism) {
        this->segmentGrid->prisms.emplace_back(this->gridData->prisms[prism]);
        this->segmentGrid->prisms[prism].back() = this->elementShift++;
    }

    for (int hexahedron = 0; hexahedron < this->numberOfHexahedronsPerSegment; ++hexahedron) {
        this->segmentGrid->hexahedrons.emplace_back(this->gridData->hexahedrons[hexahedron]);
        this->segmentGrid->hexahedrons[hexahedron].back() = this->elementShift++;
    }
}

void SegmentGridExtractor::copyFacets() {
    for (int quadrangle = 0; quadrangle < this->numberOfPrismsPerSegment; ++quadrangle) {
        this->segmentGrid->quadrangles.emplace_back(this->gridData->quadrangles[quadrangle]);
        this->segmentGrid->quadrangles[quadrangle].back() = this->elementShift++;
    }

    for (int triangle = 0; triangle < this->numberOfPrismsPerSegment; ++triangle) {
        this->segmentGrid->triangles.emplace_back(this->gridData->triangles[triangle]);
        this->segmentGrid->triangles[triangle].back() = this->elementShift++;
    }

    for (int quadrangle = this->numberOfPrismsPerSegment * this->numberOfSegments; quadrangle < this->numberOfPrismsPerSegment * this->numberOfSegments + this->numberOfHexahedronsPerSegment; ++quadrangle) {
        this->segmentGrid->quadrangles.emplace_back(this->gridData->quadrangles[quadrangle]);
        this->segmentGrid->quadrangles.back().back() = this->elementShift++;
    }

    for (int triangle = this->numberOfPrismsPerSegment; triangle < 2 * this->numberOfPrismsPerSegment; ++triangle) {
        this->segmentGrid->triangles.emplace_back(this->gridData->triangles[triangle]);
        this->segmentGrid->triangles[triangle].back() = this->elementShift++;
    }

    for (int quadrangle = this->numberOfPrismsPerSegment * this->numberOfSegments + this->numberOfHexahedronsPerSegment; quadrangle < int(this->gridData->quadrangles.size()); ++quadrangle) {
        this->segmentGrid->quadrangles.emplace_back(this->gridData->quadrangles[quadrangle]);
        this->segmentGrid->quadrangles.back().back() = this->elementShift++;
    }
}

void SegmentGridExtractor::copyLine() {
    this->segmentGrid->lines.emplace_back(this->gridData->lines[0]);
    this->segmentGrid->lines[0].back() = this->elementShift++;
}

void SegmentGridExtractor::fixRegion() {
    this->elementShift = this->numberOfPrismsPerSegment + this->numberOfHexahedronsPerSegment;
    this->segmentGrid->regions[0].begin = 0;
    this->segmentGrid->regions[0].end = this->elementShift;
}

void SegmentGridExtractor::fixBoundaries() {
    // cylinder surface
    auto boundary = this->segmentGrid->boundaries.begin();
    boundary->begin = this->elementShift;
    boundary->end = boundary->begin + this->numberOfPrismsPerSegment;
    boundary->vertices.erase(std::remove_if(boundary->vertices.begin(), boundary->vertices.end(), [=](auto v){return v >= 2 * this->numberOfVerticesPerSection;}), boundary->vertices.end());

    // first lid
    boundary = this->segmentGrid->boundaries.begin() + 1;
    boundary->begin = this->elementShift + this->numberOfPrismsPerSegment;
    boundary->end = boundary->begin + this->numberOfPrismsPerSegment + this->numberOfHexahedronsPerSegment;
    std::stable_sort(boundary->vertices.begin(), boundary->vertices.end());

    // last lid
    boundary = this->segmentGrid->boundaries.begin() + 2;
    boundary->begin = this->elementShift + this->numberOfPrismsPerSegment + this->numberOfPrismsPerSegment + this->numberOfHexahedronsPerSegment;
    boundary->end = boundary->begin + this->numberOfPrismsPerSegment + this->numberOfHexahedronsPerSegment;
    std::stable_sort(boundary->vertices.begin(), boundary->vertices.end());

    std::unordered_map<int, int> lastToSecond;
    int shift = 0;
    for (auto& vertex : boundary->vertices) {
        lastToSecond[vertex] = this->numberOfVerticesPerSection + shift++;
        vertex = lastToSecond[vertex];
    }

    for (auto triangle = this->segmentGrid->triangles.begin(); triangle != this->segmentGrid->triangles.end(); ++triangle)
        if (triangle->back() >= boundary->begin && triangle->back() < boundary->end)
            for (auto vertex = triangle->begin(); vertex != triangle->end() - 1; ++vertex)
                *vertex = lastToSecond[*vertex];

    for (auto quadrangle = this->segmentGrid->quadrangles.begin(); quadrangle != this->segmentGrid->quadrangles.end(); ++quadrangle)
        if (quadrangle->back() >= boundary->begin && quadrangle->back() < boundary->end)
            for (auto vertex = quadrangle->begin(); vertex != quadrangle->end() - 1; ++vertex)
                *vertex = lastToSecond[*vertex];
}

void SegmentGridExtractor::fixWell() {
    this->segmentGrid->wells[0].begin = this->segmentGrid->boundaries[2].end;
    this->segmentGrid->wells[0].end = this->segmentGrid->wells[0].begin + 1;
    this->segmentGrid->wells[0].vertices = std::vector<int>{this->segmentGrid->lines[0][0], this->segmentGrid->lines[0][1]};
}
