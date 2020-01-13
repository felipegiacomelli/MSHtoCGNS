#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureMshReader.hpp"

#define TOLERANCE 1e-12

struct Read_Region1_Tetrahedrons_3D_Fixture : public FixtureMshReader {
    Read_Region1_Tetrahedrons_3D_Fixture() : FixtureMshReader("MshInterface/3D-Region1-Tetrahedrons/14v_24e.msh") {}
};

FixtureTestSuite(Read_Region1_Tetrahedrons_3D_Suite, Read_Region1_Tetrahedrons_3D_Fixture)

TestCase(Dimension) {
    checkEqual(this->gridData->dimension, 3);
}

TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {10, 13,  9, 10, 12,  0},
        {10,  0,  8, 10, 12, 23},
        {5,  0,  8,  3, 24},
        {5,  7, 13,  6, 47}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqual(connectivities.size(), 48u);
    checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[0].cbegin(), expected[0].cend());
    checkEqualCollections(connectivities[23].cbegin(), connectivities[23].cend(), expected[1].cbegin(), expected[1].cend());
    checkEqualCollections(connectivities[24].cbegin(), connectivities[24].cend(), expected[2].cbegin(), expected[2].cend());
    checkEqualCollections(connectivities[47].cbegin(), connectivities[47].cend(), expected[3].cbegin(), expected[3].cend());
}

TestSuiteEnd()
