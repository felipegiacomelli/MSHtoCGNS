#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <FileMend/RadialGridDataReordered.hpp>

#define TOLERANCE 1e-6

struct RadialGridDataReorderFixture {
	RadialGridDataReorderFixture() {
		CgnsReader3D reader(this->inputPath);
		this->gridData = reader.gridData;
	}

	std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/RadialGridDataReordered/370v_324e.cgns";
	GridDataShared gridData;
	std::string radialGridDataReorderedScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/RadialGridDataReordered/ScriptRadialGridDataReorder.json";
};

FixtureTestSuite(RadialGridDataReorderSuite, RadialGridDataReorderFixture)

TestCase(RadialGridDataReorderTest) {
	RadialGridDataReordered radialGridDataReordered(this->gridData, this->radialGridDataReorderedScript);
}

TestSuiteEnd()