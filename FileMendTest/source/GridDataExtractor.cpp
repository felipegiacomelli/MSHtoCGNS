#include <BoostInterface/Test.hpp>
#include <IO/MshReader3D.hpp>
#include <FileMend/GridDataExtractor.hpp>

struct GridDataExtractorFixture {
	GridDataExtractorFixture() {
		MshReader3D reader(this->inputPath);
		this->gridData = reader.gridData;
	}

	std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractor/4x4x4_2x2x2.msh";
	GridDataShared gridData;
	std::string wellGeneratorScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractor/ScriptGridDataExtractor.json";
};

FixtureTestSuite(GridDataExtractorSuite, GridDataExtractorFixture)

TestCase(GridDataExtractorTest) {
	GridDataExtractor gridDataExtractor(this->gridData, this->wellGeneratorScript);
}

TestSuiteEnd()