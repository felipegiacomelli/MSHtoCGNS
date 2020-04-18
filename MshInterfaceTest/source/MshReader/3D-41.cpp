#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureMshReader.hpp"

#define TOLERANCE 1e-12

struct Read_41_3D_Fixture : public FixtureMshReader {
    Read_41_3D_Fixture() : FixtureMshReader("MshInterface/3D-41/32v_9e.msh") {}
};

FixtureTestSuite(Read_41_3D_Suite, Read_41_3D_Fixture)

TestCase(Coordinates) {
    std::vector<std::array<double, 3>> expected{
        {0.0000000000000000, 0.0000000000000000, 0.0},
        {1.0000000000000000, 0.0000000000000000, 0.0},
        {1.0000000000000000, 1.0000000000000000, 0.0},
        {0.0000000000000000, 1.0000000000000000, 0.0},
        {0.0000000000000000, 0.0000000000000000, 1.0},
        {1.0000000000000000, 0.0000000000000000, 1.0},
        {1.0000000000000000, 1.0000000000000000, 1.0},
        {0.0000000000000000, 1.0000000000000000, 1.0},
        {0.3333333333333333, 0.0000000000000000, 0.0},
        {0.6666666666666666, 0.0000000000000000, 0.0},
        {1.0000000000000000, 0.3333333333333333, 0.0},
        {1.0000000000000000, 0.6666666666666666, 0.0},
        {0.6666666666666667, 1.0000000000000000, 0.0},
        {0.3333333333333334, 1.0000000000000000, 0.0},
        {0.0000000000000000, 0.6666666666666667, 0.0},
        {0.0000000000000000, 0.3333333333333334, 0.0},
        {0.3333333333333333, 0.0000000000000000, 1.0},
        {0.6666666666666666, 0.0000000000000000, 1.0},
        {1.0000000000000000, 0.3333333333333333, 1.0},
        {1.0000000000000000, 0.6666666666666666, 1.0},
        {0.6666666666666667, 1.0000000000000000, 1.0},
        {0.3333333333333334, 1.0000000000000000, 1.0},
        {0.0000000000000000, 0.6666666666666667, 1.0},
        {0.0000000000000000, 0.3333333333333334, 1.0},
        {0.3333333333333334, 0.3333333333333333, 0.0},
        {0.3333333333333334, 0.6666666666666666, 0.0},
        {0.6666666666666666, 0.3333333333333334, 0.0},
        {0.6666666666666666, 0.6666666666666666, 0.0},
        {0.3333333333333334, 0.3333333333333333, 1.0},
        {0.3333333333333334, 0.6666666666666666, 1.0},
        {0.6666666666666666, 0.3333333333333334, 1.0},
        {0.6666666666666666, 0.6666666666666666, 1.0}
    };

    auto coordinates = this->gridData->coordinates;
    checkEqual(coordinates.size(), 32u);
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
    checkCloseCollection(coordinates[27], expected[27], TOLERANCE);
    checkCloseCollection(coordinates[28], expected[28], TOLERANCE);
    checkCloseCollection(coordinates[29], expected[29], TOLERANCE);
    checkCloseCollection(coordinates[30], expected[30], TOLERANCE);
    checkCloseCollection(coordinates[31], expected[31], TOLERANCE);
}

TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {17,  0,  8, 24, 15,  4, 16, 28, 23, 0},
        {17, 26, 10, 11, 27, 30, 18, 19, 31, 7},
        { 7,  3, 14, 22,  7,  9},
        { 7, 31, 19,  6, 20,  38},
        { 3,  1,  5, 39},
        { 3,  3,  7, 40}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqual(connectivities.size(), 41u);
    checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[0].cbegin(), expected[0].cend());
    checkEqualCollections(connectivities[ 7].cbegin(), connectivities[ 7].cend(), expected[1].cbegin(), expected[1].cend());
    checkEqualCollections(connectivities[ 9].cbegin(), connectivities[ 9].cend(), expected[2].cbegin(), expected[2].cend());
    checkEqualCollections(connectivities[38].cbegin(), connectivities[38].cend(), expected[3].cbegin(), expected[3].cend());
    checkEqualCollections(connectivities[39].cbegin(), connectivities[39].cend(), expected[4].cbegin(), expected[4].cend());
    checkEqualCollections(connectivities[40].cbegin(), connectivities[40].cend(), expected[5].cbegin(), expected[5].cend());
}

TestCase(Entities) {
    auto sections = this->gridData->sections;
    checkEqual(sections.size(), 9u);

    {
        auto section = this->gridData->sections[0];
        checkEqual(section.name, std::string("BODY"));
        checkEqual(section.dimension, 3);
        checkEqual(section.begin, 0);
        checkEqual(section.end, 9);
    }

    {
        auto section = this->gridData->sections[3];
        checkEqual(section.name, std::string("SOUTH"));
        checkEqual(section.dimension, 2);
        checkEqual(section.begin, 15);
        checkEqual(section.end, 18);
    }

    {
        auto section = this->gridData->sections[8];
        checkEqual(section.name, std::string("PRODUCER"));
        checkEqual(section.dimension, 1);
        checkEqual(section.begin, 40);
        checkEqual(section.end, 41);
    }
}

TestSuiteEnd()
