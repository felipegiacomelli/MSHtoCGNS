#define BOOST_TEST_MODULE IOTestModule

#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/GridReader2D.hpp>
#include <IO/GridReader3D.hpp>

#define TOLERANCE 1e-12

struct Fixture2D {
	Fixture2D() {
		GridReader2D gridReader2D("/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/2D/5n_8e.msh");
		this->gridData = gridReader2D.getGridData();
	}

	~Fixture2D() = default;

	GridData gridData;
};

FixtureTestSuite(Suite2D, Fixture2D)

TestCase(Coordinates) {
	std::vector<std::vector<double>> coordinates = this->gridData.coordinates;

	checkEqual(static_cast<int>(coordinates.size()), 5);
	checkClose(coordinates[0][0], 0.0, TOLERANCE); checkClose(coordinates[0][1], 0.0, TOLERANCE); checkClose(coordinates[0][2], 0.0, TOLERANCE);
	checkClose(coordinates[1][0], 1.0, TOLERANCE); checkClose(coordinates[1][1], 0.0, TOLERANCE); checkClose(coordinates[1][2], 0.0, TOLERANCE);
	checkClose(coordinates[2][0], 1.0, TOLERANCE); checkClose(coordinates[2][1], 1.0, TOLERANCE); checkClose(coordinates[2][2], 0.0, TOLERANCE);
	checkClose(coordinates[3][0], 0.0, TOLERANCE); checkClose(coordinates[3][1], 1.0, TOLERANCE); checkClose(coordinates[3][2], 0.0, TOLERANCE);
	checkClose(coordinates[4][0], 0.5, TOLERANCE); checkClose(coordinates[4][1], 0.5, TOLERANCE); checkClose(coordinates[4][2], 0.0, TOLERANCE);
}

TestCase(Elements) {
	std::vector<std::vector<int>> triangles = this->gridData.triangleConnectivity;

	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 1); checkEqual(triangles[0][2], 4);
	checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1], 4); checkEqual(triangles[1][2], 3);
	checkEqual(triangles[2][0], 1); checkEqual(triangles[2][1], 2); checkEqual(triangles[2][2], 4);
	checkEqual(triangles[3][0], 2); checkEqual(triangles[3][1], 3); checkEqual(triangles[3][2], 4);
}

TestCase(West) {
	BoundaryData west = this->gridData.boundaries[0];

	check(west.name == std::string("West"));

	std::vector<std::vector<int>> lines = west.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 3); checkEqual(lines[0][1], 0);
}

TestCase(East) {
	BoundaryData east = this->gridData.boundaries[1];

	check(east.name == std::string("East"));

	std::vector<std::vector<int>> lines = east.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 1); checkEqual(lines[0][1], 2);
}

TestCase(South) {
	BoundaryData south = this->gridData.boundaries[2];

	check(south.name == std::string("South"));

	std::vector<std::vector<int>> lines = south.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 0); checkEqual(lines[0][1], 1);
}

TestCase(North) {
	BoundaryData north = this->gridData.boundaries[3];

	check(north.name == std::string("North"));

	std::vector<std::vector<int>> lines = north.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 2); checkEqual(lines[0][1], 3);
}

TestSuiteEnd()

struct Fixture3D {
	Fixture3D() {
		GridReader3D gridReader3D("/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/3D/14n_48e.msh");
		this->gridData = gridReader3D.getGridData();
	}

	~Fixture3D() = default;

	GridData gridData;
};

FixtureTestSuite(Suite3D, Fixture3D)

TestCase(Coordinates) {
	std::vector<std::vector<double>> coordinates = this->gridData.coordinates;

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
	std::vector<std::vector<int>> tetrahedra = this->gridData.tetrahedronConnectivity;

	checkEqual(static_cast<int>(tetrahedra.size()), 24);
	checkEqual(tetrahedra[ 0][0], 11); checkEqual(tetrahedra[ 0][1], 13); checkEqual(tetrahedra[ 0][2], 12); checkEqual(tetrahedra[ 0][3],  9);
	checkEqual(tetrahedra[ 1][0], 13); checkEqual(tetrahedra[ 1][1],  8); checkEqual(tetrahedra[ 1][2], 11); checkEqual(tetrahedra[ 1][3], 12);
	checkEqual(tetrahedra[ 2][0], 13); checkEqual(tetrahedra[ 2][1], 12); checkEqual(tetrahedra[ 2][2],  9); checkEqual(tetrahedra[ 2][3], 10);
	checkEqual(tetrahedra[ 3][0], 13); checkEqual(tetrahedra[ 3][1],  8); checkEqual(tetrahedra[ 3][2], 12); checkEqual(tetrahedra[ 3][3], 10);
	checkEqual(tetrahedra[ 4][0],  6); checkEqual(tetrahedra[ 4][1], 11); checkEqual(tetrahedra[ 4][2],  2); checkEqual(tetrahedra[ 4][3],  9);
	checkEqual(tetrahedra[ 5][0],  8); checkEqual(tetrahedra[ 5][1],  7); checkEqual(tetrahedra[ 5][2],  4); checkEqual(tetrahedra[ 5][3], 13);
	checkEqual(tetrahedra[ 6][0], 12); checkEqual(tetrahedra[ 6][1],  2); checkEqual(tetrahedra[ 6][2],  9); checkEqual(tetrahedra[ 6][3],  1);
	checkEqual(tetrahedra[ 7][0], 11); checkEqual(tetrahedra[ 7][1],  3); checkEqual(tetrahedra[ 7][2],  2); checkEqual(tetrahedra[ 7][3], 12);
	checkEqual(tetrahedra[ 8][0], 13); checkEqual(tetrahedra[ 8][1],  7); checkEqual(tetrahedra[ 8][2],  6); checkEqual(tetrahedra[ 8][3], 11);
	checkEqual(tetrahedra[ 9][0], 12); checkEqual(tetrahedra[ 9][1],  0); checkEqual(tetrahedra[ 9][2],  1); checkEqual(tetrahedra[ 9][3], 10);
	checkEqual(tetrahedra[10][0],  3); checkEqual(tetrahedra[10][1],  7); checkEqual(tetrahedra[10][2],  8); checkEqual(tetrahedra[10][3], 11);
	checkEqual(tetrahedra[11][0],  5); checkEqual(tetrahedra[11][1], 13); checkEqual(tetrahedra[11][2],  6); checkEqual(tetrahedra[11][3],  9);
	checkEqual(tetrahedra[12][0],  0); checkEqual(tetrahedra[12][1],  3); checkEqual(tetrahedra[12][2],  8); checkEqual(tetrahedra[12][3], 12);
	checkEqual(tetrahedra[13][0],  5); checkEqual(tetrahedra[13][1],  9); checkEqual(tetrahedra[13][2],  1); checkEqual(tetrahedra[13][3], 10);
	checkEqual(tetrahedra[14][0],  5); checkEqual(tetrahedra[14][1],  4); checkEqual(tetrahedra[14][2], 13); checkEqual(tetrahedra[14][3], 10);
	checkEqual(tetrahedra[15][0],  0); checkEqual(tetrahedra[15][1],  8); checkEqual(tetrahedra[15][2],  4); checkEqual(tetrahedra[15][3], 10);
	checkEqual(tetrahedra[16][0],  6); checkEqual(tetrahedra[16][1], 13); checkEqual(tetrahedra[16][2], 11); checkEqual(tetrahedra[16][3],  9);
	checkEqual(tetrahedra[17][0],  2); checkEqual(tetrahedra[17][1], 11); checkEqual(tetrahedra[17][2], 12); checkEqual(tetrahedra[17][3],  9);
	checkEqual(tetrahedra[18][0],  8); checkEqual(tetrahedra[18][1],  7); checkEqual(tetrahedra[18][2], 13); checkEqual(tetrahedra[18][3], 11);
	checkEqual(tetrahedra[19][0],  8); checkEqual(tetrahedra[19][1],  3); checkEqual(tetrahedra[19][2], 11); checkEqual(tetrahedra[19][3], 12);
	checkEqual(tetrahedra[20][0],  9); checkEqual(tetrahedra[20][1], 12); checkEqual(tetrahedra[20][2],  1); checkEqual(tetrahedra[20][3], 10);
	checkEqual(tetrahedra[21][0],  8); checkEqual(tetrahedra[21][1],  0); checkEqual(tetrahedra[21][2], 12); checkEqual(tetrahedra[21][3], 10);
	checkEqual(tetrahedra[22][0],  5); checkEqual(tetrahedra[22][1], 13); checkEqual(tetrahedra[22][2],  9); checkEqual(tetrahedra[22][3], 10);
	checkEqual(tetrahedra[23][0],  4); checkEqual(tetrahedra[23][1],  8); checkEqual(tetrahedra[23][2], 13); checkEqual(tetrahedra[23][3], 10);
}

TestCase(West) {
	BoundaryData west = this->gridData.boundaries[0];

	check(west.name == std::string("West"));

	std::vector<std::vector<int>> triangles = west.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 8); checkEqual(triangles[0][2], 3);
	checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1], 4); checkEqual(triangles[1][2], 8);
	checkEqual(triangles[2][0], 3); checkEqual(triangles[2][1], 8); checkEqual(triangles[2][2], 7);
	checkEqual(triangles[3][0], 4); checkEqual(triangles[3][1], 7); checkEqual(triangles[3][2], 8);
}

TestCase(East) {
	BoundaryData east = this->gridData.boundaries[1];

	check(east.name == std::string("East"));

	std::vector<std::vector<int>> triangles = east.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0], 2); checkEqual(triangles[0][1], 9); checkEqual(triangles[0][2], 1);
	checkEqual(triangles[1][0], 9); checkEqual(triangles[1][1], 5); checkEqual(triangles[1][2], 1);
	checkEqual(triangles[2][0], 6); checkEqual(triangles[2][1], 9); checkEqual(triangles[2][2], 2);
	checkEqual(triangles[3][0], 9); checkEqual(triangles[3][1], 6); checkEqual(triangles[3][2], 5);
}

TestCase(South) {
	BoundaryData south = this->gridData.boundaries[2];

	check(south.name == std::string("South"));

	std::vector<std::vector<int>> triangles = south.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);	
	checkEqual(triangles[0][0],  1); checkEqual(triangles[0][1], 10); checkEqual(triangles[0][2], 0);
	checkEqual(triangles[1][0], 10); checkEqual(triangles[1][1],  4); checkEqual(triangles[1][2], 0);
	checkEqual(triangles[2][0],  5); checkEqual(triangles[2][1], 10); checkEqual(triangles[2][2], 1);
	checkEqual(triangles[3][0], 10); checkEqual(triangles[3][1],  5); checkEqual(triangles[3][2], 4);
}

TestCase(North) {
	BoundaryData north = this->gridData.boundaries[3];

	check(north.name == std::string("North"));

	std::vector<std::vector<int>> triangles = north.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0],  3); checkEqual(triangles[0][1], 11); checkEqual(triangles[0][2], 2);
	checkEqual(triangles[1][0], 11); checkEqual(triangles[1][1],  6); checkEqual(triangles[1][2], 2);
	checkEqual(triangles[2][0],  7); checkEqual(triangles[2][1], 11); checkEqual(triangles[2][2], 3);
	checkEqual(triangles[3][0], 11); checkEqual(triangles[3][1],  7); checkEqual(triangles[3][2], 6);
}

TestCase(Bottom) {
	BoundaryData bottom = this->gridData.boundaries[4];

	check(bottom.name == std::string("Bottom"));

	std::vector<std::vector<int>> triangles = bottom.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0],  0); checkEqual(triangles[0][1], 12); checkEqual(triangles[0][2],  1);
	checkEqual(triangles[1][0],  0); checkEqual(triangles[1][1],  3); checkEqual(triangles[1][2], 12);
	checkEqual(triangles[2][0],  1); checkEqual(triangles[2][1], 12); checkEqual(triangles[2][2],  2);
	checkEqual(triangles[3][0],  2); checkEqual(triangles[3][1], 12); checkEqual(triangles[3][2], 3);
}

TestCase(Top) {
	BoundaryData top = this->gridData.boundaries[5];

	check(top.name == std::string("Top"));

	std::vector<std::vector<int>> triangles = top.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0],  5); checkEqual(triangles[0][1], 13); checkEqual(triangles[0][2], 4);
	checkEqual(triangles[1][0], 13); checkEqual(triangles[1][1],  7); checkEqual(triangles[1][2], 4);
	checkEqual(triangles[2][0],  6); checkEqual(triangles[2][1], 13); checkEqual(triangles[2][2], 5);
	checkEqual(triangles[3][0],  7); checkEqual(triangles[3][1], 13); checkEqual(triangles[3][2], 6);
}

TestSuiteEnd()
