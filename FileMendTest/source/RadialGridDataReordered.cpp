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
	GridDataShared gridData;
};

FixtureTestSuite(RadialGridDataReorderSuite, RadialGridDataReorderFixture)

TestCase(RadialGridDataReorderTest) {
	RadialGridDataReordered radialGridDataReordered(this->gridData);

	printGridDataInformation(this->gridData, "gD");

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

	// for (int i = 0; i < 9; i++) {
	// 	int shift = 36 * i;

	// 	for (int j = 0; j < 24; j++)
	// 		checkEqual(reordered->hexahedronConnectivity[24*i + j].back(), shift++);

	// 	for (int j = 0; j < 12; j++)
	// 		checkEqual(reordered->prismConnectivity[12*i + j].back(), shift++);
	// }

	// for (int i = 0; i < 9; i++)
	// 	checkEqual(reordered->lineConnectivity[i].back(), 324 + i);

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

		// checkClose(calculateDistance(reordered->coordinates[37 * 0 + i], reordered->coordinates[37 *  1 + i]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 1 + i], reordered->coordinates[37 *  2 + i]), 5.5555555555555554e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 2 + i], reordered->coordinates[37 *  3 + i]), 5.5555555555555536e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 3 + i], reordered->coordinates[37 *  4 + i]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 4 + i], reordered->coordinates[37 *  5 + i]), 5.5555555555555571e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 5 + i], reordered->coordinates[37 *  6 + i]), 5.5555555555555500e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 6 + i], reordered->coordinates[37 *  7 + i]), 5.5555555555555642e+00, TOLERANCE);
		// checkClose(calculateDistance(reordered->coordinates[37 * 7 + i], reordered->coordinates[37 *  8 + i]), 5.5555538601345447e+00, TOLERANCE);
		checkClose(calculateDistance(reordered->coordinates[37 * 8 + i], reordered->coordinates[37 *  9 + i]), 5.5555572509765625e+00, TOLERANCE);
	}


	// {
	// 	auto boundary = *std::find_if(reordered->boundaries.cbegin(),reordered->boundaries.cend(), [](const auto& b){return b.name == std::string("BOUNDARY_BOTTOM");});
	// 	std::cout << std::endl << "\t" << boundary.name + " : " << boundary.facetBegin << " - " << boundary.facetEnd << " | " << boundary.facetEnd - boundary.facetBegin << std::endl << std::endl;
	// 	std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates.front()[0];
	// 	std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates.front()[1];
	// 	std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates.front()[2];
	// 	std::cout << std::endl << std::endl;

	// 	for (auto element = reordered->triangleConnectivity.cbegin(); element < reordered->triangleConnectivity.cbegin() + 12; element++) {
	// 		for (auto v = element->cbegin(); v < element->cend() - 1; v++)
	// 				std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << std::endl;

	// 	for (auto element = reordered->prismConnectivity.cbegin(); element < reordered->prismConnectivity.cbegin() + 12; element++) {
	// 		for (auto v = element->cbegin(); v < element->cend() - 1; v++)
	// 			if (hasElement(boundary.vertices.cbegin(), boundary.vertices.cend(), *v))
	// 				std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
	// 			else
	// 				std::cout << "\t" << std::setw(4) << std::right << *v;
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << std::endl;

	// 	for (int i = 0; i < 6; i++) {
	// 		std::cout << "\t" << std::setw(4) << std::right << reordered->prismConnectivity.front()[i] << " : ";
	// 		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates[reordered->prismConnectivity.front()[i]][0];
	// 		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates[reordered->prismConnectivity.front()[i]][1];
	// 		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates[reordered->prismConnectivity.front()[i]][2];
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << std::endl;

	// 	for (auto element = reordered->quadrangleConnectivity.cbegin(); element < reordered->quadrangleConnectivity.cbegin() + 12; element++) {
	// 		for (auto v = element->cbegin(); v < element->cend() - 1; v++)
	// 				std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << std::endl;

	// 	for (auto element = reordered->hexahedronConnectivity.cbegin(); element < reordered->hexahedronConnectivity.cbegin() + 24; element++) {
	// 		for (auto v = element->cbegin(); v < element->cend() - 1; v++)
	// 			if (hasElement(boundary.vertices.cbegin(), boundary.vertices.cend(), *v))
	// 				std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
	// 			else
	// 				std::cout << "\t" << std::setw(4) << std::right << *v;
	// 		std::cout << std::endl;
	// 	}

	// 	std::cout << std::endl;
	// 	for (int i = 0; i < 8; i++) {
	// 		std::cout << "\t" << std::setw(4) << std::right << reordered->hexahedronConnectivity.front()[i] << " : ";
	// 		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates[reordered->hexahedronConnectivity.front()[i]][0];
	// 		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates[reordered->hexahedronConnectivity.front()[i]][1];
	// 		std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates[reordered->hexahedronConnectivity.front()[i]][2];
	// 		std::cout << std::endl;
	// 	}


	// 	std::cout << std::endl;
	// }

	// {
	// 	auto boundary = *std::find_if(reordered->boundaries.cbegin(),reordered->boundaries.cend(), [](const auto& b){return b.name == std::string("BOUNDARY_TOP");});
	// 	std::cout << std::endl << "\t" << boundary.name << std::endl << std::endl;
	// 	std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates.back()[0];
	// 	std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates.back()[1];
	// 	std::cout << "\t" << std::setfill(' ') << std::setw(12) << std::right << std::setprecision(6) << std::fixed << reordered->coordinates.back()[2];
	// 	std::cout << std::endl << std::endl;

	// 	for (auto element = reordered->hexahedronConnectivity.cend() - 24; element < reordered->hexahedronConnectivity.cend(); element++) {
	// 		for (auto v = element->cbegin(); v < element->cend() - 1; v++)
	// 			if (hasElement(boundary.vertices.cbegin(), boundary.vertices.cend(), *v))
	// 				std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
	// 			else
	// 				std::cout << "\t" << std::setw(4) << std::right << *v;
	// 		std::cout << std::endl;
	// 	}

	// 	std::cout << std::endl;

	// 	for (auto element = reordered->prismConnectivity.cend() - 12; element < reordered->prismConnectivity.cend(); element++) {
	// 		for (auto v = element->cbegin(); v < element->cend() - 1; v++)
	// 			if (hasElement(boundary.vertices.cbegin(), boundary.vertices.cend(), *v))
	// 				std::cout << "\t" << "\033[1;31m" << std::setw(4) << std::right  << *v << "\033[0m";
	// 			else
	// 				std::cout << "\t" << std::setw(4) << std::right << *v;
	// 		std::cout << std::endl;
	// 	}

	// 	std::cout << std::endl;
	// }
}

TestSuiteEnd()