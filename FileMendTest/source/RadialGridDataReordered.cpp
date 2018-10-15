#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <FileMend/RadialGridDataReordered.hpp>
#include <Utilities/Print.hpp>

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

	checkEqual(final->triangleConnectivity.size()  ,  24u);
	checkEqual(final->quadrangleConnectivity.size(), 156u);

	checkEqual(final->lineConnectivity.size(), 9u);

	checkEqual(final->boundaries.size(),  3u);
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

	for (int i = 0; i < 37; i++) {
		checkClose(std::abs(final->coordinates[37 * 0 + i][2] - final->coordinates[37 *  1 + i][2]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 1 + i][2] - final->coordinates[37 *  2 + i][2]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 2 + i][2] - final->coordinates[37 *  3 + i][2]), 5.5555555555555536e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 3 + i][2] - final->coordinates[37 *  4 + i][2]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 4 + i][2] - final->coordinates[37 *  5 + i][2]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 5 + i][2] - final->coordinates[37 *  6 + i][2]), 5.5555555555555500e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 6 + i][2] - final->coordinates[37 *  7 + i][2]), 5.5555555555555642e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 7 + i][2] - final->coordinates[37 *  8 + i][2]), 5.5555538601345447e+00, TOLERANCE);
		checkClose(std::abs(final->coordinates[37 * 8 + i][2] - final->coordinates[37 *  9 + i][2]), 5.5555572509765625e+00, TOLERANCE);

		checkClose(calculateDistance(final->coordinates[37 * 0 + i], final->coordinates[37 *  1 + i]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(calculateDistance(final->coordinates[37 * 1 + i], final->coordinates[37 *  2 + i]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(calculateDistance(final->coordinates[37 * 2 + i], final->coordinates[37 *  3 + i]), 5.5555555555555536e+00, TOLERANCE);
		checkClose(calculateDistance(final->coordinates[37 * 3 + i], final->coordinates[37 *  4 + i]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(calculateDistance(final->coordinates[37 * 4 + i], final->coordinates[37 *  5 + i]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(calculateDistance(final->coordinates[37 * 5 + i], final->coordinates[37 *  6 + i]), 5.5555555555555500e+00, TOLERANCE);
		checkClose(calculateDistance(final->coordinates[37 * 6 + i], final->coordinates[37 *  7 + i]), 5.5555555555555642e+00, TOLERANCE);
		checkClose(calculateDistance(final->coordinates[37 * 7 + i], final->coordinates[37 *  8 + i]), 5.5555538601345447e+00, TOLERANCE);
		checkClose(calculateDistance(final->coordinates[37 * 8 + i], final->coordinates[37 *  9 + i]), 5.5555572509765625e+00, TOLERANCE);
	}


	{
		auto boundary = *std::find_if(final->boundaries.cbegin(),final->boundaries.cend(), [](const auto& b){return b.name == std::string("BOUNDARY_BOTTOM");});
		std::cout << std::endl << "\t" << boundary.name << std::endl << std::endl;
		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates.front()[0];
		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates.front()[1];
		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates.front()[2];
		std::cout << std::endl << std::endl;

		for (auto element = final->triangleConnectivity.cbegin(); element < final->triangleConnectivity.cbegin() + 12; element++) {
			for (auto v = element->cbegin(); v < element->cend() - 1; v++)
					std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
			std::cout << std::endl;
		}
		std::cout << std::endl;

		for (auto element = final->prismConnectivity.cbegin(); element < final->prismConnectivity.cbegin() + 12; element++) {
			for (auto v = element->cbegin(); v < element->cend() - 1; v++)
				if (hasElement(boundary.vertices.cbegin(), boundary.vertices.cend(), *v))
					std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
				else
					std::cout << "\t" << std::setw(4) << std::right << *v;
			std::cout << std::endl;
		}
		std::cout << std::endl;

		for (int i = 0; i < 6; i++) {
			std::cout << "\t" << std::setw(4) << std::right << final->prismConnectivity.front()[i] << " : ";
			std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates[final->prismConnectivity.front()[i]][0];
			std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates[final->prismConnectivity.front()[i]][1];
			std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates[final->prismConnectivity.front()[i]][2];
			std::cout << std::endl;
		}
		std::cout << std::endl;

		for (auto element = final->quadrangleConnectivity.cbegin(); element < final->quadrangleConnectivity.cbegin() + 12; element++) {
			for (auto v = element->cbegin(); v < element->cend() - 1; v++)
					std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
			std::cout << std::endl;
		}
		std::cout << std::endl;

		for (auto element = final->hexahedronConnectivity.cbegin(); element < final->hexahedronConnectivity.cbegin() + 24; element++) {
			for (auto v = element->cbegin(); v < element->cend() - 1; v++)
				if (hasElement(boundary.vertices.cbegin(), boundary.vertices.cend(), *v))
					std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
				else
					std::cout << "\t" << std::setw(4) << std::right << *v;
			std::cout << std::endl;
		}

		std::cout << std::endl;
		for (int i = 0; i < 8; i++) {
			std::cout << "\t" << std::setw(4) << std::right << final->hexahedronConnectivity.front()[i] << " : ";
			std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates[final->hexahedronConnectivity.front()[i]][0];
			std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates[final->hexahedronConnectivity.front()[i]][1];
			std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates[final->hexahedronConnectivity.front()[i]][2];
			std::cout << std::endl;
		}


		std::cout << std::endl;
	}

	{
		auto boundary = *std::find_if(final->boundaries.cbegin(),final->boundaries.cend(), [](const auto& b){return b.name == std::string("BOUNDARY_TOP");});
		std::cout << std::endl << "\t" << boundary.name << std::endl << std::endl;
		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates.back()[0];
		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates.back()[1];
		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << final->coordinates.back()[2];
		std::cout << std::endl << std::endl;

		for (auto element = final->hexahedronConnectivity.cend() - 24; element < final->hexahedronConnectivity.cend(); element++) {
			for (auto v = element->cbegin(); v < element->cend() - 1; v++)
				if (hasElement(boundary.vertices.cbegin(), boundary.vertices.cend(), *v))
					std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
				else
					std::cout << "\t" << std::setw(4) << std::right << *v;
			std::cout << std::endl;
		}

		std::cout << std::endl;

		for (auto element = final->prismConnectivity.cend() - 12; element < final->prismConnectivity.cend(); element++) {
			for (auto v = element->cbegin(); v < element->cend() - 1; v++)
				if (hasElement(boundary.vertices.cbegin(), boundary.vertices.cend(), *v))
					std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
				else
					std::cout << "\t" << std::setw(4) << std::right << *v;
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
}

TestSuiteEnd()