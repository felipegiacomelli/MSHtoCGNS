#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsReader.hpp"

struct ReaderFixtureQuadrangles : public FixtureCgnsReader {
    ReaderFixtureQuadrangles() : FixtureCgnsReader("CgnsInterface/2D-Region1-ElementType1/9v_4e.cgns") {}
};

FixtureTestSuite(CgnsReader_Quadrangles, ReaderFixtureQuadrangles)

TestCase(Coordinates) {
    std::vector<std::array<double, 3>> expected{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {1.0, 1.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.5, 0.0, 0.0},
        {1.0, 0.5, 0.0},
        {0.5, 1.0, 0.0},
        {0.0, 0.5, 0.0},
        {0.5, 0.5, 0.0}
    };

    auto coordinates = this->gridData->coordinates;
    checkEqual(coordinates.size(), 9u);
    checkCloseCollection(coordinates[0], expected[0], TOLERANCE);
    checkCloseCollection(coordinates[1], expected[1], TOLERANCE);
    checkCloseCollection(coordinates[2], expected[2], TOLERANCE);
    checkCloseCollection(coordinates[3], expected[3], TOLERANCE);
    checkCloseCollection(coordinates[4], expected[4], TOLERANCE);
    checkCloseCollection(coordinates[5], expected[5], TOLERANCE);
    checkCloseCollection(coordinates[6], expected[6], TOLERANCE);
    checkCloseCollection(coordinates[7], expected[7], TOLERANCE);
    checkCloseCollection(coordinates[8], expected[8], TOLERANCE);
}

TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {7, 0, 4,  8, 7, 0},
        {7, 7, 8,  6, 3, 1},
        {7, 4, 1,  5, 8, 2},
        {7, 8, 5,  2, 6, 3},
        {3, 3, 7,  4},
        {3, 7, 0,  5},
        {3, 1, 5,  6},
        {3, 5, 2,  7},
        {3, 0, 4,  8},
        {3, 4, 1,  9},
        {3, 2, 6, 10},
        {3, 6, 3, 11}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqual(connectivities.size(), 12u);
    checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[ 0].cbegin(), expected[ 0].cend());
    checkEqualCollections(connectivities[ 1].cbegin(), connectivities[ 1].cend(), expected[ 1].cbegin(), expected[ 1].cend());
    checkEqualCollections(connectivities[ 2].cbegin(), connectivities[ 2].cend(), expected[ 2].cbegin(), expected[ 2].cend());
    checkEqualCollections(connectivities[ 3].cbegin(), connectivities[ 3].cend(), expected[ 3].cbegin(), expected[ 3].cend());
    checkEqualCollections(connectivities[ 4].cbegin(), connectivities[ 4].cend(), expected[ 4].cbegin(), expected[ 4].cend());
    checkEqualCollections(connectivities[ 5].cbegin(), connectivities[ 5].cend(), expected[ 5].cbegin(), expected[ 5].cend());
    checkEqualCollections(connectivities[ 6].cbegin(), connectivities[ 6].cend(), expected[ 6].cbegin(), expected[ 6].cend());
    checkEqualCollections(connectivities[ 7].cbegin(), connectivities[ 7].cend(), expected[ 7].cbegin(), expected[ 7].cend());
    checkEqualCollections(connectivities[ 8].cbegin(), connectivities[ 8].cend(), expected[ 8].cbegin(), expected[ 8].cend());
    checkEqualCollections(connectivities[ 9].cbegin(), connectivities[ 9].cend(), expected[ 9].cbegin(), expected[ 9].cend());
    checkEqualCollections(connectivities[10].cbegin(), connectivities[10].cend(), expected[10].cbegin(), expected[10].cend());
    checkEqualCollections(connectivities[11].cbegin(), connectivities[11].cend(), expected[11].cbegin(), expected[11].cend());
}

TestSuiteEnd()
