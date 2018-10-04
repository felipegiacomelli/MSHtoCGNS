#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <FileMend/RadialGridDataReordered.hpp>

#define TOLERANCE 1e-2

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

	for (int i = 0; i < 9; i++) {
		int shift = 36 * i;

		for (int j = 0; j < 24; j++)
			checkEqual(final->hexahedronConnectivity[24*i + j].back(), shift++);

		for (int j = 0; j < 12; j++)
			checkEqual(final->prismConnectivity[12*i + j].back(), shift++);
	}

	for (int i = 0; i < 9; i++)
		checkEqual(final->lineConnectivity[i].back(), 324 + i);

	for (int i = 0; i < 37; i++)
		checkSmall(final->coordinates[i][2], TOLERANCE);

	for (int i = 37; i < 74; i++)
		checkClose(final->coordinates[i][2], 5.5555555, TOLERANCE);

	for (int i = 74; i < 111; i++)
		checkClose(final->coordinates[i][2], 11.111111, TOLERANCE);

	for (int i = 111; i < 148; i++)
		checkClose(final->coordinates[i][2], 16.666666, TOLERANCE);

	for (int i = 148; i < 185; i++)
		checkClose(final->coordinates[i][2], 22.222222, TOLERANCE);

	for (int i = 185; i < 222; i++)
		checkClose(final->coordinates[i][2], 27.777777, TOLERANCE);

	for (int i = 222; i < 259; i++)
		checkClose(final->coordinates[i][2], 33.333333, TOLERANCE);

	for (int i = 259; i < 296; i++)
		checkClose(final->coordinates[i][2], 38.888888, TOLERANCE);

	for (int i = 296; i < 333; i++)
		checkClose(final->coordinates[i][2], 44.444444, TOLERANCE);

	for (int i = 333; i < 370; i++)
		checkClose(final->coordinates[i][2], 49.999999, TOLERANCE);

	std::cout << std::endl;
	for (int i = 0; i < 9; i++)
		std::cout << std::scientific << std::setprecision(16) << std::abs(final->coordinates[final->lineConnectivity[i][0]][2] - final->coordinates[final->lineConnectivity[i][1]][2]) << std::endl;

	for (int i = 0; i < 10; i++) {
		if (i == 0 || i == 8 || i == 9) {
			checkClose(final->coordinates[37*i +  0][0], -81.29376453, TOLERANCE);	checkClose(final->coordinates[37*i +  0][1], 55.17165378, TOLERANCE);
			checkClose(final->coordinates[37*i +  1][0], -83.53553572, TOLERANCE);	checkClose(final->coordinates[37*i +  1][1], 56.46446799, TOLERANCE);
			checkClose(final->coordinates[37*i +  2][0], -80.86250969, TOLERANCE);	checkClose(final->coordinates[37*i +  2][1], 56.78110252, TOLERANCE);
			checkClose(final->coordinates[37*i +  3][0], -82.35702381, TOLERANCE);	checkClose(final->coordinates[37*i +  3][1], 57.64297866, TOLERANCE);
			checkClose(final->coordinates[37*i +  4][0], -80.43125484, TOLERANCE);	checkClose(final->coordinates[37*i +  4][1], 58.39055126, TOLERANCE);
			checkClose(final->coordinates[37*i +  5][0], -81.17851191, TOLERANCE);	checkClose(final->coordinates[37*i +  5][1], 58.82148933, TOLERANCE);
			checkClose(final->coordinates[37*i +  6][0], -78.70582460, TOLERANCE);	checkClose(final->coordinates[37*i +  6][1], 55.17176073, TOLERANCE);
			checkClose(final->coordinates[37*i +  7][0], -79.13721640, TOLERANCE);	checkClose(final->coordinates[37*i +  7][1], 56.78117382, TOLERANCE);
			checkClose(final->coordinates[37*i +  8][0], -79.56860820, TOLERANCE);	checkClose(final->coordinates[37*i +  8][1], 58.39058691, TOLERANCE);
			checkClose(final->coordinates[37*i +  9][0], -76.46446430, TOLERANCE);	checkClose(final->coordinates[37*i +  9][1], 56.46446801, TOLERANCE);
			checkClose(final->coordinates[37*i + 10][0], -77.64297620, TOLERANCE);	checkClose(final->coordinates[37*i + 10][1], 57.64297868, TOLERANCE);
			checkClose(final->coordinates[37*i + 11][0], -78.82148810, TOLERANCE);	checkClose(final->coordinates[37*i + 11][1], 58.82148934, TOLERANCE);
			checkClose(final->coordinates[37*i + 12][0], -75.17165335, TOLERANCE);	checkClose(final->coordinates[37*i + 12][1], 58.70623717, TOLERANCE);
			checkClose(final->coordinates[37*i + 13][0], -76.78110223, TOLERANCE);	checkClose(final->coordinates[37*i + 13][1], 59.13749145, TOLERANCE);
			checkClose(final->coordinates[37*i + 14][0], -78.39055112, TOLERANCE);	checkClose(final->coordinates[37*i + 14][1], 59.56874572, TOLERANCE);
			checkClose(final->coordinates[37*i + 15][0], -75.17176029, TOLERANCE);	checkClose(final->coordinates[37*i + 15][1], 61.29417370, TOLERANCE);
			checkClose(final->coordinates[37*i + 16][0], -76.78117352, TOLERANCE);	checkClose(final->coordinates[37*i + 16][1], 60.86278246, TOLERANCE);
			checkClose(final->coordinates[37*i + 17][0], -78.39058676, TOLERANCE);	checkClose(final->coordinates[37*i + 17][1], 60.43139123, TOLERANCE);
			checkClose(final->coordinates[37*i + 18][0], -76.46446429, TOLERANCE);	checkClose(final->coordinates[37*i + 18][1], 63.53553200, TOLERANCE);
			checkClose(final->coordinates[37*i + 19][0], -77.64297619, TOLERANCE);	checkClose(final->coordinates[37*i + 19][1], 62.35702133, TOLERANCE);
			checkClose(final->coordinates[37*i + 20][0], -78.82148810, TOLERANCE);	checkClose(final->coordinates[37*i + 20][1], 61.17851067, TOLERANCE);
			checkClose(final->coordinates[37*i + 21][0], -84.82834666, TOLERANCE);	checkClose(final->coordinates[37*i + 21][1], 58.70623716, TOLERANCE);
			checkClose(final->coordinates[37*i + 22][0], -83.21889778, TOLERANCE);	checkClose(final->coordinates[37*i + 22][1], 59.13749144, TOLERANCE);
			checkClose(final->coordinates[37*i + 23][0], -81.60944889, TOLERANCE);	checkClose(final->coordinates[37*i + 23][1], 59.56874572, TOLERANCE);
			checkClose(final->coordinates[37*i + 24][0], -84.82823972, TOLERANCE);	checkClose(final->coordinates[37*i + 24][1], 61.29417370, TOLERANCE);
			checkClose(final->coordinates[37*i + 25][0], -83.21882648, TOLERANCE);	checkClose(final->coordinates[37*i + 25][1], 60.86278247, TOLERANCE);
			checkClose(final->coordinates[37*i + 26][0], -81.60941324, TOLERANCE);	checkClose(final->coordinates[37*i + 26][1], 60.43139123, TOLERANCE);
			checkClose(final->coordinates[37*i + 27][0], -83.53553572, TOLERANCE);	checkClose(final->coordinates[37*i + 27][1], 63.53553200, TOLERANCE);
			checkClose(final->coordinates[37*i + 28][0], -82.35702381, TOLERANCE);	checkClose(final->coordinates[37*i + 28][1], 62.35702133, TOLERANCE);
			checkClose(final->coordinates[37*i + 29][0], -81.17851191, TOLERANCE);	checkClose(final->coordinates[37*i + 29][1], 61.17851067, TOLERANCE);
			checkClose(final->coordinates[37*i + 30][0], -81.29376453, TOLERANCE);	checkClose(final->coordinates[37*i + 30][1], 64.82834622, TOLERANCE);
			checkClose(final->coordinates[37*i + 31][0], -80.86250968, TOLERANCE);	checkClose(final->coordinates[37*i + 31][1], 63.21889748, TOLERANCE);
			checkClose(final->coordinates[37*i + 32][0], -80.43125484, TOLERANCE);	checkClose(final->coordinates[37*i + 32][1], 61.60944874, TOLERANCE);
			checkClose(final->coordinates[37*i + 33][0], -78.70582460, TOLERANCE);	checkClose(final->coordinates[37*i + 33][1], 64.82823927, TOLERANCE);
			checkClose(final->coordinates[37*i + 34][0], -79.13721640, TOLERANCE);	checkClose(final->coordinates[37*i + 34][1], 63.21882618, TOLERANCE);
			checkClose(final->coordinates[37*i + 35][0], -79.56860820, TOLERANCE);	checkClose(final->coordinates[37*i + 35][1], 61.60941309, TOLERANCE);
			checkClose(final->coordinates[37*i + 36][0], -80.00000000, TOLERANCE);	checkClose(final->coordinates[37*i + 36][1], 60.00000000, TOLERANCE);
		}
		else {
			checkClose(final->coordinates[37*i +  0][0], -81.29324314, TOLERANCE);	checkClose(final->coordinates[37*i +  0][1], 55.17360315, TOLERANCE);
			checkClose(final->coordinates[37*i +  1][0], -83.53553356, TOLERANCE);	checkClose(final->coordinates[37*i +  1][1], 56.46446994, TOLERANCE);
			checkClose(final->coordinates[37*i +  2][0], -80.86216209, TOLERANCE);	checkClose(final->coordinates[37*i +  2][1], 56.78240210, TOLERANCE);
			checkClose(final->coordinates[37*i +  3][0], -82.35702237, TOLERANCE);	checkClose(final->coordinates[37*i +  3][1], 57.64297996, TOLERANCE);
			checkClose(final->coordinates[37*i +  4][0], -80.43108105, TOLERANCE);	checkClose(final->coordinates[37*i +  4][1], 58.39120105, TOLERANCE);
			checkClose(final->coordinates[37*i +  5][0], -81.17851119, TOLERANCE);	checkClose(final->coordinates[37*i +  5][1], 58.82148998, TOLERANCE);
			checkClose(final->coordinates[37*i +  6][0], -80.00000000, TOLERANCE);	checkClose(final->coordinates[37*i +  6][1], 60.00000000, TOLERANCE);
			checkClose(final->coordinates[37*i +  7][0], -78.70633527, TOLERANCE);	checkClose(final->coordinates[37*i +  7][1], 55.17370341, TOLERANCE);
			checkClose(final->coordinates[37*i +  8][0], -79.13755684, TOLERANCE);	checkClose(final->coordinates[37*i +  8][1], 56.78246894, TOLERANCE);
			checkClose(final->coordinates[37*i +  9][0], -79.56877842, TOLERANCE);	checkClose(final->coordinates[37*i +  9][1], 58.39123447, TOLERANCE);
			checkClose(final->coordinates[37*i + 10][0], -76.46446223, TOLERANCE);	checkClose(final->coordinates[37*i + 10][1], 56.46446615, TOLERANCE);
			checkClose(final->coordinates[37*i + 11][0], -77.64297482, TOLERANCE);	checkClose(final->coordinates[37*i + 11][1], 57.64297743, TOLERANCE);
			checkClose(final->coordinates[37*i + 12][0], -78.82148741, TOLERANCE);	checkClose(final->coordinates[37*i + 12][1], 58.82148872, TOLERANCE);
			checkClose(final->coordinates[37*i + 13][0], -75.17360409, TOLERANCE);	checkClose(final->coordinates[37*i + 13][1], 58.70675817, TOLERANCE);
			checkClose(final->coordinates[37*i + 14][0], -76.78240273, TOLERANCE);	checkClose(final->coordinates[37*i + 14][1], 59.13783878, TOLERANCE);
			checkClose(final->coordinates[37*i + 15][0], -78.39120136, TOLERANCE);	checkClose(final->coordinates[37*i + 15][1], 59.56891939, TOLERANCE);
			checkClose(final->coordinates[37*i + 16][0], -75.17370458, TOLERANCE);	checkClose(final->coordinates[37*i + 16][1], 61.29367181, TOLERANCE);
			checkClose(final->coordinates[37*i + 17][0], -76.78246972, TOLERANCE);	checkClose(final->coordinates[37*i + 17][1], 60.86244787, TOLERANCE);
			checkClose(final->coordinates[37*i + 18][0], -78.39123486, TOLERANCE);	checkClose(final->coordinates[37*i + 18][1], 60.43122394, TOLERANCE);
			checkClose(final->coordinates[37*i + 19][0], -76.46446244, TOLERANCE);	checkClose(final->coordinates[37*i + 19][1], 63.53553405, TOLERANCE);
			checkClose(final->coordinates[37*i + 20][0], -77.64297496, TOLERANCE);	checkClose(final->coordinates[37*i + 20][1], 62.35702270, TOLERANCE);
			checkClose(final->coordinates[37*i + 21][0], -78.82148748, TOLERANCE);	checkClose(final->coordinates[37*i + 21][1], 61.17851135, TOLERANCE);
			checkClose(final->coordinates[37*i + 22][0], -84.82640208, TOLERANCE);	checkClose(final->coordinates[37*i + 22][1], 58.70675772, TOLERANCE);
			checkClose(final->coordinates[37*i + 23][0], -83.21760139, TOLERANCE);	checkClose(final->coordinates[37*i + 23][1], 59.13783848, TOLERANCE);
			checkClose(final->coordinates[37*i + 24][0], -81.60880069, TOLERANCE);	checkClose(final->coordinates[37*i + 24][1], 59.56891924, TOLERANCE);
			checkClose(final->coordinates[37*i + 25][0], -84.82630546, TOLERANCE);	checkClose(final->coordinates[37*i + 25][1], 61.29365582, TOLERANCE);
			checkClose(final->coordinates[37*i + 26][0], -83.21753697, TOLERANCE);	checkClose(final->coordinates[37*i + 26][1], 60.86243722, TOLERANCE);
			checkClose(final->coordinates[37*i + 27][0], -81.60876849, TOLERANCE);	checkClose(final->coordinates[37*i + 27][1], 60.43121861, TOLERANCE);
			checkClose(final->coordinates[37*i + 28][0], -83.53553356, TOLERANCE);	checkClose(final->coordinates[37*i + 28][1], 63.53553006, TOLERANCE);
			checkClose(final->coordinates[37*i + 29][0], -82.35702237, TOLERANCE);	checkClose(final->coordinates[37*i + 29][1], 62.35702004, TOLERANCE);
			checkClose(final->coordinates[37*i + 30][0], -81.17851119, TOLERANCE);	checkClose(final->coordinates[37*i + 30][1], 61.17851002, TOLERANCE);
			checkClose(final->coordinates[37*i + 31][0], -81.29324460, TOLERANCE);	checkClose(final->coordinates[37*i + 31][1], 64.82640211, TOLERANCE);
			checkClose(final->coordinates[37*i + 32][0], -80.86216307, TOLERANCE);	checkClose(final->coordinates[37*i + 32][1], 63.21760141, TOLERANCE);
			checkClose(final->coordinates[37*i + 33][0], -80.43108153, TOLERANCE);	checkClose(final->coordinates[37*i + 33][1], 61.60880070, TOLERANCE);
			checkClose(final->coordinates[37*i + 34][0], -78.70633402, TOLERANCE);	checkClose(final->coordinates[37*i + 34][1], 64.82630191, TOLERANCE);
			checkClose(final->coordinates[37*i + 35][0], -79.13755601, TOLERANCE);	checkClose(final->coordinates[37*i + 35][1], 63.21753461, TOLERANCE);
			checkClose(final->coordinates[37*i + 36][0], -79.56877801, TOLERANCE);	checkClose(final->coordinates[37*i + 36][1], 61.60876730, TOLERANCE);
		}
	}
}

TestSuiteEnd()