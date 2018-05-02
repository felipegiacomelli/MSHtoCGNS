#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader3D.hpp>

#define TOLERANCE 1e-12

struct Region1_ElementType1_3D_Msh {
	Region1_ElementType1_3D_Msh() {
		MshReader3D mshReader3D(std::string(TEST_INPUT_DIRECTORY) + "IO/3D-Region1-ElementType1/14v_24e.msh");
		this->gridData = mshReader3D.gridData;
	}

	~Region1_ElementType1_3D_Msh() = default;

	GridDataShared gridData;
};

FixtureTestSuite(ReadMsh_Region1_ElementType1_3D, Region1_ElementType1_3D_Msh)

TestCase(Coordinates) {
	auto coordinates = this->gridData->coordinates;

	checkEqual(static_cast<int>(coordinates.size()), 14);
	checkClose(coordinates[ 0][0], 0.0, TOLERANCE); checkClose(coordinates[ 0][1], 0.0, TOLERANCE); checkClose(coordinates[ 0][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 1][0], 1.0, TOLERANCE); checkClose(coordinates[ 1][1], 0.0, TOLERANCE); checkClose(coordinates[ 1][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 2][0], 1.0, TOLERANCE); checkClose(coordinates[ 2][1], 1.0, TOLERANCE); checkClose(coordinates[ 2][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 3][0], 0.0, TOLERANCE); checkClose(coordinates[ 3][1], 1.0, TOLERANCE); checkClose(coordinates[ 3][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 4][0], 0.0, TOLERANCE); checkClose(coordinates[ 4][1], 0.0, TOLERANCE); checkClose(coordinates[ 4][2], 1.0, TOLERANCE);
	checkClose(coordinates[ 5][0], 1.0, TOLERANCE); checkClose(coordinates[ 5][1], 0.0, TOLERANCE); checkClose(coordinates[ 5][2], 1.0, TOLERANCE);
	checkClose(coordinates[ 6][0], 1.0, TOLERANCE); checkClose(coordinates[ 6][1], 1.0, TOLERANCE); checkClose(coordinates[ 6][2], 1.0, TOLERANCE);
	checkClose(coordinates[ 7][0], 0.0, TOLERANCE); checkClose(coordinates[ 7][1], 1.0, TOLERANCE); checkClose(coordinates[ 7][2], 1.0, TOLERANCE);
	checkClose(coordinates[ 8][0], 0.0, TOLERANCE); checkClose(coordinates[ 8][1], 0.5, TOLERANCE); checkClose(coordinates[ 8][2], 0.5, TOLERANCE);
	checkClose(coordinates[ 9][0], 1.0, TOLERANCE); checkClose(coordinates[ 9][1], 0.5, TOLERANCE); checkClose(coordinates[ 9][2], 0.5, TOLERANCE);
	checkClose(coordinates[10][0], 0.5, TOLERANCE); checkClose(coordinates[10][1], 0.0, TOLERANCE); checkClose(coordinates[10][2], 0.5, TOLERANCE);
	checkClose(coordinates[11][0], 0.5, TOLERANCE); checkClose(coordinates[11][1], 1.0, TOLERANCE); checkClose(coordinates[11][2], 0.5, TOLERANCE);
	checkClose(coordinates[12][0], 0.5, TOLERANCE); checkClose(coordinates[12][1], 0.5, TOLERANCE); checkClose(coordinates[12][2], 0.0, TOLERANCE);
	checkClose(coordinates[13][0], 0.5, TOLERANCE); checkClose(coordinates[13][1], 0.5, TOLERANCE); checkClose(coordinates[13][2], 1.0, TOLERANCE);
}

TestCase(Elements) {
	auto tetrahedra = this->gridData->tetrahedronConnectivity;

	checkEqual(static_cast<int>(tetrahedra.size()), 24);
	checkEqual(tetrahedra[ 0][0], 13); checkEqual(tetrahedra[ 0][1],  9); checkEqual(tetrahedra[ 0][2], 10); checkEqual(tetrahedra[ 0][3], 12); checkEqual(tetrahedra[ 0][4],  0);
	checkEqual(tetrahedra[ 1][0], 13); checkEqual(tetrahedra[ 1][1], 11); checkEqual(tetrahedra[ 1][2],  9); checkEqual(tetrahedra[ 1][3], 12); checkEqual(tetrahedra[ 1][4],  1);
	checkEqual(tetrahedra[ 2][0],  8); checkEqual(tetrahedra[ 2][1], 13); checkEqual(tetrahedra[ 2][2], 10); checkEqual(tetrahedra[ 2][3], 12); checkEqual(tetrahedra[ 2][4],  2);
	checkEqual(tetrahedra[ 3][0], 13); checkEqual(tetrahedra[ 3][1],  8); checkEqual(tetrahedra[ 3][2], 11); checkEqual(tetrahedra[ 3][3], 12); checkEqual(tetrahedra[ 3][4],  3);
	checkEqual(tetrahedra[ 4][0],  0); checkEqual(tetrahedra[ 4][1],  3); checkEqual(tetrahedra[ 4][2],  8); checkEqual(tetrahedra[ 4][3], 12); checkEqual(tetrahedra[ 4][4],  4);
	checkEqual(tetrahedra[ 5][0],  8); checkEqual(tetrahedra[ 5][1],  7); checkEqual(tetrahedra[ 5][2],  4); checkEqual(tetrahedra[ 5][3], 13); checkEqual(tetrahedra[ 5][4],  5);
	checkEqual(tetrahedra[ 6][0],  5); checkEqual(tetrahedra[ 6][1],  9); checkEqual(tetrahedra[ 6][2],  1); checkEqual(tetrahedra[ 6][3], 10); checkEqual(tetrahedra[ 6][4],  6);
	checkEqual(tetrahedra[ 7][0],  9); checkEqual(tetrahedra[ 7][1],  2); checkEqual(tetrahedra[ 7][2],  1); checkEqual(tetrahedra[ 7][3], 12); checkEqual(tetrahedra[ 7][4],  7);
	checkEqual(tetrahedra[ 8][0],  3); checkEqual(tetrahedra[ 8][1],  7); checkEqual(tetrahedra[ 8][2],  8); checkEqual(tetrahedra[ 8][3], 11); checkEqual(tetrahedra[ 8][4],  8);
	checkEqual(tetrahedra[ 9][0], 13); checkEqual(tetrahedra[ 9][1],  7); checkEqual(tetrahedra[ 9][2],  6); checkEqual(tetrahedra[ 9][3], 11); checkEqual(tetrahedra[ 9][4],  9);
	checkEqual(tetrahedra[10][0],  5); checkEqual(tetrahedra[10][1], 13); checkEqual(tetrahedra[10][2],  6); checkEqual(tetrahedra[10][3],  9); checkEqual(tetrahedra[10][4], 10);
	checkEqual(tetrahedra[11][0],  6); checkEqual(tetrahedra[11][1], 11); checkEqual(tetrahedra[11][2],  2); checkEqual(tetrahedra[11][3],  9); checkEqual(tetrahedra[11][4], 11);
	checkEqual(tetrahedra[12][0], 11); checkEqual(tetrahedra[12][1],  3); checkEqual(tetrahedra[12][2],  2); checkEqual(tetrahedra[12][3], 12); checkEqual(tetrahedra[12][4], 12);
	checkEqual(tetrahedra[13][0],  5); checkEqual(tetrahedra[13][1],  4); checkEqual(tetrahedra[13][2], 13); checkEqual(tetrahedra[13][3], 10); checkEqual(tetrahedra[13][4], 13);
	checkEqual(tetrahedra[14][0],  0); checkEqual(tetrahedra[14][1],  8); checkEqual(tetrahedra[14][2],  4); checkEqual(tetrahedra[14][3], 10); checkEqual(tetrahedra[14][4], 14);
	checkEqual(tetrahedra[15][0],  1); checkEqual(tetrahedra[15][1],  0); checkEqual(tetrahedra[15][2], 10); checkEqual(tetrahedra[15][3], 12); checkEqual(tetrahedra[15][4], 15);
	checkEqual(tetrahedra[16][0],  8); checkEqual(tetrahedra[16][1],  3); checkEqual(tetrahedra[16][2], 11); checkEqual(tetrahedra[16][3], 12); checkEqual(tetrahedra[16][4], 16);
	checkEqual(tetrahedra[17][0],  8); checkEqual(tetrahedra[17][1],  7); checkEqual(tetrahedra[17][2], 13); checkEqual(tetrahedra[17][3], 11); checkEqual(tetrahedra[17][4], 17);
	checkEqual(tetrahedra[18][0],  6); checkEqual(tetrahedra[18][1], 13); checkEqual(tetrahedra[18][2], 11); checkEqual(tetrahedra[18][3],  9); checkEqual(tetrahedra[18][4], 18);
	checkEqual(tetrahedra[19][0], 11); checkEqual(tetrahedra[19][1],  2); checkEqual(tetrahedra[19][2],  9); checkEqual(tetrahedra[19][3], 12); checkEqual(tetrahedra[19][4], 19);
	checkEqual(tetrahedra[20][0],  5); checkEqual(tetrahedra[20][1], 13); checkEqual(tetrahedra[20][2],  9); checkEqual(tetrahedra[20][3], 10); checkEqual(tetrahedra[20][4], 20);
	checkEqual(tetrahedra[21][0],  4); checkEqual(tetrahedra[21][1],  8); checkEqual(tetrahedra[21][2], 13); checkEqual(tetrahedra[21][3], 10); checkEqual(tetrahedra[21][4], 21);
	checkEqual(tetrahedra[22][0],  9); checkEqual(tetrahedra[22][1],  1); checkEqual(tetrahedra[22][2], 10); checkEqual(tetrahedra[22][3], 12); checkEqual(tetrahedra[22][4], 22);
	checkEqual(tetrahedra[23][0],  0); checkEqual(tetrahedra[23][1],  8); checkEqual(tetrahedra[23][2], 10); checkEqual(tetrahedra[23][3], 12); checkEqual(tetrahedra[23][4], 23);
}

TestCase(Facets) {
	auto lines = this->gridData->triangleConnectivity;

	checkEqual(lines.size(), 16u);
	checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 8); checkEqual(triangles[0][2], 3); checkEqual(triangles[0][3], 24);
	checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1], 4); checkEqual(triangles[1][2], 8); checkEqual(triangles[1][3], 25);
	checkEqual(triangles[2][0], 3); checkEqual(triangles[2][1], 8); checkEqual(triangles[2][2], 7); checkEqual(triangles[2][3], 26);
	checkEqual(triangles[3][0], 4); checkEqual(triangles[3][1], 7); checkEqual(triangles[3][2], 8); checkEqual(triangles[3][3], 27);
	checkEqual(triangles[0][0], 2); checkEqual(triangles[0][1], 9); checkEqual(triangles[0][2], 1); checkEqual(triangles[0][3], 28);
	checkEqual(triangles[1][0], 9); checkEqual(triangles[1][1], 5); checkEqual(triangles[1][2], 1); checkEqual(triangles[1][3], 29);
	checkEqual(triangles[2][0], 6); checkEqual(triangles[2][1], 9); checkEqual(triangles[2][2], 2); checkEqual(triangles[2][3], 30);
	checkEqual(triangles[3][0], 9); checkEqual(triangles[3][1], 6); checkEqual(triangles[3][2], 5); checkEqual(triangles[3][3], 31);	
	checkEqual(triangles[0][0],  1); checkEqual(triangles[0][1], 10); checkEqual(triangles[0][2], 0); checkEqual(triangles[0][3], 32);
	checkEqual(triangles[1][0], 10); checkEqual(triangles[1][1],  4); checkEqual(triangles[1][2], 0); checkEqual(triangles[1][3], 33);
	checkEqual(triangles[2][0],  5); checkEqual(triangles[2][1], 10); checkEqual(triangles[2][2], 1); checkEqual(triangles[2][3], 34);
	checkEqual(triangles[3][0], 10); checkEqual(triangles[3][1],  5); checkEqual(triangles[3][2], 4); checkEqual(triangles[3][3], 35);	
	checkEqual(triangles[0][0],  3); checkEqual(triangles[0][1], 11); checkEqual(triangles[0][2], 2); checkEqual(triangles[0][3], 36);
	checkEqual(triangles[1][0], 11); checkEqual(triangles[1][1],  6); checkEqual(triangles[1][2], 2); checkEqual(triangles[1][3], 37);
	checkEqual(triangles[2][0],  7); checkEqual(triangles[2][1], 11); checkEqual(triangles[2][2], 3); checkEqual(triangles[2][3], 38);
	checkEqual(triangles[3][0], 11); checkEqual(triangles[3][1],  7); checkEqual(triangles[3][2], 6); checkEqual(triangles[3][3], 39);	

		checkEqual(triangles[0][0],  0); checkEqual(triangles[0][1], 12); checkEqual(triangles[0][2],  1); checkEqual(triangles[0][3], 40);
	checkEqual(triangles[1][0],  0); checkEqual(triangles[1][1],  3); checkEqual(triangles[1][2], 12); checkEqual(triangles[1][3], 41);
	checkEqual(triangles[2][0],  1); checkEqual(triangles[2][1], 12); checkEqual(triangles[2][2],  2); checkEqual(triangles[2][3], 42);
	checkEqual(triangles[3][0],  2); checkEqual(triangles[3][1], 12); checkEqual(triangles[3][2],  3); checkEqual(triangles[3][3], 43);

		checkEqual(triangles[0][0],  5); checkEqual(triangles[0][1], 13); checkEqual(triangles[0][2], 4); checkEqual(triangles[0][3], 44);
	checkEqual(triangles[1][0], 13); checkEqual(triangles[1][1],  7); checkEqual(triangles[1][2], 4); checkEqual(triangles[1][3], 45);
	checkEqual(triangles[2][0],  6); checkEqual(triangles[2][1], 13); checkEqual(triangles[2][2], 5); checkEqual(triangles[2][3], 46);
	checkEqual(triangles[3][0],  7); checkEqual(triangles[3][1], 13); checkEqual(triangles[3][2], 6); checkEqual(triangles[3][3], 47);

}

TestCase(Boundaries) {
	checkEqual(this->gridData->boundaries.size(), 6u);
}

TestCase(West) {
	BoundaryData west = this->gridData->boundaries[0];

	check(west.name == std::string("West"));

	auto triangles = west.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);


	auto vertices = west.vertices;
	checkEqual(static_cast<int>(vertices.size()), 5);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 3);
	checkEqual(vertices[2], 4);
	checkEqual(vertices[3], 7);
	checkEqual(vertices[4], 8);
}

TestCase(East) {
	BoundaryData east = this->gridData->boundaries[1];

	check(east.name == std::string("East"));

	auto triangles = east.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	check(std::all_of(triangles.cbegin(), triangles.cend(), [](const auto& connectivity){return connectivity.size() == unsigned(4);}));


	auto vertices = east.vertices;
	checkEqual(static_cast<int>(vertices.size()), 5);
	checkEqual(vertices[0], 1);
	checkEqual(vertices[1], 2);
	checkEqual(vertices[2], 5);
	checkEqual(vertices[3], 6);
	checkEqual(vertices[4], 9);
}

TestCase(South) {
	BoundaryData south = this->gridData->boundaries[2];

	check(south.name == std::string("South"));

	auto triangles = south.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	check(std::all_of(triangles.cbegin(), triangles.cend(), [](const auto& connectivity){return connectivity.size() == unsigned(4);}));


	auto vertices = south.vertices;
	checkEqual(static_cast<int>(vertices.size()), 5);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 1);
	checkEqual(vertices[2], 4);
	checkEqual(vertices[3], 5);
	checkEqual(vertices[4], 10);
}

TestCase(North) {
	BoundaryData north = this->gridData->boundaries[3];

	check(north.name == std::string("North"));

	auto triangles = north.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	check(std::all_of(triangles.cbegin(), triangles.cend(), [](const auto& connectivity){return connectivity.size() == unsigned(4);}));


	auto vertices = north.vertices;
	checkEqual(static_cast<int>(vertices.size()), 5);
	checkEqual(vertices[0],  2);
	checkEqual(vertices[1],  3);
	checkEqual(vertices[2],  6);
	checkEqual(vertices[3],  7);
	checkEqual(vertices[4], 11);
}

TestCase(Bottom) {
	BoundaryData bottom = this->gridData->boundaries[4];

	check(bottom.name == std::string("Bottom"));

	auto triangles = bottom.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	check(std::all_of(triangles.cbegin(), triangles.cend(), [](const auto& connectivity){return connectivity.size() == unsigned(4);}));


	auto vertices = bottom.vertices;
	checkEqual(static_cast<int>(vertices.size()), 5);
	checkEqual(vertices[0],  0);
	checkEqual(vertices[1],  1);
	checkEqual(vertices[2],  2);
	checkEqual(vertices[3],  3);
	checkEqual(vertices[4], 12);
}

TestCase(Top) {
	BoundaryData top = this->gridData->boundaries[5];

	check(top.name == std::string("Top"));

	auto triangles = top.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	check(std::all_of(triangles.cbegin(), triangles.cend(), [](const auto& connectivity){return connectivity.size() == unsigned(4);}));


	auto vertices = top.vertices;
	checkEqual(static_cast<int>(vertices.size()), 5);
	checkEqual(vertices[0],  4);
	checkEqual(vertices[1],  5);
	checkEqual(vertices[2],  6);
	checkEqual(vertices[3],  7);
	checkEqual(vertices[4], 13);
}

TestCase(Regions) {
	checkEqual(this->gridData->regions.size(), 1u);

	auto elementsOnRegion = this->gridData->regions[0].elementsOnRegion;
	checkEqual(static_cast<int>(elementsOnRegion.size()), 24);
	checkEqual(elementsOnRegion[ 0], 0);
	checkEqual(elementsOnRegion[ 1], 1);
	checkEqual(elementsOnRegion[ 2], 2);
	checkEqual(elementsOnRegion[ 3], 3);
	checkEqual(elementsOnRegion[ 4], 4);
	checkEqual(elementsOnRegion[ 5], 5);
	checkEqual(elementsOnRegion[ 6], 6);
	checkEqual(elementsOnRegion[ 7], 7);
	checkEqual(elementsOnRegion[ 8], 8);
	checkEqual(elementsOnRegion[ 9], 9);
	checkEqual(elementsOnRegion[10], 10);
	checkEqual(elementsOnRegion[11], 11);
	checkEqual(elementsOnRegion[12], 12);
	checkEqual(elementsOnRegion[13], 13);
	checkEqual(elementsOnRegion[14], 14);
	checkEqual(elementsOnRegion[15], 15);
	checkEqual(elementsOnRegion[16], 16);
	checkEqual(elementsOnRegion[17], 17);
	checkEqual(elementsOnRegion[18], 18);
	checkEqual(elementsOnRegion[19], 19);
	checkEqual(elementsOnRegion[20], 20);
	checkEqual(elementsOnRegion[21], 21);
	checkEqual(elementsOnRegion[22], 22);
	checkEqual(elementsOnRegion[23], 23);
}

TestSuiteEnd()
