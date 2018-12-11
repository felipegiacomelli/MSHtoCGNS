#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include "MSHtoCGNS/FileMend/RadialGridDataReordered.hpp"
#include "MSHtoCGNS/FileMend/SegmentGridExtractor.hpp"

#define TOLERANCE 1e-4

struct SegmentGridExtractorFixture {
    SegmentGridExtractorFixture() {
        CgnsReader3D cgnsReader3D(this->inputPath);
        RadialGridDataReordered radialGridDataReordered(cgnsReader3D.gridData);
        this->gridData = radialGridDataReordered.reordered;
    }

    double calculateDistance(std::array<double, 3> a, std::array<double, 3> b) {
        return std::sqrt(std::inner_product(a.cbegin(), a.cend(), b.cbegin(), 0.0, std::plus<>(), [](auto c, auto d){return (c-d)*(c-d);}));
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/RadialGridDataReordered/370v_324e.cgns";
    boost::shared_ptr<GridData> gridData;
};

FixtureTestSuite(SegmentGridExtractorSuite, SegmentGridExtractorFixture)

TestCase(SegmentGridExtractorTest) {
    SegmentGridExtractor segmentGridExtractor(this->gridData);

    auto segmentGrid = segmentGridExtractor.segmentGrid;

    checkEqual(segmentGrid->coordinates.size(), 74u);

    checkEqual(segmentGrid->tetrahedronConnectivity.size(), 0u);
    checkEqual(segmentGrid->hexahedronConnectivity.size(), 24u);
    checkEqual(segmentGrid->prismConnectivity.size(), 12u);
    checkEqual(segmentGrid->pyramidConnectivity.size(), 0u);

    checkEqual(segmentGrid->triangleConnectivity.size(), 24u);
    checkEqual(segmentGrid->quadrangleConnectivity.size(), 60u);

    checkEqual(segmentGrid->lineConnectivity.size(), 1u);

    checkEqual(segmentGrid->boundaries.size(), 3u);
    checkEqual(segmentGrid->regions.size(), 1u);
    checkEqual(segmentGrid->wells.size(), 1u);

    auto boundary = segmentGrid->boundaries[0];
    checkEqual(boundary.begin, 36);
    checkEqual(boundary.end, 48);
    checkEqual(boundary.vertices.size(), 24u);
    check(std::all_of(boundary.vertices.cbegin(), boundary.vertices.cend(), [=](auto v){return v >= 0 && v < 74;}));

    boundary = segmentGrid->boundaries[1];
    checkEqual(boundary.begin, 48);
    checkEqual(boundary.end, 84);
    checkEqual(boundary.vertices.size(), 37u);
    check(std::all_of(boundary.vertices.cbegin(), boundary.vertices.cend(), [=](auto v){return v >= 0 && v < 37;}));

    boundary = segmentGrid->boundaries[2];
    checkEqual(boundary.begin,  84);
    checkEqual(boundary.end, 120);
    checkEqual(boundary.vertices.size(), 37u);
    check(std::all_of(boundary.vertices.cbegin(), boundary.vertices.cend(), [=](auto v){return v >= 37 && v < 74;}));

    checkEqual(segmentGrid->wells[0].begin, 120);
    checkEqual(segmentGrid->wells[0].end, 121);
    checkEqual(segmentGrid->wells[0].vertices.size(), 2u);
    checkEqual(segmentGrid->wells[0].vertices[0],  0);
    checkEqual(segmentGrid->wells[0].vertices[1], 37);

    for (int j = 0; j < 12; j++)
        checkEqual(segmentGrid->prismConnectivity[j].back(), j);

    for (int j = 0; j < 24; j++)
        checkEqual(segmentGrid->hexahedronConnectivity[j].back(), 12 + j);

    for (int j = 0; j < 12; j++)
        checkEqual(segmentGrid->quadrangleConnectivity[j].back(), 36 + j);

    for (int j = 0; j < 12; j++) {
        checkEqual(segmentGrid->triangleConnectivity[j     ].back(), 48 + j);
        checkEqual(segmentGrid->triangleConnectivity[j + 12].back(), 84 + j);
    }

    for (int j = 0; j < 24; j++) {
        checkEqual(segmentGrid->quadrangleConnectivity[j + 12].back(), 60 + j);
        checkEqual(segmentGrid->quadrangleConnectivity[j + 36].back(), 96 + j);
    }

    checkEqual(segmentGrid->lineConnectivity[0].back(), 120);

    for (int j = 0; j < 12; j++)
        check(std::all_of(segmentGrid->triangleConnectivity[j + 12].cbegin(), segmentGrid->triangleConnectivity[j + 12].cend() - 1, [=](auto v){return v >= 37 && v < 74;}));

    for (int j = 0; j < 24; j++)
        check(std::all_of(segmentGrid->quadrangleConnectivity[j + 36].cbegin(), segmentGrid->quadrangleConnectivity[j + 36].cend() - 1, [=](auto v){return v >= 37 && v < 74;}));

    for (int j = 0; j < 37; j++) {
        checkSmall(segmentGrid->coordinates[37 * 0 + j][2], TOLERANCE);
        checkClose(segmentGrid->coordinates[37 * 1 + j][2], 5.5555555, TOLERANCE);
    }

    for (int j = 0; j < 37; j++)
        checkClose(std::abs(segmentGrid->coordinates[37 * 0 + j][2] - segmentGrid->coordinates[37 *  1 + j][2]), 5.5555555555555554e+00, TOLERANCE);

    for (int j = 0; j < 37; j++)
        checkClose(calculateDistance(segmentGrid->coordinates[37 * 0 + j], segmentGrid->coordinates[37 *  1 + j]), 5.5555555555555554e+00, TOLERANCE);
}

TestSuiteEnd()
