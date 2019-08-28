#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include "MSHtoCGNS/Manipulation/RadialGridDataReordered.hpp"
#include <cmath>

#define TOLERANCE 1e-4

struct RadialGridDataReorderedFixture {
    RadialGridDataReorderedFixture() {
        CgnsReader cgnsReader(this->inputPath);
        this->gridData = cgnsReader.gridData;
    }

    double calculateDistance(std::array<double, 3> a, std::array<double, 3> b) {
        return std::sqrt(std::inner_product(a.cbegin(), a.cend(), b.cbegin(), 0.0, std::plus<>(), [](auto c, auto d){return (c-d)*(c-d);}));
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/RadialGridDataReordered/370v_324e.cgns";
    boost::shared_ptr<GridData> gridData;
};

FixtureTestSuite(RadialGridDataReorderedSuite, RadialGridDataReorderedFixture)

TestCase(RadialGridDataReorderedTest) {
    RadialGridDataReordered radialGridDataReordered(this->gridData);

    auto reordered = radialGridDataReordered.reordered;

    checkEqual(reordered->coordinates.size(), 370u);

    checkEqual(reordered->tetrahedrons.size(), 0u);
    checkEqual(reordered->hexahedrons.size(), 216u);
    checkEqual(reordered->prisms.size(), 108u);
    checkEqual(reordered->pyramids.size(), 0u);

    checkEqual(reordered->triangles.size(),  24u);
    checkEqual(reordered->quadrangles.size(), 156u);

    checkEqual(reordered->lines.size(), 9u);

    checkEqual(reordered->boundaries.size(), 3u);
    checkEqual(reordered->regions.size(), 1u);
    checkEqual(reordered->wells.size(), 1u);

    checkEqual(reordered->boundaries[0].begin, 324);
    checkEqual(reordered->boundaries[0].end, 432);

    checkEqual(reordered->boundaries[1].begin, 432);
    checkEqual(reordered->boundaries[1].end, 468);

    checkEqual(reordered->boundaries[2].begin, 468);
    checkEqual(reordered->boundaries[2].end, 504);

    checkEqual(reordered->wells[0].begin, 504);
    checkEqual(reordered->wells[0].end, 513);
    checkEqual(reordered->wells[0].vertices.size(), 10u);
    for (int j = 0; j < 10; ++j)
        checkEqual(reordered->wells[0].vertices[j], j * 37);

    for (int i = 0; i < 9; ++i) {
        int shift = 36 * i;

        for (int j = 0; j < 12; ++j)
            checkEqual(reordered->prisms[12*i + j].back(), shift++);

        for (int j = 0; j < 24; ++j)
            checkEqual(reordered->hexahedrons[24*i + j].back(), shift++);
    }

    for (int i = 0; i < 108; ++i) {
        checkEqual(reordered->quadrangles[i].back(), 324 + i);
    }

    for (int i = 0; i < 12; ++i) {
        checkEqual(reordered->triangles[i     ].back(), 432 + i);
        checkEqual(reordered->triangles[i + 12].back(), 468 + i);
    }

    for (int i = 0; i < 24; ++i) {
        checkEqual(reordered->quadrangles[i + 108].back(), 444 + i);
        checkEqual(reordered->quadrangles[i + 132].back(), 480 + i);
    }

    for (int i = 0; i < 9; ++i)
        checkEqual(reordered->lines[i].back(), 504 + i);

    for (int i = 0; i < 37; ++i) {
        checkSmall(reordered->coordinates[37 * 0 + i][2], TOLERANCE);
        checkClose(reordered->coordinates[37 * 1 + i][2], 5.5555555, TOLERANCE);
        checkClose(reordered->coordinates[37 * 2 + i][2], 11.111111, TOLERANCE);
        checkClose(reordered->coordinates[37 * 3 + i][2], 16.666666, TOLERANCE);
        checkClose(reordered->coordinates[37 * 4 + i][2], 22.222222, TOLERANCE);
        checkClose(reordered->coordinates[37 * 5 + i][2], 27.777777, TOLERANCE);
        checkClose(reordered->coordinates[37 * 6 + i][2], 33.333333, TOLERANCE);
        checkClose(reordered->coordinates[37 * 7 + i][2], 38.888888, TOLERANCE);
        checkClose(reordered->coordinates[37 * 8 + i][2], 44.444444, TOLERANCE);
        checkClose(reordered->coordinates[37 * 9 + i][2], 49.999999, TOLERANCE);
    }

    for (int i = 0; i < 37; ++i) {
        checkClose(fabs(reordered->coordinates[37 * 0 + i][2] - reordered->coordinates[37 *  1 + i][2]), 5.5555555555555554e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 1 + i][2] - reordered->coordinates[37 *  2 + i][2]), 5.5555555555555554e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 2 + i][2] - reordered->coordinates[37 *  3 + i][2]), 5.5555555555555536e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 3 + i][2] - reordered->coordinates[37 *  4 + i][2]), 5.5555555555555571e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 4 + i][2] - reordered->coordinates[37 *  5 + i][2]), 5.5555555555555571e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 5 + i][2] - reordered->coordinates[37 *  6 + i][2]), 5.5555555555555500e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 6 + i][2] - reordered->coordinates[37 *  7 + i][2]), 5.5555555555555642e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 7 + i][2] - reordered->coordinates[37 *  8 + i][2]), 5.5555538601345447e+00, TOLERANCE);
        checkClose(fabs(reordered->coordinates[37 * 8 + i][2] - reordered->coordinates[37 *  9 + i][2]), 5.5555572509765625e+00, TOLERANCE);
    }

    for (int i = 0; i < 37; ++i) {
        checkClose(this->calculateDistance(reordered->coordinates[37 * 0 + i], reordered->coordinates[37 *  1 + i]), 5.5555555555555554e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 1 + i], reordered->coordinates[37 *  2 + i]), 5.5555555555555554e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 2 + i], reordered->coordinates[37 *  3 + i]), 5.5555555555555536e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 3 + i], reordered->coordinates[37 *  4 + i]), 5.5555555555555571e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 4 + i], reordered->coordinates[37 *  5 + i]), 5.5555555555555571e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 5 + i], reordered->coordinates[37 *  6 + i]), 5.5555555555555500e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 6 + i], reordered->coordinates[37 *  7 + i]), 5.5555555555555642e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 7 + i], reordered->coordinates[37 *  8 + i]), 5.5555538601345447e+00, TOLERANCE);
        checkClose(this->calculateDistance(reordered->coordinates[37 * 8 + i], reordered->coordinates[37 *  9 + i]), 5.5555572509765625e+00, TOLERANCE);
    }
}

TestSuiteEnd()
