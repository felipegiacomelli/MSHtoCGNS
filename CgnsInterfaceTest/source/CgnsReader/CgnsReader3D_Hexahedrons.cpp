#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsReader.hpp"

struct ReaderFixtureHexahedrons : public FixtureCgnsReader {
    ReaderFixtureHexahedrons() : FixtureCgnsReader("CgnsInterface/3D-Region1-Hexahedra/27v_8e.cgns") {}
};

FixtureTestSuite(CgnsReader_Hexahedrons, ReaderFixtureHexahedrons)

TestCase(Coordinates) {
    std::vector<std::array<double, 3>> expected{
        {0.0, 0.0, 0.0},
        {0.5, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 0.5, 0.0},
        {0.5, 0.5, 0.0},
        {1.0, 0.5, 0.0},
        {0.0, 1.0, 0.0},
        {0.5, 1.0, 0.0},
        {1.0, 1.0, 0.0},
        {0.0, 0.0, 0.5},
        {0.5, 0.0, 0.5},
        {1.0, 0.0, 0.5},
        {0.0, 0.5, 0.5},
        {0.5, 0.5, 0.5},
        {1.0, 0.5, 0.5},
        {0.0, 1.0, 0.5},
        {0.5, 1.0, 0.5},
        {1.0, 1.0, 0.5},
        {0.0, 0.0, 1.0},
        {0.5, 0.0, 1.0},
        {1.0, 0.0, 1.0},
        {0.0, 0.5, 1.0},
        {0.5, 0.5, 1.0},
        {1.0, 0.5, 1.0},
        {0.0, 1.0, 1.0},
        {0.5, 1.0, 1.0},
        {1.0, 1.0, 1.0}
    };

    auto coordinates = this->gridData->coordinates;
    checkEqual(coordinates.size(), 27u);
    checkCloseCollection(coordinates[ 0], expected[ 0], TOLERANCE);
    checkCloseCollection(coordinates[ 1], expected[ 1], TOLERANCE);
    checkCloseCollection(coordinates[ 2], expected[ 2], TOLERANCE);
    checkCloseCollection(coordinates[ 3], expected[ 3], TOLERANCE);
    checkCloseCollection(coordinates[ 4], expected[ 4], TOLERANCE);
    checkCloseCollection(coordinates[ 5], expected[ 5], TOLERANCE);
    checkCloseCollection(coordinates[ 6], expected[ 6], TOLERANCE);
    checkCloseCollection(coordinates[ 7], expected[ 7], TOLERANCE);
    checkCloseCollection(coordinates[ 8], expected[ 8], TOLERANCE);
    checkCloseCollection(coordinates[ 9], expected[ 9], TOLERANCE);
    checkCloseCollection(coordinates[10], expected[10], TOLERANCE);
    checkCloseCollection(coordinates[11], expected[11], TOLERANCE);
    checkCloseCollection(coordinates[12], expected[12], TOLERANCE);
    checkCloseCollection(coordinates[13], expected[13], TOLERANCE);
    checkCloseCollection(coordinates[14], expected[14], TOLERANCE);
    checkCloseCollection(coordinates[15], expected[15], TOLERANCE);
    checkCloseCollection(coordinates[16], expected[16], TOLERANCE);
    checkCloseCollection(coordinates[17], expected[17], TOLERANCE);
    checkCloseCollection(coordinates[18], expected[18], TOLERANCE);
    checkCloseCollection(coordinates[19], expected[19], TOLERANCE);
    checkCloseCollection(coordinates[20], expected[20], TOLERANCE);
    checkCloseCollection(coordinates[21], expected[21], TOLERANCE);
    checkCloseCollection(coordinates[22], expected[22], TOLERANCE);
    checkCloseCollection(coordinates[23], expected[23], TOLERANCE);
    checkCloseCollection(coordinates[24], expected[24], TOLERANCE);
    checkCloseCollection(coordinates[25], expected[25], TOLERANCE);
    checkCloseCollection(coordinates[26], expected[26], TOLERANCE);
}


TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {17,  0,  1,  4,  3,  9, 10, 13, 12, 0},
        {17,  1,  2,  5,  4, 10, 11, 14, 13, 1},
        {17,  3,  4,  7,  6, 12, 13, 16, 15, 2},
        {17,  4,  5,  8,  7, 13, 14, 17, 16, 3},
        {17,  9, 10, 13, 12, 18, 19, 22, 21, 4},
        {17, 10, 11, 14, 13, 19, 20, 23, 22, 5},
        {17, 12, 13, 16, 15, 21, 22, 25, 24, 6},
        {17, 13, 14, 17, 16, 22, 23, 26, 25, 7},
        { 7,  0,  9, 12,  3,  8},
        { 7,  3, 12, 15,  6,  9},
        { 7,  9, 18, 21, 12, 10},
        { 7, 12, 21, 24, 15, 11},
        { 7,  2,  5, 14, 11, 12},
        { 7,  5,  8, 17, 14, 13},
        { 7, 11, 14, 23, 20, 14},
        { 7, 14, 17, 26, 23, 15},
        { 7,  0,  1, 10,  9, 16},
        { 7,  1,  2, 11, 10, 17},
        { 7,  9, 10, 19, 18, 18},
        { 7, 10, 11, 20, 19, 19},
        { 7,  7,  6, 15, 16, 20},
        { 7,  8,  7, 16, 17, 21},
        { 7, 16, 15, 24, 25, 22},
        { 7, 17, 16, 25, 26, 23},
        { 7,  1,  0,  3,  4, 24},
        { 7,  2,  1,  4,  5, 25},
        { 7,  4,  3,  6,  7, 26},
        { 7,  5,  4,  7,  8, 27},
        { 7, 18, 19, 22, 21, 28},
        { 7, 19, 20, 23, 22, 29},
        { 7, 21, 22, 25, 24, 30},
        { 7, 22, 23, 26, 25, 31}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqual(connectivities.size(), 32u);
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
    checkEqualCollections(connectivities[18].cbegin(), connectivities[18].cend(), expected[18].cbegin(), expected[18].cend());
    checkEqualCollections(connectivities[19].cbegin(), connectivities[19].cend(), expected[19].cbegin(), expected[19].cend());
    checkEqualCollections(connectivities[20].cbegin(), connectivities[20].cend(), expected[20].cbegin(), expected[20].cend());
    checkEqualCollections(connectivities[21].cbegin(), connectivities[21].cend(), expected[21].cbegin(), expected[21].cend());
    checkEqualCollections(connectivities[22].cbegin(), connectivities[22].cend(), expected[22].cbegin(), expected[22].cend());
    checkEqualCollections(connectivities[23].cbegin(), connectivities[23].cend(), expected[23].cbegin(), expected[23].cend());
    checkEqualCollections(connectivities[24].cbegin(), connectivities[24].cend(), expected[24].cbegin(), expected[24].cend());
    checkEqualCollections(connectivities[25].cbegin(), connectivities[25].cend(), expected[25].cbegin(), expected[25].cend());
    checkEqualCollections(connectivities[26].cbegin(), connectivities[26].cend(), expected[26].cbegin(), expected[26].cend());
    checkEqualCollections(connectivities[27].cbegin(), connectivities[27].cend(), expected[27].cbegin(), expected[27].cend());
    checkEqualCollections(connectivities[28].cbegin(), connectivities[28].cend(), expected[28].cbegin(), expected[28].cend());
    checkEqualCollections(connectivities[29].cbegin(), connectivities[29].cend(), expected[29].cbegin(), expected[29].cend());
    checkEqualCollections(connectivities[30].cbegin(), connectivities[30].cend(), expected[30].cbegin(), expected[30].cend());
    checkEqualCollections(connectivities[31].cbegin(), connectivities[31].cend(), expected[31].cbegin(), expected[31].cend());
}

TestCase(Entities) {
    checkEqual(this->gridData->sections.size(), 7u);

    {
        auto section = this->gridData->sections[0];
        checkEqual(section.name, std::string("GEOMETRY"));
        checkEqual(section.dimension, 3);
        checkEqual(section.begin, 0);
        checkEqual(section.end, 8);

        std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
        checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
    }

    {
        auto section = this->gridData->sections[2];
        checkEqual(section.name, std::string("EAST"));
        checkEqual(section.dimension, 2);
        checkEqual(section.begin, 12);
        checkEqual(section.end, 16);

        std::vector<int> expected{2, 5, 8, 11, 14, 17, 20, 23, 26};
        checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
    }

    {
        auto section = this->gridData->sections[3];
        checkEqual(section.name, std::string("SOUTH"));
        checkEqual(section.dimension, 2);
        checkEqual(section.begin, 16);
        checkEqual(section.end, 20);

        std::vector<int> expected{0, 1, 2, 9, 10, 11, 18, 19, 20};
        checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
    }
}

TestSuiteEnd()
