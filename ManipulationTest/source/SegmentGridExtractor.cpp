#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"
#include "MSHtoCGNS/Manipulation/RadialGridDataReordered.hpp"
#include "MSHtoCGNS/Manipulation/SegmentGridExtractor.hpp"
#include <cmath>

#define TOLERANCE 1e-4

struct SegmentGridExtractorFixture {
    SegmentGridExtractorFixture() {
        CgnsReader cgnsReader(this->inputPath);
        RadialGridDataReordered radialGridDataReordered(cgnsReader.gridData);
        this->gridData = radialGridDataReordered.reordered;
    }

    double calculateDistance(std::array<double, 3> a, std::array<double, 3> b) {
        return std::sqrt(std::inner_product(a.cbegin(), a.cend(), b.cbegin(), 0.0, std::plus<>(), [](auto c, auto d){return (c-d)*(c-d);}));
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/RadialGridDataReordered/370v_324e.cgns";
    boost::shared_ptr<GridData> gridData;
};

FixtureTestSuite(SegmentGridExtractorSuite, SegmentGridExtractorFixture)

TestCase(SegmentGridExtractorTest) {
    SegmentGridExtractor segmentGridExtractor(this->gridData);

    auto segment = segmentGridExtractor.segment;

    checkEqual(segment->coordinates.size(), 74u);

    checkEqual(segment->tetrahedrons.size(), 0u);
    checkEqual(segment->hexahedrons.size(), 24u);
    checkEqual(segment->prisms.size(), 12u);
    checkEqual(segment->pyramids.size(), 0u);

    checkEqual(segment->triangles.size(), 24u);
    checkEqual(segment->quadrangles.size(), 60u);

    checkEqual(segment->lines.size(), 1u);

    checkEqual(segment->boundaries.size(), 3u);
    checkEqual(segment->regions.size(), 1u);
    checkEqual(segment->wells.size(), 1u);

    auto boundary = segment->boundaries[0];
    checkEqual(boundary.begin, 36);
    checkEqual(boundary.end, 48);
    checkEqual(boundary.vertices.size(), 24u);
    check(std::all_of(boundary.vertices.cbegin(), boundary.vertices.cend(), [=](auto v){return v >= 0 && v < 74;}));

    boundary = segment->boundaries[1];
    checkEqual(boundary.begin, 48);
    checkEqual(boundary.end, 84);
    checkEqual(boundary.vertices.size(), 37u);
    check(std::all_of(boundary.vertices.cbegin(), boundary.vertices.cend(), [=](auto v){return v >= 0 && v < 37;}));

    boundary = segment->boundaries[2];
    checkEqual(boundary.begin,  84);
    checkEqual(boundary.end, 120);
    checkEqual(boundary.vertices.size(), 37u);
    check(std::all_of(boundary.vertices.cbegin(), boundary.vertices.cend(), [=](auto v){return v >= 37 && v < 74;}));

    checkEqual(segment->wells[0].begin, 120);
    checkEqual(segment->wells[0].end, 121);
    checkEqual(segment->wells[0].vertices.size(), 2u);
    checkEqual(segment->wells[0].vertices[0],  0);
    checkEqual(segment->wells[0].vertices[1], 37);

    for (int j = 0; j < 12; ++j)
        checkEqual(segment->prisms[j].back(), j);

    for (int j = 0; j < 24; ++j)
        checkEqual(segment->hexahedrons[j].back(), 12 + j);

    for (int j = 0; j < 12; ++j)
        checkEqual(segment->quadrangles[j].back(), 36 + j);

    for (int j = 0; j < 12; ++j) {
        checkEqual(segment->triangles[j     ].back(), 48 + j);
        checkEqual(segment->triangles[j + 12].back(), 84 + j);
    }

    for (int j = 0; j < 24; ++j) {
        checkEqual(segment->quadrangles[j + 12].back(), 60 + j);
        checkEqual(segment->quadrangles[j + 36].back(), 96 + j);
    }

    checkEqual(segment->lines[0].back(), 120);

    for (int j = 0; j < 12; ++j)
        check(std::all_of(segment->triangles[j + 12].cbegin(), segment->triangles[j + 12].cend() - 1, [=](auto v){return v >= 37 && v < 74;}));

    for (int j = 0; j < 24; ++j)
        check(std::all_of(segment->quadrangles[j + 36].cbegin(), segment->quadrangles[j + 36].cend() - 1, [=](auto v){return v >= 37 && v < 74;}));

    for (int j = 0; j < 37; ++j) {
        checkSmall(segment->coordinates[37 * 0 + j][2], TOLERANCE);
        checkClose(segment->coordinates[37 * 1 + j][2], 5.5555555, TOLERANCE);
    }

    for (int j = 0; j < 37; ++j)
        checkClose(fabs(segment->coordinates[37 * 0 + j][2] - segment->coordinates[37 *  1 + j][2]), 5.5555555555555554e+00, TOLERANCE);

    for (int j = 0; j < 37; ++j)
        checkClose(calculateDistance(segment->coordinates[37 * 0 + j], segment->coordinates[37 *  1 + j]), 5.5555555555555554e+00, TOLERANCE);
}

TestSuiteEnd()
