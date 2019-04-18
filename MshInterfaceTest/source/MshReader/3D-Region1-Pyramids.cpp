#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/MshInterface/MshReaderFixture.hpp"

#define TOLERANCE 1e-12

struct Read_Region1_Pyramids_3D_Fixture : public MshReaderFixture {
    Read_Region1_Pyramids_3D_Fixture() : MshReaderFixture("MshInterface/3D-Region1-Pyramids/35v_48e.msh") {}
};

FixtureTestSuite(Read_Region1_Pyramids_3D_Suite, Read_Region1_Pyramids_3D_Fixture)

TestCase(Elements) {
    auto pyramids = this->gridData->pyramids;

    checkEqual((pyramids.size()), 48u);
    checkEqual(pyramids[0][0], 0); checkEqual(pyramids[0][1], 1); checkEqual(pyramids[0][2], 4); checkEqual(pyramids[0][3], 3); checkEqual(pyramids[0][4], 27); checkEqual(pyramids[0][5], 0);
    checkEqual(pyramids[47][0], 17); checkEqual(pyramids[47][1], 26); checkEqual(pyramids[47][2], 25); checkEqual(pyramids[47][3], 16); checkEqual(pyramids[47][4], 34); checkEqual(pyramids[47][5], 47);
}

TestSuiteEnd()
