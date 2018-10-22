#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <FileMend/RadialGridDataReordered.hpp>
#include <Utilities/Print.hpp>

#define TOLERANCE 1e-4

double calculateDistance(std::array<double, 3> a, std::array<double, 3> b) {
	return std::sqrt(std::inner_product(a.cbegin(), a.cend(), b.cbegin(), 0.0, std::plus<>(), [](auto c, auto d){return (c-d)*(c-d);}));
}

struct RadialGridDataReorderFixture {
	RadialGridDataReorderFixture() {
		CgnsReader3D reader(this->inputPath);
		this->gridData = reader.gridData;
	}

	std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/RadialGridDataReordered/370v_324e.cgns";
	boost::shared_ptr<GridData> gridData;
};

FixtureTestSuite(RadialGridDataReorderSuite, RadialGridDataReorderFixture)

TestCase(RadialGridDataReorderTest) {
	RadialGridDataReordered radialGridDataReordered(this->gridData);

	auto reordered = radialGridDataReordered.reordered;

	checkEqual(reordered->coordinates.size(), 370u);

	checkEqual(reordered->tetrahedronConnectivity.size(),   0u);
	checkEqual(reordered->hexahedronConnectivity.size() , 216u);
	checkEqual(reordered->prismConnectivity.size()      , 108u);
	checkEqual(reordered->pyramidConnectivity.size()    ,   0u);

	checkEqual(reordered->triangleConnectivity.size()  ,  24u);
	checkEqual(reordered->quadrangleConnectivity.size(), 156u);

	checkEqual(reordered->lineConnectivity.size(), 9u);

	checkEqual(reordered->boundaries.size(),  3u);
	checkEqual(reordered->regions.size()   ,  1u);
	checkEqual(reordered->wells.size()     ,  1u);

	checkEqual(reordered->boundaries[0].facetBegin, 324);
	checkEqual(reordered->boundaries[0].facetEnd  , 432);

	checkEqual(reordered->boundaries[1].facetBegin, 432);
	checkEqual(reordered->boundaries[1].facetEnd  , 468);

	checkEqual(reordered->boundaries[2].facetBegin, 468);
	checkEqual(reordered->boundaries[2].facetEnd  , 504);

	checkEqual(reordered->wells[0].lineBegin, 504);
	checkEqual(reordered->wells[0].lineEnd  , 513);

	for (int i = 0; i < 9; i++) {
		int shift = 36 * i;

		for (int j = 0; j < 12; j++)
			checkEqual(reordered->prismConnectivity[12*i + j].back(), shift++);

		for (int j = 0; j < 24; j++)
			checkEqual(reordered->hexahedronConnectivity[24*i + j].back(), shift++);
	}

	for (int i = 0; i < 108; i++) {
		checkEqual(reordered->quadrangleConnectivity[i].back(), 324 + i);
	}

	for (int i = 0; i < 12; i++) {
		checkEqual(reordered->triangleConnectivity[i     ].back(), 432 + i);
		checkEqual(reordered->triangleConnectivity[i + 12].back(), 468 + i);
	}

	for (int i = 0; i < 24; i++) {
		checkEqual(reordered->quadrangleConnectivity[i + 108].back(), 444 + i);
		checkEqual(reordered->quadrangleConnectivity[i + 132].back(), 480 + i);
	}

	for (int i = 0; i < 9; i++)
		checkEqual(reordered->lineConnectivity[i].back(), 504 + i);

	for (int i = 0; i < 37; i++) {
		checkSmall(reordered->coordinates[37 * 0 + i][2], TOLERANCE);
		checkClose(reordered->coordinates[37 * 1 + i][2], 5.5555555, TOLERANCE);
		checkClose(reordered->coordinates[37 * 2 + i][2], 11.111111, TOLERANCE);
		checkClose(reordered->coordinates[37 * 3 + i][2], 16.666666, TOLERANCE);
		checkClose(reordered->coordinates[37 * 4 + i][2], 22.222222, TOLERANCE);
		checkClose(reordered->coordinates[37 * 5 + i][2], 27.777777, TOLERANCE);
		checkClose(reordered->coordinates[37 * 6 + i][2], 33.333333, TOLERANCE);
		checkClose(reordered->coordinates[37 * 7 + i][2], 38.888888, TOLERANCE);
		checkClose(reordered->coordinates[37 * 8 + i][2], 44.444444, TOLERANCE);
		checkClose(reordered->coordinates[37 * 9 + i][2], 49.999999, TOLERANCE);
	}

	for (int i = 0; i < 37; i++) {
		checkClose(std::abs(reordered->coordinates[37 * 0 + i][2] - reordered->coordinates[37 *  1 + i][2]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(std::abs(reordered->coordinates[37 * 1 + i][2] - reordered->coordinates[37 *  2 + i][2]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(std::abs(reordered->coordinates[37 * 2 + i][2] - reordered->coordinates[37 *  3 + i][2]), 5.5555555555555536e+00, TOLERANCE);
		checkClose(std::abs(reordered->coordinates[37 * 3 + i][2] - reordered->coordinates[37 *  4 + i][2]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(std::abs(reordered->coordinates[37 * 4 + i][2] - reordered->coordinates[37 *  5 + i][2]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(std::abs(reordered->coordinates[37 * 5 + i][2] - reordered->coordinates[37 *  6 + i][2]), 5.5555555555555500e+00, TOLERANCE);
		checkClose(std::abs(reordered->coordinates[37 * 6 + i][2] - reordered->coordinates[37 *  7 + i][2]), 5.5555555555555642e+00, TOLERANCE);
		checkClose(std::abs(reordered->coordinates[37 * 7 + i][2] - reordered->coordinates[37 *  8 + i][2]), 5.5555538601345447e+00, TOLERANCE);
		checkClose(std::abs(reordered->coordinates[37 * 8 + i][2] - reordered->coordinates[37 *  9 + i][2]), 5.5555572509765625e+00, TOLERANCE);
	}

	for (int i = 0; i < 37; i++) {
		checkClose(calculateDistance(reordered->coordinates[37 * 0 + i], reordered->coordinates[37 *  1 + i]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 1 + i], reordered->coordinates[37 *  2 + i]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 2 + i], reordered->coordinates[37 *  3 + i]), 5.5555555555555536e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 3 + i], reordered->coordinates[37 *  4 + i]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 4 + i], reordered->coordinates[37 *  5 + i]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 5 + i], reordered->coordinates[37 *  6 + i]), 5.5555555555555500e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 6 + i], reordered->coordinates[37 *  7 + i]), 5.5555555555555642e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 7 + i], reordered->coordinates[37 *  8 + i]), 5.5555538601345447e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 8 + i], reordered->coordinates[37 *  9 + i]), 5.5555572509765625e+00, TOLERANCE);
	}
}

TestSuiteEnd()