#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/CgnsReader3D.hpp>

#define TOLERANCE 1e-12

struct Region1_ElementType1_3D_Cgns {
	Region1_ElementType1_3D_Cgns() {
		CgnsReader3D cgnsReader3D("/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/3D/27n_8e.cgns");
		this->gridData = cgnsReader3D.getGridData();
	}

	~Region1_ElementType1_3D_Cgns() = default;

	GridDataShared gridData;
};

FixtureTestSuite(ReadCgns_Region1_ElementType1_3D, Region1_ElementType1_3D_Cgns)

TestCase(Coordinates) {
	auto coordinates = this->gridData->coordinates;

	checkEqual(static_cast<int>(coordinates.size()), 27);
	checkClose(coordinates[ 0][0], 0.0, TOLERANCE); checkClose(coordinates[ 0][1], 0.0, TOLERANCE); checkClose(coordinates[ 0][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 1][0], 0.5, TOLERANCE); checkClose(coordinates[ 1][1], 0.0, TOLERANCE); checkClose(coordinates[ 1][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 2][0], 1.0, TOLERANCE); checkClose(coordinates[ 2][1], 0.0, TOLERANCE); checkClose(coordinates[ 2][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 3][0], 0.0, TOLERANCE); checkClose(coordinates[ 3][1], 0.5, TOLERANCE); checkClose(coordinates[ 3][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 4][0], 0.5, TOLERANCE); checkClose(coordinates[ 4][1], 0.5, TOLERANCE); checkClose(coordinates[ 4][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 5][0], 1.0, TOLERANCE); checkClose(coordinates[ 5][1], 0.5, TOLERANCE); checkClose(coordinates[ 5][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 6][0], 0.0, TOLERANCE); checkClose(coordinates[ 6][1], 1.0, TOLERANCE); checkClose(coordinates[ 6][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 7][0], 0.5, TOLERANCE); checkClose(coordinates[ 7][1], 1.0, TOLERANCE); checkClose(coordinates[ 7][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 8][0], 1.0, TOLERANCE); checkClose(coordinates[ 8][1], 1.0, TOLERANCE); checkClose(coordinates[ 8][2], 0.0, TOLERANCE);
	checkClose(coordinates[ 9][0], 0.0, TOLERANCE); checkClose(coordinates[ 9][1], 0.0, TOLERANCE); checkClose(coordinates[ 9][2], 0.5, TOLERANCE);
	checkClose(coordinates[10][0], 0.5, TOLERANCE); checkClose(coordinates[10][1], 0.0, TOLERANCE); checkClose(coordinates[10][2], 0.5, TOLERANCE);
	checkClose(coordinates[11][0], 1.0, TOLERANCE); checkClose(coordinates[11][1], 0.0, TOLERANCE); checkClose(coordinates[11][2], 0.5, TOLERANCE);
	checkClose(coordinates[12][0], 0.0, TOLERANCE); checkClose(coordinates[12][1], 0.5, TOLERANCE); checkClose(coordinates[12][2], 0.5, TOLERANCE);
	checkClose(coordinates[13][0], 0.5, TOLERANCE); checkClose(coordinates[13][1], 0.5, TOLERANCE); checkClose(coordinates[13][2], 0.5, TOLERANCE);
	checkClose(coordinates[14][0], 1.0, TOLERANCE); checkClose(coordinates[14][1], 0.5, TOLERANCE); checkClose(coordinates[14][2], 0.5, TOLERANCE);
	checkClose(coordinates[15][0], 0.0, TOLERANCE); checkClose(coordinates[15][1], 1.0, TOLERANCE); checkClose(coordinates[15][2], 0.5, TOLERANCE);
	checkClose(coordinates[16][0], 0.5, TOLERANCE); checkClose(coordinates[16][1], 1.0, TOLERANCE); checkClose(coordinates[16][2], 0.5, TOLERANCE);
	checkClose(coordinates[17][0], 1.0, TOLERANCE); checkClose(coordinates[17][1], 1.0, TOLERANCE); checkClose(coordinates[17][2], 0.5, TOLERANCE);
	checkClose(coordinates[18][0], 0.0, TOLERANCE); checkClose(coordinates[18][1], 0.0, TOLERANCE); checkClose(coordinates[18][2], 1.0, TOLERANCE);
	checkClose(coordinates[19][0], 0.5, TOLERANCE); checkClose(coordinates[19][1], 0.0, TOLERANCE); checkClose(coordinates[19][2], 1.0, TOLERANCE);
	checkClose(coordinates[20][0], 1.0, TOLERANCE); checkClose(coordinates[20][1], 0.0, TOLERANCE); checkClose(coordinates[20][2], 1.0, TOLERANCE);
	checkClose(coordinates[21][0], 0.0, TOLERANCE); checkClose(coordinates[21][1], 0.5, TOLERANCE); checkClose(coordinates[21][2], 1.0, TOLERANCE);
	checkClose(coordinates[22][0], 0.5, TOLERANCE); checkClose(coordinates[22][1], 0.5, TOLERANCE); checkClose(coordinates[22][2], 1.0, TOLERANCE);
	checkClose(coordinates[23][0], 1.0, TOLERANCE); checkClose(coordinates[23][1], 0.5, TOLERANCE); checkClose(coordinates[23][2], 1.0, TOLERANCE);
	checkClose(coordinates[24][0], 0.0, TOLERANCE); checkClose(coordinates[24][1], 1.0, TOLERANCE); checkClose(coordinates[24][2], 1.0, TOLERANCE);
	checkClose(coordinates[25][0], 0.5, TOLERANCE); checkClose(coordinates[25][1], 1.0, TOLERANCE); checkClose(coordinates[25][2], 1.0, TOLERANCE);
	checkClose(coordinates[26][0], 1.0, TOLERANCE); checkClose(coordinates[26][1], 1.0, TOLERANCE); checkClose(coordinates[26][2], 1.0, TOLERANCE);
}

TestCase(Elements) {
	auto hexahedra = this->gridData->hexahedronConnectivity;

	checkEqual(static_cast<int>(hexahedra.size()), 8);
	checkEqual(hexahedra[ 0][0],  0); checkEqual(hexahedra[ 0][1],  1); checkEqual(hexahedra[ 0][2],  4); checkEqual(hexahedra[ 0][3],  3); checkEqual(hexahedra[ 0][4],  9); checkEqual(hexahedra[ 0][5], 10); checkEqual(hexahedra[ 0][6], 13); checkEqual(hexahedra[ 0][7], 12);
	checkEqual(hexahedra[ 1][0],  1); checkEqual(hexahedra[ 1][1],  2); checkEqual(hexahedra[ 1][2],  5); checkEqual(hexahedra[ 1][3],  4); checkEqual(hexahedra[ 1][4], 10); checkEqual(hexahedra[ 1][5], 11); checkEqual(hexahedra[ 1][6], 14); checkEqual(hexahedra[ 1][7], 13);
	checkEqual(hexahedra[ 2][0],  3); checkEqual(hexahedra[ 2][1],  4); checkEqual(hexahedra[ 2][2],  7); checkEqual(hexahedra[ 2][3],  6); checkEqual(hexahedra[ 2][4], 12); checkEqual(hexahedra[ 2][5], 13); checkEqual(hexahedra[ 2][6], 16); checkEqual(hexahedra[ 2][7], 15);
	checkEqual(hexahedra[ 3][0],  4); checkEqual(hexahedra[ 3][1],  5); checkEqual(hexahedra[ 3][2],  8); checkEqual(hexahedra[ 3][3],  7); checkEqual(hexahedra[ 3][4], 13); checkEqual(hexahedra[ 3][5], 14); checkEqual(hexahedra[ 3][6], 17); checkEqual(hexahedra[ 3][7], 16);
	checkEqual(hexahedra[ 4][0],  9); checkEqual(hexahedra[ 4][1], 10); checkEqual(hexahedra[ 4][2], 13); checkEqual(hexahedra[ 4][3], 12); checkEqual(hexahedra[ 4][4], 18); checkEqual(hexahedra[ 4][5], 19); checkEqual(hexahedra[ 4][6], 22); checkEqual(hexahedra[ 4][7], 21);
	checkEqual(hexahedra[ 5][0], 10); checkEqual(hexahedra[ 5][1], 11); checkEqual(hexahedra[ 5][2], 14); checkEqual(hexahedra[ 5][3], 13); checkEqual(hexahedra[ 5][4], 19); checkEqual(hexahedra[ 5][5], 20); checkEqual(hexahedra[ 5][6], 23); checkEqual(hexahedra[ 5][7], 22);
	checkEqual(hexahedra[ 6][0], 12); checkEqual(hexahedra[ 6][1], 13); checkEqual(hexahedra[ 6][2], 16); checkEqual(hexahedra[ 6][3], 15); checkEqual(hexahedra[ 6][4], 21); checkEqual(hexahedra[ 6][5], 22); checkEqual(hexahedra[ 6][6], 25); checkEqual(hexahedra[ 6][7], 24);
	checkEqual(hexahedra[ 7][0], 13); checkEqual(hexahedra[ 7][1], 14); checkEqual(hexahedra[ 7][2], 17); checkEqual(hexahedra[ 7][3], 16); checkEqual(hexahedra[ 7][4], 22); checkEqual(hexahedra[ 7][5], 23); checkEqual(hexahedra[ 7][6], 26); checkEqual(hexahedra[ 7][7], 25);
}

TestCase(Boundaries) {
	auto boundaries = this->gridData->boundaries;

	checkEqual(static_cast<int>(boundaries.size()), 6);
}

TestCase(West) {
	BoundaryData west = this->gridData->boundaries[0];

	check(west.name == std::string("West"));

	auto quadrangles = west.quadrangleConnectivity;
	checkEqual(static_cast<int>(quadrangles.size()), 4);
	checkEqual(quadrangles[0][0],  0); checkEqual(quadrangles[0][1],  9); checkEqual(quadrangles[0][2], 12); checkEqual(quadrangles[0][3],  3);
	checkEqual(quadrangles[1][0],  3); checkEqual(quadrangles[1][1], 12); checkEqual(quadrangles[1][2], 15); checkEqual(quadrangles[1][3],  6);
	checkEqual(quadrangles[2][0],  9); checkEqual(quadrangles[2][1], 18); checkEqual(quadrangles[2][2], 21); checkEqual(quadrangles[2][3], 12);
	checkEqual(quadrangles[3][0], 12); checkEqual(quadrangles[3][1], 21); checkEqual(quadrangles[3][2], 24); checkEqual(quadrangles[3][3], 15);

	auto vertices = west.vertices;
	checkEqual(static_cast<int>(vertices.size()), 9);
	checkEqual(vertices[0],  0);
	checkEqual(vertices[1],  3); 
	checkEqual(vertices[2],  6);
	checkEqual(vertices[3],  9);
	checkEqual(vertices[4], 12); 
	checkEqual(vertices[5], 15);
	checkEqual(vertices[6], 18);
	checkEqual(vertices[7], 21); 
	checkEqual(vertices[8], 24);
}

TestCase(East) {
	BoundaryData east = this->gridData->boundaries[1];

	check(east.name == std::string("East"));

	auto quadrangles = east.quadrangleConnectivity;
	checkEqual(static_cast<int>(quadrangles.size()), 4);
	checkEqual(quadrangles[0][0],  2); checkEqual(quadrangles[0][1],  5); checkEqual(quadrangles[0][2], 14); checkEqual(quadrangles[0][3], 11);
	checkEqual(quadrangles[1][0],  5); checkEqual(quadrangles[1][1],  8); checkEqual(quadrangles[1][2], 17); checkEqual(quadrangles[1][3], 14);
	checkEqual(quadrangles[2][0], 11); checkEqual(quadrangles[2][1], 14); checkEqual(quadrangles[2][2], 23); checkEqual(quadrangles[2][3], 20);
	checkEqual(quadrangles[3][0], 14); checkEqual(quadrangles[3][1], 17); checkEqual(quadrangles[3][2], 26); checkEqual(quadrangles[3][3], 23);

	auto vertices = east.vertices;
	checkEqual(static_cast<int>(vertices.size()), 9);
	checkEqual(vertices[0],  2);
	checkEqual(vertices[1],  5); 
	checkEqual(vertices[2],  8);
	checkEqual(vertices[3], 11);
	checkEqual(vertices[4], 14); 
	checkEqual(vertices[5], 17);
	checkEqual(vertices[6], 20);
	checkEqual(vertices[7], 23); 
	checkEqual(vertices[8], 26);
}

TestCase(South) {
	BoundaryData south = this->gridData->boundaries[2];

	check(south.name == std::string("South"));

	auto quadrangles = south.quadrangleConnectivity;
	checkEqual(static_cast<int>(quadrangles.size()), 4);	
	checkEqual(quadrangles[0][0],  0); checkEqual(quadrangles[0][1],  1); checkEqual(quadrangles[0][2], 10); checkEqual(quadrangles[0][3],  9);
	checkEqual(quadrangles[1][0],  1); checkEqual(quadrangles[1][1],  2); checkEqual(quadrangles[1][2], 11); checkEqual(quadrangles[1][3], 10);
	checkEqual(quadrangles[2][0],  9); checkEqual(quadrangles[2][1], 10); checkEqual(quadrangles[2][2], 19); checkEqual(quadrangles[2][3], 18);
	checkEqual(quadrangles[3][0], 10); checkEqual(quadrangles[3][1], 11); checkEqual(quadrangles[3][2], 20); checkEqual(quadrangles[3][3], 19);
	
	auto vertices = south.vertices;
	checkEqual(static_cast<int>(vertices.size()), 9);
	checkEqual(vertices[0],  0);
	checkEqual(vertices[1],  1); 
	checkEqual(vertices[2],  2);
	checkEqual(vertices[3],  9);
	checkEqual(vertices[4], 10); 
	checkEqual(vertices[5], 11);
	checkEqual(vertices[6], 18);
	checkEqual(vertices[7], 19); 
	checkEqual(vertices[8], 20);
} 

TestCase(North) {
	BoundaryData north = this->gridData->boundaries[3];

	check(north.name == std::string("North"));

	auto quadrangles = north.quadrangleConnectivity;
	checkEqual(static_cast<int>(quadrangles.size()), 4);
	checkEqual(quadrangles[0][0],  7); checkEqual(quadrangles[0][1],  6); checkEqual(quadrangles[0][2], 15); checkEqual(quadrangles[0][3], 16);
	checkEqual(quadrangles[1][0],  8); checkEqual(quadrangles[1][1],  7); checkEqual(quadrangles[1][2], 16); checkEqual(quadrangles[1][3], 17);
	checkEqual(quadrangles[2][0], 16); checkEqual(quadrangles[2][1], 15); checkEqual(quadrangles[2][2], 24); checkEqual(quadrangles[2][3], 25);
	checkEqual(quadrangles[3][0], 17); checkEqual(quadrangles[3][1], 16); checkEqual(quadrangles[3][2], 25); checkEqual(quadrangles[3][3], 26);

	auto vertices = north.vertices;
	checkEqual(static_cast<int>(vertices.size()), 9);
	checkEqual(vertices[0],  6);
	checkEqual(vertices[1],  7); 
	checkEqual(vertices[2],  8);
	checkEqual(vertices[3], 15);
	checkEqual(vertices[4], 16); 
	checkEqual(vertices[5], 17);
	checkEqual(vertices[6], 24);
	checkEqual(vertices[7], 25); 
	checkEqual(vertices[8], 26);
}

TestCase(Bottom) {
	BoundaryData bottom = this->gridData->boundaries[4];

	check(bottom.name == std::string("Bottom"));

	auto quadrangles = bottom.quadrangleConnectivity;
	checkEqual(static_cast<int>(quadrangles.size()), 4);
	checkEqual(quadrangles[0][0], 1); checkEqual(quadrangles[0][1], 0); checkEqual(quadrangles[0][2], 3); checkEqual(quadrangles[0][3], 4);
	checkEqual(quadrangles[1][0], 2); checkEqual(quadrangles[1][1], 1); checkEqual(quadrangles[1][2], 4); checkEqual(quadrangles[1][3], 5);
	checkEqual(quadrangles[2][0], 4); checkEqual(quadrangles[2][1], 3); checkEqual(quadrangles[2][2], 6); checkEqual(quadrangles[2][3], 7);
	checkEqual(quadrangles[3][0], 5); checkEqual(quadrangles[3][1], 4); checkEqual(quadrangles[3][2], 7); checkEqual(quadrangles[3][3], 8);
	
	auto vertices = bottom.vertices;
	checkEqual(static_cast<int>(vertices.size()), 9);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 1); 
	checkEqual(vertices[2], 2);
	checkEqual(vertices[3], 3);
	checkEqual(vertices[4], 4); 
	checkEqual(vertices[5], 5);
	checkEqual(vertices[6], 6);
	checkEqual(vertices[7], 7); 
	checkEqual(vertices[8], 8);
}

TestCase(Top) {
	BoundaryData top = this->gridData->boundaries[5];

	check(top.name == std::string("Top"));

	auto quadrangles = top.quadrangleConnectivity;
	checkEqual(static_cast<int>(quadrangles.size()), 4);
	checkEqual(quadrangles[0][0], 18); checkEqual(quadrangles[0][1], 19); checkEqual(quadrangles[0][2], 22); checkEqual(quadrangles[0][3], 21);
	checkEqual(quadrangles[1][0], 19); checkEqual(quadrangles[1][1], 20); checkEqual(quadrangles[1][2], 23); checkEqual(quadrangles[1][3], 22);
	checkEqual(quadrangles[2][0], 21); checkEqual(quadrangles[2][1], 22); checkEqual(quadrangles[2][2], 25); checkEqual(quadrangles[2][3], 24);
	checkEqual(quadrangles[3][0], 22); checkEqual(quadrangles[3][1], 23); checkEqual(quadrangles[3][2], 26); checkEqual(quadrangles[3][3], 25);

	auto vertices = top.vertices;
	checkEqual(static_cast<int>(vertices.size()), 9);
	checkEqual(vertices[0], 18);
	checkEqual(vertices[1], 19); 
	checkEqual(vertices[2], 20);
	checkEqual(vertices[3], 21);
	checkEqual(vertices[4], 22); 
	checkEqual(vertices[5], 23);
	checkEqual(vertices[6], 24);
	checkEqual(vertices[7], 25); 
	checkEqual(vertices[8], 26);
}

TestSuiteEnd()
