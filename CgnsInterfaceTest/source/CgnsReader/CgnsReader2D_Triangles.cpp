#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsReader.hpp"

struct ReaderFixtureTriangles : public FixtureCgnsReader {
    ReaderFixtureTriangles() : FixtureCgnsReader("CgnsInterface/2D-Region4-ElementType1/11v_10e.cgns") {}
};

FixtureTestSuite(CgnsReader_Triangles, ReaderFixtureTriangles)

TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {7, 4,  5,  8, 7, 0},
        {5, 3,  4,  9, 1},
        {5, 3,  9,  6, 2},
        {5, 4,  7,  9, 3},
        {5, 6,  9,  7, 4},
        {7, 0,  1,  4, 3, 5},
        {5, 1,  2, 10, 6},
        {5, 1, 10,  4, 7},
        {5, 2,  5, 10, 8},
        {5, 4, 10,  5, 9},
        {3, 6,  3, 10},
        {3, 3,  0, 11},
        {3, 2,  5, 12},
        {3, 5,  8, 13},
        {3, 0,  1, 14},
        {3, 1,  2, 15},
        {3, 8,  7, 16},
        {3, 7,  6, 17}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqual(connectivities.size(), 18u);
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
    checkEqualCollections(connectivities[12].cbegin(), connectivities[12].cend(), expected[12].cbegin(), expected[12].cend());
    checkEqualCollections(connectivities[13].cbegin(), connectivities[13].cend(), expected[13].cbegin(), expected[13].cend());
    checkEqualCollections(connectivities[14].cbegin(), connectivities[14].cend(), expected[14].cbegin(), expected[14].cend());
    checkEqualCollections(connectivities[15].cbegin(), connectivities[15].cend(), expected[15].cbegin(), expected[15].cend());
    checkEqualCollections(connectivities[16].cbegin(), connectivities[16].cend(), expected[16].cbegin(), expected[16].cend());
    checkEqualCollections(connectivities[17].cbegin(), connectivities[17].cend(), expected[17].cbegin(), expected[17].cend());
}

TestSuiteEnd()
