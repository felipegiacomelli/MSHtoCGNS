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

	checkEqual(coordinates.size(), 9u);
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

	checkEqual(quadrangles.size(), 4u);
	checkEqual(quadrangles[0][0], 0); checkEqual(quadrangles[0][1], 1); checkEqual(quadrangles[0][2], 4); checkEqual(quadrangles[0][3], 3); checkEqual(quadrangles[0][4], 0);
	checkEqual(quadrangles[1][0], 1); checkEqual(quadrangles[1][1], 2); checkEqual(quadrangles[1][2], 5); checkEqual(quadrangles[1][3], 4); checkEqual(quadrangles[1][4], 1);
	checkEqual(quadrangles[2][0], 3); checkEqual(quadrangles[2][1], 4); checkEqual(quadrangles[2][2], 7); checkEqual(quadrangles[2][3], 6); checkEqual(quadrangles[2][4], 2);
	checkEqual(quadrangles[3][0], 4); checkEqual(quadrangles[3][1], 5); checkEqual(quadrangles[3][2], 8); checkEqual(quadrangles[3][3], 7); checkEqual(quadrangles[3][4], 3);
}

TestCase(Facets) {
	auto lines = this->gridData->lineConnectivity;

	checkEqual(lines.size(), 8u);
	checkEqual(lines[0][0], 6); checkEqual(lines[0][1], 3); checkEqual(lines[0][2],  4);
	checkEqual(lines[1][0], 3); checkEqual(lines[1][1], 0); checkEqual(lines[1][2],  5);
	checkEqual(lines[2][0], 2); checkEqual(lines[2][1], 5); checkEqual(lines[2][2],  6);
	checkEqual(lines[3][0], 5); checkEqual(lines[3][1], 8); checkEqual(lines[3][2],  7);
	checkEqual(lines[4][0], 0); checkEqual(lines[4][1], 1); checkEqual(lines[4][2],  8);
	checkEqual(lines[5][0], 1); checkEqual(lines[5][1], 2); checkEqual(lines[5][2],  9);
	checkEqual(lines[6][0], 8); checkEqual(lines[6][1], 7); checkEqual(lines[6][2], 10);
	checkEqual(lines[7][0], 7); checkEqual(lines[7][1], 6); checkEqual(lines[7][2], 11);
}

TestCase(Regions) {
	checkEqual(this->gridData->regions.size(), 1u);
}

TestCase(Geometry) {
	auto region = this->gridData->regions[0];

	check(region.name == std::string("Geometry"));

	auto elementsOnRegion = region.elementsOnRegion;
	checkEqual(elementsOnRegion.size(), 4u);
	checkEqual(elementsOnRegion[0], 0);
	checkEqual(elementsOnRegion[1], 1);
	checkEqual(elementsOnRegion[2], 2);
	checkEqual(elementsOnRegion[3], 3);
}

TestCase(Boundaries) {
	checkEqual(this->gridData->boundaries.size(), 4u);
}

TestCase(West) {
	BoundaryData boundary = this->gridData->boundaries[0];

	check(boundary.name == std::string("West"));

	auto facetsOnBoundary = boundary.facetsOnBoundary;
	checkEqual(facetsOnBoundary.size(), 2u);
	checkEqual(facetsOnBoundary[0], 4);
	checkEqual(facetsOnBoundary[1], 5);

	auto vertices = boundary.vertices;
	checkEqual(vertices.size(), 3u);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 3);
	checkEqual(vertices[2], 6);
}

TestCase(East) {
	BoundaryData boundary = this->gridData->boundaries[1];

	check(boundary.name == std::string("East"));

	auto facetsOnBoundary = boundary.facetsOnBoundary;
	checkEqual(facetsOnBoundary.size(), 2u);
	checkEqual(facetsOnBoundary[0], 6);
	checkEqual(facetsOnBoundary[1], 7);

	auto vertices = boundary.vertices;
	checkEqual(vertices.size(), 3u);
	checkEqual(vertices[0], 2);
	checkEqual(vertices[1], 5);
	checkEqual(vertices[2], 8);
}

TestCase(South) {
	BoundaryData boundary = this->gridData->boundaries[2];

	check(boundary.name == std::string("South"));

	auto facetsOnBoundary = boundary.facetsOnBoundary;
	checkEqual(facetsOnBoundary.size(), 2u);
	checkEqual(facetsOnBoundary[0], 8);
	checkEqual(facetsOnBoundary[1], 9);

	auto vertices = boundary.vertices;
	checkEqual(vertices.size(), 3u);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 1);
	checkEqual(vertices[2], 2);
}

TestCase(North) {
	BoundaryData boundary = this->gridData->boundaries[3];

	check(boundary.name == std::string("North"));

	auto facetsOnBoundary = boundary.facetsOnBoundary;
	checkEqual(facetsOnBoundary.size(), 2u);
	checkEqual(facetsOnBoundary[0], 10);
	checkEqual(facetsOnBoundary[1], 11);

	auto vertices = boundary.vertices;
	checkEqual(vertices.size(), 3u);
	checkEqual(vertices[0], 6);
	checkEqual(vertices[1], 7);
	checkEqual(vertices[2], 8);
}

TestSuiteEnd()