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
};

FixtureTestSuite(RadialGridDataReorderSuite, RadialGridDataReorderFixture)

TestCase(RadialGridDataReorderTest) {
	RadialGridDataReordered radialGridDataReordered(this->gridData);

	auto final = radialGridDataReordered.final;

	for (int i = 0; i < 24; i++) {
		std::cout << "\t";
		for (auto v : final->hexahedronConnectivity[i])
			std::cout << "\t" << v;
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (int i = 0; i < 12; i++) {
		std::cout << "\t";
		for (auto v : final->prismConnectivity[i])
			std::cout << "\t" << v;
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (int i = 0; i < 9; i++) {
		std::cout << "\t";
		for (auto v : final->lineConnectivity[i])
			std::cout << "\t" << v;
		std::cout << std::endl;
	}

}

TestSuiteEnd()