#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader2D.hpp>

#define TOLERANCE 1e-12

struct Region1_ElementType1_2D_Cgns {
	Region1_ElementType1_2D_Cgns() {
		CgnsReader2D cgnsReader2D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/2D-Region1-ElementType1/9v_4e.cgns");
		this->gridData = cgnsReader2D.gridData;
	}

	~Region1_ElementType1_2D_Cgns() = default;

	GridDataShared gridData;
};

FixtureTestSuite(ReadCgns_Region1_ElementType1_2D, Region1_ElementType1_2D_Cgns)

TestCase(Coordinates) {
	auto coordinates = this->gridData->coordinates;

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
	auto quadrangles = this->gridData->quadrangleConnectivity;

	checkEqual(static_cast<int>(quadrangles.size()), 4);
	checkEqual(quadrangles[0][0], 0); checkEqual(quadrangles[0][1], 1); checkEqual(quadrangles[0][2], 4); checkEqual(quadrangles[0][3], 3); checkEqual(quadrangles[0][4], 0);
	checkEqual(quadrangles[1][0], 1); checkEqual(quadrangles[1][1], 2); checkEqual(quadrangles[1][2], 5); checkEqual(quadrangles[1][3], 4); checkEqual(quadrangles[1][4], 1);
	checkEqual(quadrangles[2][0], 3); checkEqual(quadrangles[2][1], 4); checkEqual(quadrangles[2][2], 7); checkEqual(quadrangles[2][3], 6); checkEqual(quadrangles[2][4], 2);
	checkEqual(quadrangles[3][0], 4); checkEqual(quadrangles[3][1], 5); checkEqual(quadrangles[3][2], 8); checkEqual(quadrangles[3][3], 7); checkEqual(quadrangles[3][4], 3);
}

TestCase(Boundaries) {
	auto boundaries = this->gridData->boundaries;

	checkEqual(static_cast<int>(boundaries.size()), 4);
}

TestCase(West) {
	BoundaryData west = this->gridData->boundaries[0];

	check(west.name == std::string("West"));

	auto lines = west.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 2);
	checkEqual(lines[0][0], 6); checkEqual(lines[0][1], 3); checkEqual(lines[0][2], 4);
	checkEqual(lines[1][0], 3); checkEqual(lines[1][1], 0); checkEqual(lines[1][2], 5);

	auto vertices = west.vertices;
	checkEqual(static_cast<int>(vertices.size()), 3);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 3); 
	checkEqual(vertices[2], 6);
}

TestCase(East) {
	BoundaryData east = this->gridData->boundaries[1];

	check(east.name == std::string("East"));

	auto lines = east.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 2);
	checkEqual(lines[0][0], 2); checkEqual(lines[0][1], 5); checkEqual(lines[0][2], 6);
	checkEqual(lines[1][0], 5); checkEqual(lines[1][1], 8); checkEqual(lines[1][2], 7);
	
	auto vertices = east.vertices;
	checkEqual(static_cast<int>(vertices.size()), 3);
	checkEqual(vertices[0], 2);
	checkEqual(vertices[1], 5); 
	checkEqual(vertices[2], 8);
}

TestCase(South) {
	BoundaryData south = this->gridData->boundaries[2];

	check(south.name == std::string("South"));

	auto lines = south.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 2);
	checkEqual(lines[0][0], 0); checkEqual(lines[0][1], 1); checkEqual(lines[0][2], 8);
	checkEqual(lines[1][0], 1); checkEqual(lines[1][1], 2); checkEqual(lines[1][2], 9);
	
	auto vertices = south.vertices;
	checkEqual(static_cast<int>(vertices.size()), 3);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 1); 
	checkEqual(vertices[2], 2);
}

TestCase(North) {
	BoundaryData north = this->gridData->boundaries[3];

	check(north.name == std::string("North"));

	auto lines = north.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 2);
	checkEqual(lines[0][0], 8); checkEqual(lines[0][1], 7); checkEqual(lines[0][2], 10);
	checkEqual(lines[1][0], 7); checkEqual(lines[1][1], 6); checkEqual(lines[1][2], 11);

	auto vertices = north.vertices;
	checkEqual(static_cast<int>(vertices.size()), 3);
	checkEqual(vertices[0], 6);
	checkEqual(vertices[1], 7); 
	checkEqual(vertices[2], 8);
}

TestCase(Regions) {
	checkEqual(static_cast<int>(this->gridData->regions.size()), 1);

	check(this->gridData->regions[0].name == std::string("Geometry"));

	auto elementsOnRegion = this->gridData->regions[0].elementsOnRegion;
	checkEqual(static_cast<int>(elementsOnRegion.size()), 4);
	checkEqual(elementsOnRegion[0], 0);
	checkEqual(elementsOnRegion[1], 1);
	checkEqual(elementsOnRegion[2], 2);
	checkEqual(elementsOnRegion[3], 3);
}

TestSuiteEnd()