#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureMshReader.hpp"

#define TOLERANCE 1e-12

struct Read_Region1_Prisms_3D_Fixture : public FixtureMshReader {
    Read_Region1_Prisms_3D_Fixture() : FixtureMshReader("MshInterface/3D-Region1-Prisms/27v_16e.msh") {}
};

FixtureTestSuite(Read_Region1_Prisms_3D_Suite, Read_Region1_Prisms_3D_Fixture)

TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {14,  0,  1,  4,  9, 10, 13,  0},
        {14, 13, 17, 16, 22, 26, 25, 15}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqual(connectivities.size(), 48u);
    checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[0].cbegin(), expected[0].cend());
    checkEqualCollections(connectivities[15].cbegin(), connectivities[15].cend(), expected[1].cbegin(), expected[1].cend());
}

TestSuiteEnd()
