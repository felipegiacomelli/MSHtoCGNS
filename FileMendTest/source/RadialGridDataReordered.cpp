#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <FileMend/RadialGridDataReordered.hpp>

#define TOLERANCE 1e-4

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

	for (int i = 0; i < 37; i++) {
		checkSmall(final->coordinates[37 * 0 + i][2], TOLERANCE);
		checkClose(final->coordinates[37 * 1 + i][2], 5.5555555, TOLERANCE);
		checkClose(final->coordinates[37 * 2 + i][2], 11.111111, TOLERANCE);
		checkClose(final->coordinates[37 * 3 + i][2], 16.666666, TOLERANCE);
		checkClose(final->coordinates[37 * 4 + i][2], 22.222222, TOLERANCE);
		checkClose(final->coordinates[37 * 5 + i][2], 27.777777, TOLERANCE);
		checkClose(final->coordinates[37 * 6 + i][2], 33.333333, TOLERANCE);
		checkClose(final->coordinates[37 * 7 + i][2], 38.888888, TOLERANCE);
		checkClose(final->coordinates[37 * 8 + i][2], 44.444444, TOLERANCE);
		checkClose(final->coordinates[37 * 9 + i][2], 49.999999, TOLERANCE);
	}

	for (auto i = 0; i < 37; i++) {
		bool x = std::abs(final->coordinates[i][0] -final->coordinates[37 + i][0]) < 1e-2;
		bool y = std::abs(final->coordinates[i][1] -final->coordinates[37 + i][1]) < 1e-2;

		std::cout << std::setprecision(5) << std::scientific << "\t\t";

		if (x)
			std::cout << std::setfill(' ') << std::setw(12) << std::right << final->coordinates[i][0];
		else
			std::cout << std::setfill(' ') << std::setw(12) << std::right << "\033[1;31m" << final->coordinates[i][0] << "\033[0m";

		if (y)
			std::cout << std::setfill(' ') << std::setw(12) << std::right << final->coordinates[i][1];
		else
			std::cout << std::setfill(' ') << std::setw(12) << std::right << "\033[1;31m" << final->coordinates[i][1] << "\033[0m";

		std::cout << std::setfill(' ') << std::setw(12) << std::right << final->coordinates[i][2];


		std::cout << std::setprecision(5) << std::scientific << "\t\t";

		if (x)
			std::cout << std::setfill(' ') << std::setw(12) << std::right << final->coordinates[37+i][0];
		else
			std::cout << std::setfill(' ') << std::setw(12) << std::right << "\033[1;31m" << final->coordinates[37+i][0] << "\033[0m";

		if (y)
			std::cout << std::setfill(' ') << std::setw(12) << std::right << final->coordinates[37+i][1];
		else
			std::cout << std::setfill(' ') << std::setw(12) << std::right << "\033[1;31m" << final->coordinates[37+i][1] << "\033[0m";

		std::cout << std::setfill(' ') << std::setw(12) << std::right << final->coordinates[37+i][2];

		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl;

	for (int i = 0; i < 37; i++) {
		auto d(std::inner_product(final->coordinates[37 * 0 + i].cbegin(), final->coordinates[37 * 0 + i].cend(), final->coordinates[37 * 1 + i].cbegin(), 0.0, std::plus<>(), [](auto a, auto b){return (a-b)*(a-b);}));

		std::cout << "\t" << std::sqrt(d) << std::endl;

		checkClose(std::abs(final->coordinates[37 * 0 + i][2] - final->coordinates[37 *  1 + i][2]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 1 + i][2] - final->coordinates[37 *  2 + i][2]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 2 + i][2] - final->coordinates[37 *  3 + i][2]), 5.5555555555555536e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 3 + i][2] - final->coordinates[37 *  4 + i][2]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 4 + i][2] - final->coordinates[37 *  5 + i][2]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 5 + i][2] - final->coordinates[37 *  6 + i][2]), 5.5555555555555500e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 6 + i][2] - final->coordinates[37 *  7 + i][2]), 5.5555555555555642e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 7 + i][2] - final->coordinates[37 *  8 + i][2]), 5.5555538601345447e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 8 + i][2] - final->coordinates[37 *  9 + i][2]), 5.5555572509765625e+00, TOLERANCE);
	}
}

TestSuiteEnd()