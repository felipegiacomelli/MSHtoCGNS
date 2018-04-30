#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader2D.hpp>

#define TOLERANCE 1e-12

struct Region1_ElementType2_2D_Cgns {
	Region1_ElementType2_2D_Cgns() {
		CgnsReader2D cgnsReader2D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/2D-Region1-ElementType2/11v_10e.cgns");
		this->gridData = cgnsReader2D.gridData;
	}

	~Region1_ElementType2_2D_Cgns() = default;

	GridDataShared gridData;
};

FixtureTestSuite(ReadCgns_Region1_ElementType2_2D, Region1_ElementType2_2D_Cgns)

TestCase(Coordinates) {
	auto coordinates = this->gridData->coordinates;

	checkEqual(static_cast<int>(coordinates.size()), 11);
	checkClose(coordinates[	0][0], 0.00, TOLERANCE); checkClose(coordinates[ 0][1], 0.00, TOLERANCE); checkClose(coordinates[ 0][2], 0.00, TOLERANCE);
	checkClose(coordinates[	1][0], 0.50, TOLERANCE); checkClose(coordinates[ 1][1], 0.00, TOLERANCE); checkClose(coordinates[ 1][2], 0.00, TOLERANCE);
	checkClose(coordinates[	2][0], 1.00, TOLERANCE); checkClose(coordinates[ 2][1], 0.00, TOLERANCE); checkClose(coordinates[ 2][2], 0.00, TOLERANCE);
	checkClose(coordinates[	3][0], 0.00, TOLERANCE); checkClose(coordinates[ 3][1], 0.50, TOLERANCE); checkClose(coordinates[ 3][2], 0.00, TOLERANCE);
	checkClose(coordinates[	4][0], 0.50, TOLERANCE); checkClose(coordinates[ 4][1], 0.50, TOLERANCE); checkClose(coordinates[ 4][2], 0.00, TOLERANCE);
	checkClose(coordinates[	5][0], 1.00, TOLERANCE); checkClose(coordinates[ 5][1], 0.50, TOLERANCE); checkClose(coordinates[ 5][2], 0.00, TOLERANCE);
	checkClose(coordinates[	6][0], 0.00, TOLERANCE); checkClose(coordinates[ 6][1], 1.00, TOLERANCE); checkClose(coordinates[ 6][2], 0.00, TOLERANCE);
	checkClose(coordinates[	7][0], 0.50, TOLERANCE); checkClose(coordinates[ 7][1], 1.00, TOLERANCE); checkClose(coordinates[ 7][2], 0.00, TOLERANCE);
	checkClose(coordinates[	8][0], 1.00, TOLERANCE); checkClose(coordinates[ 8][1], 1.00, TOLERANCE); checkClose(coordinates[ 8][2], 0.00, TOLERANCE);
	checkClose(coordinates[	9][0], 0.25, TOLERANCE); checkClose(coordinates[ 9][1], 0.75, TOLERANCE); checkClose(coordinates[ 9][2], 0.00, TOLERANCE);
	checkClose(coordinates[10][0], 0.75, TOLERANCE); checkClose(coordinates[10][1], 0.25, TOLERANCE); checkClose(coordinates[10][2], 0.00, TOLERANCE);
}

TestCase(Elements) {
	auto triangles = this->gridData->triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 8);
	checkEqual(triangles[0][0], 3); checkEqual(triangles[0][1],  4); checkEqual(triangles[0][2],  9); checkEqual(triangles[0][3], 0);
	checkEqual(triangles[1][0], 3); checkEqual(triangles[1][1],  9); checkEqual(triangles[1][2],  6); checkEqual(triangles[1][3], 1);
	checkEqual(triangles[2][0], 4); checkEqual(triangles[2][1],  7); checkEqual(triangles[2][2],  9); checkEqual(triangles[2][3], 2);
	checkEqual(triangles[3][0], 6); checkEqual(triangles[3][1],  9); checkEqual(triangles[3][2],  7); checkEqual(triangles[3][3], 3);
	checkEqual(triangles[4][0], 1); checkEqual(triangles[4][1],  2); checkEqual(triangles[4][2], 10); checkEqual(triangles[4][3], 4);
	checkEqual(triangles[5][0], 1); checkEqual(triangles[5][1], 10); checkEqual(triangles[5][2],  4); checkEqual(triangles[5][3], 5);
	checkEqual(triangles[6][0], 2); checkEqual(triangles[6][1],  5); checkEqual(triangles[6][2], 10); checkEqual(triangles[6][3], 6);
	checkEqual(triangles[7][0], 4); checkEqual(triangles[7][1], 10); checkEqual(triangles[7][2],  5); checkEqual(triangles[7][3], 7);

	auto quadrangles = this->gridData->quadrangleConnectivity;
	checkEqual(static_cast<int>(quadrangles.size()), 2);
	checkEqual(quadrangles[0][0], 4); checkEqual(quadrangles[0][1], 5); checkEqual(quadrangles[0][2], 8); checkEqual(quadrangles[0][3], 7); checkEqual(quadrangles[0][4], 8);
	checkEqual(quadrangles[1][0], 0); checkEqual(quadrangles[1][1], 1); checkEqual(quadrangles[1][2], 4); checkEqual(quadrangles[1][3], 3); checkEqual(quadrangles[1][4], 9);
} 

TestCase(Boundaries) {
	checkEqual(static_cast<int>(this->gridData->boundaries.size()), 4);
}

TestCase(West) {
	BoundaryData west = this->gridData->boundaries[0];

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
	BoundaryData east = this->gridData->boundaries[1];

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
	BoundaryData south = this->gridData->boundaries[2];

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
	BoundaryData north = this->gridData->boundaries[3];

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

TestCase(Regions) {
	checkEqual(static_cast<int>(this->gridData->regions.size()), 1);

	check(this->gridData->regions[0].name == std::string("Geometry"));

	auto elementsOnRegion = this->gridData->regions[0].elementsOnRegion;
	checkEqual(static_cast<int>(elementsOnRegion.size()), 10);
	checkEqual(elementsOnRegion[0], 0);
	checkEqual(elementsOnRegion[1], 1);
	checkEqual(elementsOnRegion[2], 2);
	checkEqual(elementsOnRegion[3], 3);	
	checkEqual(elementsOnRegion[4], 4);	
	checkEqual(elementsOnRegion[5], 5);	
	checkEqual(elementsOnRegion[6], 6);	
	checkEqual(elementsOnRegion[7], 7);	
	checkEqual(elementsOnRegion[8], 8);	
	checkEqual(elementsOnRegion[9], 9);	
}

TestSuiteEnd()