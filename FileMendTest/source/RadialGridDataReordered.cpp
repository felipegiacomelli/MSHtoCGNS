#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
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

	checkEqual(final->coordinates.size(), 370u);

	checkEqual(final->tetrahedronConnectivity.size(),   0u);
	checkEqual(final->hexahedronConnectivity.size() , 216u);
	checkEqual(final->prismConnectivity.size()      , 108u);
	checkEqual(final->pyramidConnectivity.size()    ,   0u);

	checkEqual(final->triangleConnectivity.size()  , 0u);
	checkEqual(final->quadrangleConnectivity.size(), 0u);

	checkEqual(final->lineConnectivity.size(), 9u);

	checkEqual(final->boundaries.size(),  0u);
	checkEqual(final->regions.size()   ,  1u);
	checkEqual(final->wells.size()     ,  1u);

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

	std::cout << std::endl;

	for (int i = 0; i < 37; i++) {
		std::cout << "\t";
		for (auto v : final->coordinates[i])
			std::cout << "\t" << std::scientific << std::setprecision(6) << v;
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (int i = 37; i < 74; i++) {
		std::cout << "\t";
		for (auto v : final->coordinates[i])
			std::cout << "\t" << std::scientific << std::setprecision(6) << v;
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (int i = 74; i < 111; i++) {
		std::cout << "\t";
		for (auto v : final->coordinates[i])
			std::cout << "\t" << std::scientific << std::setprecision(6) << v;
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (int i = 111; i < 148; i++) {
		std::cout << "\t";
		for (auto v : final->coordinates[i])
			std::cout << "\t" << std::scientific << std::setprecision(6) << v;
		std::cout << std::endl;
	}

	CgnsCreator3D creator(final, "/home/felipe/Downloads/msh_to_cgns");

}

TestSuiteEnd()