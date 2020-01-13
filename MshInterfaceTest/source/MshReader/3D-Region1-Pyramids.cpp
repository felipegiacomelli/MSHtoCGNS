#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureMshReader.hpp"

#define TOLERANCE 1e-12

struct Read_Region1_Pyramids_3D_Fixture : public FixtureMshReader {
    Read_Region1_Pyramids_3D_Fixture() : FixtureMshReader("MshInterface/3D-Region1-Pyramids/35v_48e.msh") {}
};

FixtureTestSuite(Read_Region1_Pyramids_3D_Suite, Read_Region1_Pyramids_3D_Fixture)

TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {12, 0, 1, 4, 3, 27, 0},
        {12, 17, 26, 25, 16, 34, 47}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqual(connectivities.size(), 72u);
    checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[0].cbegin(), expected[0].cend());
    checkEqualCollections(connectivities[47].cbegin(), connectivities[47].cend(), expected[1].cbegin(), expected[1].cend());
}

TestSuiteEnd()
