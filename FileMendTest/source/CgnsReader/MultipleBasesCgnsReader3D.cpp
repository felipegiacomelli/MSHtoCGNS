#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/FileMend/CgnsReader/MultipleBasesCgnsReader3D.hpp"

#define TOLERANCE 1e-12

struct MultipleBasesCgnsReader3DFixture {
    MultipleBasesCgnsReader3DFixture() {
        // MultipleBasesCgnsReader3D specialCgnsReader3D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Mixed/12523v_57072e.cgns");
        MultipleBasesCgnsReader3D specialCgnsReader3D("/home/felipe/Downloads/2216v_7020e.cgns");
    }
};

FixtureTestSuite(MultipleBasesCgnsReader3DSuite, MultipleBasesCgnsReader3DFixture)

TestCase(MultipleBasesCgnsReader3DCase) {
    check(1 == 1);
}

TestSuiteEnd()
