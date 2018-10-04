#include <BoostInterface/Test.hpp>
#include <MshInterface/MshReader/MshReader2D.hpp>

#define TOLERANCE 1e-12

struct Region1_ElementType1_2D_Msh {
	Region1_ElementType1_2D_Msh() {
		MshReader2D mshReader2D(std::string(TEST_INPUT_DIRECTORY) + "MshInterface/2D-Region1-ElementType1/5v_4e.msh");
		this->gridData = mshReader2D.gridData;
	}

	~Region1_ElementType1_2D_Msh() = default;

	GridDataShared gridData;
};

FixtureTestSuite(ReadMsh_Region1_ElementType1_2D, Region1_ElementType1_2D_Msh)

TestCase(Coordinates) {
	auto coordinates = this->gridData->coordinates;

	checkEqual(coordinates.size(), 5u);
	checkClose(coordinates[0][0], 0.0, TOLERANCE); checkClose(coordinates[0][1], 0.0, TOLERANCE); checkClose(coordinates[0][2], 0.0, TOLERANCE);
	checkClose(coordinates[1][0], 1.0, TOLERANCE); checkClose(coordinates[1][1], 0.0, TOLERANCE); checkClose(coordinates[1][2], 0.0, TOLERANCE);
	checkClose(coordinates[2][0], 1.0, TOLERANCE); checkClose(coordinates[2][1], 1.0, TOLERANCE); checkClose(coordinates[2][2], 0.0, TOLERANCE);
	checkClose(coordinates[3][0], 0.0, TOLERANCE); checkClose(coordinates[3][1], 1.0, TOLERANCE); checkClose(coordinates[3][2], 0.0, TOLERANCE);
	checkClose(coordinates[4][0], 0.5, TOLERANCE); checkClose(coordinates[4][1], 0.5, TOLERANCE); checkClose(coordinates[4][2], 0.0, TOLERANCE);
}

TestCase(Elements) {
	auto triangles = this->gridData->triangleConnectivity;

	checkEqual(triangles.size(), 4u);
	checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 1); checkEqual(triangles[0][2], 4); checkEqual(triangles[0][3], 0);
	checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1], 4); checkEqual(triangles[1][2], 3); checkEqual(triangles[1][3], 1);
	checkEqual(triangles[2][0], 1); checkEqual(triangles[2][1], 2); checkEqual(triangles[2][2], 4); checkEqual(triangles[2][3], 2);
	checkEqual(triangles[3][0], 2); checkEqual(triangles[3][1], 3); checkEqual(triangles[3][2], 4); checkEqual(triangles[3][3], 3);
}

TestCase(Facets) {
	auto lines = this->gridData->lineConnectivity;

	checkEqual(lines.size(), 4u);
	checkEqual(lines[0][0], 3); checkEqual(lines[0][1], 0); checkEqual(lines[0][2], 4);
	checkEqual(lines[1][0], 1); checkEqual(lines[1][1], 2); checkEqual(lines[1][2], 5);
	checkEqual(lines[2][0], 0); checkEqual(lines[2][1], 1); checkEqual(lines[2][2], 6);
	checkEqual(lines[3][0], 2); checkEqual(lines[3][1], 3); checkEqual(lines[3][2], 7);
}

TestCase(Regions) {
	checkEqual(this->gridData->regions.size(), 1u);
}

TestCase(Geometry) {
	auto region = this->gridData->regions[0];

	check(region.name == std::string("Geometry"));

	checkEqual(region.elementBegin, 0);
	checkEqual(region.elementEnd, 4);
}

TestCase(Boundaries) {
	checkEqual(this->gridData->boundaries.size(), 4u);
}

TestCase(West) {
	auto boundary = this->gridData->boundaries[0];

	check(boundary.name == std::string("West"));

	checkEqual(boundary.facetBegin, 4);
	checkEqual(boundary.facetEnd, 5);

	auto vertices = boundary.vertices;
	checkEqual(vertices.size(), 2u);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 3);
}

TestCase(East) {
	auto boundary = this->gridData->boundaries[1];

	check(boundary.name == std::string("East"));

	checkEqual(boundary.facetBegin, 5);
	checkEqual(boundary.facetEnd, 6);

	auto vertices = boundary.vertices;
	checkEqual(vertices.size(), 2u);
	checkEqual(vertices[0], 1);
	checkEqual(vertices[1], 2);
}

TestCase(South) {
	auto boundary = this->gridData->boundaries[2];

	check(boundary.name == std::string("South"));

	checkEqual(boundary.facetBegin, 6);
	checkEqual(boundary.facetEnd, 7);

	auto vertices = boundary.vertices;
	checkEqual(vertices.size(), 2u);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 1);
}

TestCase(North) {
	auto boundary = this->gridData->boundaries[3];

	check(boundary.name == std::string("North"));

	checkEqual(boundary.facetBegin, 7);
	checkEqual(boundary.facetEnd, 8);

	auto vertices = boundary.vertices;
	checkEqual(vertices.size(), 2u);
	checkEqual(vertices[0], 2);
	checkEqual(vertices[1], 3);
}

TestSuiteEnd()