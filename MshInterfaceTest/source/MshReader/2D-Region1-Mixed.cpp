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
    checkEqualCollections(coordinates[ 0].cbegin(), coordinates[ 0].cend(), expected[ 0].cbegin(), expected[ 0].cend());
    checkEqualCollections(coordinates[ 1].cbegin(), coordinates[ 1].cend(), expected[ 1].cbegin(), expected[ 1].cend());
    checkEqualCollections(coordinates[ 2].cbegin(), coordinates[ 2].cend(), expected[ 2].cbegin(), expected[ 2].cend());
    checkEqualCollections(coordinates[ 3].cbegin(), coordinates[ 3].cend(), expected[ 3].cbegin(), expected[ 3].cend());
    checkEqualCollections(coordinates[ 4].cbegin(), coordinates[ 4].cend(), expected[ 4].cbegin(), expected[ 4].cend());
    checkEqualCollections(coordinates[ 5].cbegin(), coordinates[ 5].cend(), expected[ 5].cbegin(), expected[ 5].cend());
    checkEqualCollections(coordinates[ 6].cbegin(), coordinates[ 6].cend(), expected[ 6].cbegin(), expected[ 6].cend());
    checkEqualCollections(coordinates[ 7].cbegin(), coordinates[ 7].cend(), expected[ 7].cbegin(), expected[ 7].cend());
    checkEqualCollections(coordinates[ 8].cbegin(), coordinates[ 8].cend(), expected[ 8].cbegin(), expected[ 8].cend());
    checkEqualCollections(coordinates[ 9].cbegin(), coordinates[ 9].cend(), expected[ 9].cbegin(), expected[ 9].cend());
    checkEqualCollections(coordinates[10].cbegin(), coordinates[10].cend(), expected[10].cbegin(), expected[10].cend());
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
