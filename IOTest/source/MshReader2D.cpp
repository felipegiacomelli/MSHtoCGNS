#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader2D.hpp>

#define TOLERANCE 1e-12

struct Msh2D {
	Msh2D() {
		MshReader2D gridReader2D("/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/2D/5n_8e.msh");
		this->gridData = gridReader2D.getGridData();
	}

	~Msh2D() = default;

	GridData gridData;
};

FixtureTestSuite(ReadMsh2D, Msh2D)

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

TestCase(Boundaries) {
	std::vector<BoundaryData> boundaries = this->gridData.boundaries;

	checkEqual(static_cast<int>(boundaries.size()), 4);
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