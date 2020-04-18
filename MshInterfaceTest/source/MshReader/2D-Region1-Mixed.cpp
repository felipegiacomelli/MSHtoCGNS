#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureMshReader.hpp"

#define TOLERANCE 1e-12

struct Read_Region1_Mixed_2D_Fixture : public FixtureMshReader {
    Read_Region1_Mixed_2D_Fixture() : FixtureMshReader("MshInterface/2D-Region1-ElementType2/11v_10e.msh") {}
};

FixtureTestSuite(Read_Region1_Mixed_2D_Suite, Read_Region1_Mixed_2D_Fixture)

TestCase(Dimension) {
    checkEqual(this->gridData->dimension, 2);
}

TestCase(LocalVertices) {
    checkEqual(this->gridData->numberOfLocalVertices, 11);
}

TestCase(Coordinates) {
    std::vector<std::array<double, 3>> expected{
        {0.00, 0.00, 0.00},
        {0.50, 0.00, 0.00},
        {1.00, 0.00, 0.00},
        {0.00, 0.50, 0.00},
        {0.50, 0.50, 0.00},
        {1.00, 0.50, 0.00},
        {0.00, 1.00, 0.00},
        {0.50, 1.00, 0.00},
        {1.00, 1.00, 0.00},
        {0.25, 0.75, 0.00},
        {0.75, 0.25, 0.00},
    };

    auto coordinates = this->gridData->coordinates;
    checkEqual(coordinates.size(), 11u);
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
}

TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {5, 3,  4,  9, 0},
        {5, 4, 10,  5, 7},
        {7, 4,  5,  8, 7, 8},
        {7, 0,  1,  4, 3, 9},
        {3, 6,  3, 10},
        {3, 7,  6, 17}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqual(connectivities.size(), 18u);
    checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[0].cbegin(), expected[0].cend());
    checkEqualCollections(connectivities[ 7].cbegin(), connectivities[ 7].cend(), expected[1].cbegin(), expected[1].cend());
    checkEqualCollections(connectivities[ 8].cbegin(), connectivities[ 8].cend(), expected[2].cbegin(), expected[2].cend());
    checkEqualCollections(connectivities[ 9].cbegin(), connectivities[ 9].cend(), expected[3].cbegin(), expected[3].cend());
    checkEqualCollections(connectivities[10].cbegin(), connectivities[10].cend(), expected[4].cbegin(), expected[4].cend());
    checkEqualCollections(connectivities[17].cbegin(), connectivities[17].cend(), expected[5].cbegin(), expected[5].cend());
}

TestCase(Entities) {
    checkEqual(this->gridData->sections.size(), 5u);

    {
        auto section = this->gridData->sections[0];
        checkEqual(section.name, std::string("GEOMETRY"));
        checkEqual(section.dimension, 2);
        checkEqual(section.begin, 0);
        checkEqual(section.end, 10);

        std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto vertices = section.vertices;
        checkEqualCollections(vertices.cbegin(), vertices.cend(), expected.cbegin(), expected.cend());
    }

    {
        auto section = this->gridData->sections[1];
        checkEqual(section.name, std::string("WEST"));
        checkEqual(section.dimension, 1);
        checkEqual(section.begin, 10);
        checkEqual(section.end, 12);

        std::vector<int> expected{0, 3, 6};
        auto vertices = section.vertices;
        checkEqualCollections(vertices.cbegin(), vertices.cend(), expected.cbegin(), expected.cend());
    }

    {
        auto section = this->gridData->sections[4];
        checkEqual(section.name, std::string("NORTH"));
        checkEqual(section.dimension, 1);
        checkEqual(section.begin, 16);
        checkEqual(section.end, 18);

        std::vector<int> expected{6, 7, 8};
        auto vertices = section.vertices;
        checkEqualCollections(vertices.cbegin(), vertices.cend(), expected.cbegin(), expected.cend());
    }
}

TestSuiteEnd()
