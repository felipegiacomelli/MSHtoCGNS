#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/MshInterface/MshReaderFixture.hpp"

#define TOLERANCE 1e-12

struct Read_Region1_Prisms_3D_Fixture : public MshReaderFixture {
    Read_Region1_Prisms_3D_Fixture() : MshReaderFixture("MshInterface/3D-Region1-Prisms/27v_16e.msh") {}
};

FixtureTestSuite(Read_Region1_Prisms_3D_Suite, Read_Region1_Prisms_3D_Fixture)

TestCase(Elements) {
    auto prisms = this->gridData->prisms;

    checkEqual((prisms.size()), 16u);
    checkEqual(prisms[0][0], 0); checkEqual(prisms[0][1], 1); checkEqual(prisms[0][2], 4); checkEqual(prisms[0][3], 9); checkEqual(prisms[0][4], 10); checkEqual(prisms[0][5], 13); checkEqual(prisms[0][6], 0);
    checkEqual(prisms[15][0], 13); checkEqual(prisms[15][1], 17); checkEqual(prisms[15][2], 16); checkEqual(prisms[15][3], 22); checkEqual(prisms[15][4], 26); checkEqual(prisms[15][5], 25); checkEqual(prisms[15][6], 15);
}

TestSuiteEnd()
