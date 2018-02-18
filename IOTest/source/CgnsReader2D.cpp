#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/CgnsReader2D.hpp>

#define TOLERANCE 1e-12

struct Cgns2D {
	Cgns2D() {
		CgnsReader2D cgnsReader2D("/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/2D/9n_4e.cgns");
		this->gridData = cgnsReader2D.getGridData();
	}

	~Cgns2D() = default;

	GridData gridData;
};

FixtureTestSuite(ReadCgns2D, Cgns2D)

TestCase(Coordinates) {
	auto coordinates = this->gridData.coordinates;

	checkEqual(static_cast<int>(coordinates.size()), 9);
	checkClose(coordinates[0][0], 0.0, TOLERANCE); checkClose(coordinates[0][1], 0.0, TOLERANCE); checkClose(coordinates[0][2], 0.0, TOLERANCE);
	checkClose(coordinates[1][0], 0.5, TOLERANCE); checkClose(coordinates[1][1], 0.0, TOLERANCE); checkClose(coordinates[1][2], 0.0, TOLERANCE);
	checkClose(coordinates[2][0], 1.0, TOLERANCE); checkClose(coordinates[2][1], 0.0, TOLERANCE); checkClose(coordinates[2][2], 0.0, TOLERANCE);
	checkClose(coordinates[3][0], 0.0, TOLERANCE); checkClose(coordinates[3][1], 0.5, TOLERANCE); checkClose(coordinates[3][2], 0.0, TOLERANCE);
	checkClose(coordinates[4][0], 0.5, TOLERANCE); checkClose(coordinates[4][1], 0.5, TOLERANCE); checkClose(coordinates[4][2], 0.0, TOLERANCE);
	checkClose(coordinates[5][0], 1.0, TOLERANCE); checkClose(coordinates[5][1], 0.5, TOLERANCE); checkClose(coordinates[5][2], 0.0, TOLERANCE);
	checkClose(coordinates[6][0], 0.0, TOLERANCE); checkClose(coordinates[6][1], 1.0, TOLERANCE); checkClose(coordinates[6][2], 0.0, TOLERANCE);
	checkClose(coordinates[7][0], 0.5, TOLERANCE); checkClose(coordinates[7][1], 1.0, TOLERANCE); checkClose(coordinates[7][2], 0.0, TOLERANCE);
	checkClose(coordinates[8][0], 1.0, TOLERANCE); checkClose(coordinates[8][1], 1.0, TOLERANCE); checkClose(coordinates[8][2], 0.0, TOLERANCE);
}

TestCase(Elements) {
	auto quadrangles = this->gridData.quadrangleConnectivity;

	checkEqual(static_cast<int>(quadrangles.size()), 4);
	checkEqual(quadrangles[0][0], 0); checkEqual(quadrangles[0][1], 1); checkEqual(quadrangles[0][2], 4); checkEqual(quadrangles[0][3], 3);
	checkEqual(quadrangles[1][0], 1); checkEqual(quadrangles[1][1], 2); checkEqual(quadrangles[1][2], 5); checkEqual(quadrangles[1][3], 4);
	checkEqual(quadrangles[2][0], 3); checkEqual(quadrangles[2][1], 4); checkEqual(quadrangles[2][2], 7); checkEqual(quadrangles[2][3], 6);
	checkEqual(quadrangles[3][0], 4); checkEqual(quadrangles[3][1], 5); checkEqual(quadrangles[3][2], 8); checkEqual(quadrangles[3][3], 7);
}

TestCase(Boundaries) {
	auto boundaries = this->gridData.boundaries;

	checkEqual(static_cast<int>(boundaries.size()), 4);
}

TestCase(West) {
	BoundaryData west = this->gridData.boundaries[0];

	check(west.name == std::string("West"));

	auto lines = west.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 2);
	checkEqual(lines[0][0], 6); checkEqual(lines[0][1], 3);
	checkEqual(lines[1][0], 3); checkEqual(lines[1][1], 0);

	auto vertices = west.vertices;
	checkEqual(static_cast<int>(vertices.size()), 3);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 3); 
	checkEqual(vertices[2], 6);
}

TestCase(East) {
	BoundaryData east = this->gridData.boundaries[1];

	check(east.name == std::string("East"));

	auto lines = east.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 2);
	checkEqual(lines[0][0], 2); checkEqual(lines[0][1], 5);
	checkEqual(lines[1][0], 5); checkEqual(lines[1][1], 8);
	
	auto vertices = east.vertices;
	checkEqual(static_cast<int>(vertices.size()), 3);
	checkEqual(vertices[0], 2);
	checkEqual(vertices[1], 5); 
	checkEqual(vertices[2], 8);
}

TestCase(South) {
	BoundaryData south = this->gridData.boundaries[2];

	check(south.name == std::string("South"));

	auto lines = south.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 2);
	checkEqual(lines[0][0], 0); checkEqual(lines[0][1], 1);
	checkEqual(lines[1][0], 1); checkEqual(lines[1][1], 2);
	
	auto vertices = south.vertices;
	checkEqual(static_cast<int>(vertices.size()), 3);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 1); 
	checkEqual(vertices[2], 2);
}

TestCase(North) {
	BoundaryData north = this->gridData.boundaries[3];

	check(north.name == std::string("North"));

	auto lines = north.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 2);
	checkEqual(lines[0][0], 8); checkEqual(lines[0][1], 7);
	checkEqual(lines[1][0], 7); checkEqual(lines[1][1], 6);

	auto vertices = north.vertices;
	checkEqual(static_cast<int>(vertices.size()), 3);
	checkEqual(vertices[0], 6);
	checkEqual(vertices[1], 7); 
	checkEqual(vertices[2], 8);
}

TestSuiteEnd()