#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <cgnslib.h>

#define TOLERANCE 1e-12

struct Region1_Mixed_3D_Cgns {
	Region1_Mixed_3D_Cgns() {
		CgnsReader3D cgnsReader3D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Mixed/Grid.cgns");
		this->gridData = cgnsReader3D.gridData;
	}

	~Region1_Mixed_3D_Cgns() = default;

	GridDataShared gridData;
};

FixtureTestSuite(ReadCgns_Region1_Mixed_3D, Region1_Mixed_3D_Cgns)

TestCase(A) {
	std::cout << std::endl;
	std::cout << "\tNumber of vertices: " << gridData->coordinates.size() << std::endl;
	std::cout << "\tNumber of elements: " << gridData->tetrahedronConnectivity.size() + gridData->hexahedronConnectivity.size() + gridData->prismConnectivity.size() + gridData->pyramidConnectivity.size() << std::endl;
}

// TestCase(B) {
// 	std::cout << std::endl;
// 	std::cout << std::endl;
// 	std::cout << "ElementTypeNull: " << ElementTypeNull << std::endl;
// 	std::cout << "ElementTypeUserDefined: " << ElementTypeUserDefined << std::endl;
// 	std::cout << "NODE: " << NODE << std::endl;
// 	std::cout << "BAR_2: " << BAR_2 << std::endl;
// 	std::cout << "BAR_3: " << BAR_3 << std::endl;
// 	std::cout << "TRI_3: " << TRI_3 << std::endl;
// 	std::cout << "TRI_6: " << TRI_6 << std::endl;
// 	std::cout << "QUAD_4: " << QUAD_4 << std::endl;
// 	std::cout << "QUAD_8: " << QUAD_8 << std::endl;
// 	std::cout << "QUAD_9: " << QUAD_9 << std::endl;
// 	std::cout << "TETRA_4: " << TETRA_4 << std::endl;
// 	std::cout << "TETRA_10: " << TETRA_10 << std::endl;
// 	std::cout << "PYRA_5: " << PYRA_5 << std::endl;
// 	std::cout << "PYRA_14: " << PYRA_14 << std::endl;
// 	std::cout << "PENTA_6: " << PENTA_6 << std::endl;
// 	std::cout << "PENTA_15: " << PENTA_15 << std::endl;
// 	std::cout << "PENTA_18: " << PENTA_18 << std::endl;
// 	std::cout << "HEXA_8: " << HEXA_8 << std::endl;
// 	std::cout << "HEXA_20: " << HEXA_20 << std::endl;
// 	std::cout << "HEXA_27: " << HEXA_27 << std::endl;
// 	std::cout << "MIXED: " << MIXED << std::endl;
// 	std::cout << "PYRA_13: " << PYRA_13 << std::endl;
// 	std::cout << "NGON_n: " << NGON_n << std::endl;
// 	std::cout << "NFACE_n: " << NFACE_n << std::endl;
// 	std::cout << "BAR_4: " << BAR_4 << std::endl;
// 	std::cout << "TRI_9: " << TRI_9 << std::endl;
// 	std::cout << "TRI_10: " << TRI_10 << std::endl;
// 	std::cout << "QUAD_12: " << QUAD_12 << std::endl;
// 	std::cout << "QUAD_16: " << QUAD_16 << std::endl;
// 	std::cout << "TETRA_16: " << TETRA_16 << std::endl;
// 	std::cout << "TETRA_20: " << TETRA_20 << std::endl;
// 	std::cout << "PYRA_21: " << PYRA_21 << std::endl;
// 	std::cout << "PYRA_29: " << PYRA_29 << std::endl;
// 	std::cout << "PYRA_30: " << PYRA_30 << std::endl;
// 	std::cout << "PENTA_24: " << PENTA_24 << std::endl;
// 	std::cout << "PENTA_38: " << PENTA_38 << std::endl;
// 	std::cout << "PENTA_40: " << PENTA_40 << std::endl;
// 	std::cout << "HEXA_32: " << HEXA_32 << std::endl;
// 	std::cout << "HEXA_56: " << HEXA_56 << std::endl;
// 	std::cout << "HEXA_64: " << HEXA_64 << std::endl;
// 	std::cout << "BAR_5: " << BAR_5 << std::endl;
// 	std::cout << "TRI_12: " << TRI_12 << std::endl;
// 	std::cout << "TRI_15: " << TRI_15 << std::endl;
// 	std::cout << "QUAD_P4_16: " << QUAD_P4_16 << std::endl;
// 	std::cout << "QUAD_25: " << QUAD_25 << std::endl;
// 	std::cout << "TETRA_22: " << TETRA_22 << std::endl;
// 	std::cout << "TETRA_34: " << TETRA_34 << std::endl;
// 	std::cout << "TETRA_35: " << TETRA_35 << std::endl;
// 	std::cout << "PYRA_P4_29: " << PYRA_P4_29 << std::endl;
// 	std::cout << "PYRA_50: " << PYRA_50 << std::endl;
// 	std::cout << "PYRA_55: " << PYRA_55 << std::endl;
// 	std::cout << "PENTA_33: " << PENTA_33 << std::endl;
// 	std::cout << "PENTA_66: " << PENTA_66 << std::endl;
// 	std::cout << "PENTA_75: " << PENTA_75 << std::endl;
// 	std::cout << "HEXA_44: " << HEXA_44 << std::endl;
// 	std::cout << "HEXA_98: " << HEXA_98 << std::endl;
// 	std::cout << "HEXA_125: " << HEXA_125 << std::endl;
// }

// TestCase(Coordinates) {
// 	auto coordinates = this->gridData->coordinates;

// 	checkEqual(coordinates.size(), 27u);
// 	checkClose(coordinates[ 0][0], 0.0, TOLERANCE); checkClose(coordinates[ 0][1], 0.0, TOLERANCE); checkClose(coordinates[ 0][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 1][0], 0.5, TOLERANCE); checkClose(coordinates[ 1][1], 0.0, TOLERANCE); checkClose(coordinates[ 1][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 2][0], 1.0, TOLERANCE); checkClose(coordinates[ 2][1], 0.0, TOLERANCE); checkClose(coordinates[ 2][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 3][0], 0.0, TOLERANCE); checkClose(coordinates[ 3][1], 0.5, TOLERANCE); checkClose(coordinates[ 3][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 4][0], 0.5, TOLERANCE); checkClose(coordinates[ 4][1], 0.5, TOLERANCE); checkClose(coordinates[ 4][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 5][0], 1.0, TOLERANCE); checkClose(coordinates[ 5][1], 0.5, TOLERANCE); checkClose(coordinates[ 5][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 6][0], 0.0, TOLERANCE); checkClose(coordinates[ 6][1], 1.0, TOLERANCE); checkClose(coordinates[ 6][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 7][0], 0.5, TOLERANCE); checkClose(coordinates[ 7][1], 1.0, TOLERANCE); checkClose(coordinates[ 7][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 8][0], 1.0, TOLERANCE); checkClose(coordinates[ 8][1], 1.0, TOLERANCE); checkClose(coordinates[ 8][2], 0.0, TOLERANCE);
// 	checkClose(coordinates[ 9][0], 0.0, TOLERANCE); checkClose(coordinates[ 9][1], 0.0, TOLERANCE); checkClose(coordinates[ 9][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[10][0], 0.5, TOLERANCE); checkClose(coordinates[10][1], 0.0, TOLERANCE); checkClose(coordinates[10][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[11][0], 1.0, TOLERANCE); checkClose(coordinates[11][1], 0.0, TOLERANCE); checkClose(coordinates[11][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[12][0], 0.0, TOLERANCE); checkClose(coordinates[12][1], 0.5, TOLERANCE); checkClose(coordinates[12][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[13][0], 0.5, TOLERANCE); checkClose(coordinates[13][1], 0.5, TOLERANCE); checkClose(coordinates[13][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[14][0], 1.0, TOLERANCE); checkClose(coordinates[14][1], 0.5, TOLERANCE); checkClose(coordinates[14][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[15][0], 0.0, TOLERANCE); checkClose(coordinates[15][1], 1.0, TOLERANCE); checkClose(coordinates[15][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[16][0], 0.5, TOLERANCE); checkClose(coordinates[16][1], 1.0, TOLERANCE); checkClose(coordinates[16][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[17][0], 1.0, TOLERANCE); checkClose(coordinates[17][1], 1.0, TOLERANCE); checkClose(coordinates[17][2], 0.5, TOLERANCE);
// 	checkClose(coordinates[18][0], 0.0, TOLERANCE); checkClose(coordinates[18][1], 0.0, TOLERANCE); checkClose(coordinates[18][2], 1.0, TOLERANCE);
// 	checkClose(coordinates[19][0], 0.5, TOLERANCE); checkClose(coordinates[19][1], 0.0, TOLERANCE); checkClose(coordinates[19][2], 1.0, TOLERANCE);
// 	checkClose(coordinates[20][0], 1.0, TOLERANCE); checkClose(coordinates[20][1], 0.0, TOLERANCE); checkClose(coordinates[20][2], 1.0, TOLERANCE);
// 	checkClose(coordinates[21][0], 0.0, TOLERANCE); checkClose(coordinates[21][1], 0.5, TOLERANCE); checkClose(coordinates[21][2], 1.0, TOLERANCE);
// 	checkClose(coordinates[22][0], 0.5, TOLERANCE); checkClose(coordinates[22][1], 0.5, TOLERANCE); checkClose(coordinates[22][2], 1.0, TOLERANCE);
// 	checkClose(coordinates[23][0], 1.0, TOLERANCE); checkClose(coordinates[23][1], 0.5, TOLERANCE); checkClose(coordinates[23][2], 1.0, TOLERANCE);
// 	checkClose(coordinates[24][0], 0.0, TOLERANCE); checkClose(coordinates[24][1], 1.0, TOLERANCE); checkClose(coordinates[24][2], 1.0, TOLERANCE);
// 	checkClose(coordinates[25][0], 0.5, TOLERANCE); checkClose(coordinates[25][1], 1.0, TOLERANCE); checkClose(coordinates[25][2], 1.0, TOLERANCE);
// 	checkClose(coordinates[26][0], 1.0, TOLERANCE); checkClose(coordinates[26][1], 1.0, TOLERANCE); checkClose(coordinates[26][2], 1.0, TOLERANCE);
// }

// TestCase(Elements) {
// 	auto hexahedra = this->gridData->hexahedronConnectivity;

// 	checkEqual(hexahedra.size(), 8u);

// 	checkEqual(hexahedra[0][0],  0); checkEqual(hexahedra[0][1],  1); checkEqual(hexahedra[0][2],  4); checkEqual(hexahedra[0][3],  3);
// 	checkEqual(hexahedra[1][0],  1); checkEqual(hexahedra[1][1],  2); checkEqual(hexahedra[1][2],  5); checkEqual(hexahedra[1][3],  4);
// 	checkEqual(hexahedra[2][0],  3); checkEqual(hexahedra[2][1],  4); checkEqual(hexahedra[2][2],  7); checkEqual(hexahedra[2][3],  6);
// 	checkEqual(hexahedra[3][0],  4); checkEqual(hexahedra[3][1],  5); checkEqual(hexahedra[3][2],  8); checkEqual(hexahedra[3][3],  7);
// 	checkEqual(hexahedra[4][0],  9); checkEqual(hexahedra[4][1], 10); checkEqual(hexahedra[4][2], 13); checkEqual(hexahedra[4][3], 12);
// 	checkEqual(hexahedra[5][0], 10); checkEqual(hexahedra[5][1], 11); checkEqual(hexahedra[5][2], 14); checkEqual(hexahedra[5][3], 13);
// 	checkEqual(hexahedra[6][0], 12); checkEqual(hexahedra[6][1], 13); checkEqual(hexahedra[6][2], 16); checkEqual(hexahedra[6][3], 15);
// 	checkEqual(hexahedra[7][0], 13); checkEqual(hexahedra[7][1], 14); checkEqual(hexahedra[7][2], 17); checkEqual(hexahedra[7][3], 16);

// 	checkEqual(hexahedra[0][4],  9); checkEqual(hexahedra[0][5], 10); checkEqual(hexahedra[0][6], 13); checkEqual(hexahedra[0][7], 12);
// 	checkEqual(hexahedra[1][4], 10); checkEqual(hexahedra[1][5], 11); checkEqual(hexahedra[1][6], 14); checkEqual(hexahedra[1][7], 13);
// 	checkEqual(hexahedra[2][4], 12); checkEqual(hexahedra[2][5], 13); checkEqual(hexahedra[2][6], 16); checkEqual(hexahedra[2][7], 15);
// 	checkEqual(hexahedra[3][4], 13); checkEqual(hexahedra[3][5], 14); checkEqual(hexahedra[3][6], 17); checkEqual(hexahedra[3][7], 16);
// 	checkEqual(hexahedra[4][4], 18); checkEqual(hexahedra[4][5], 19); checkEqual(hexahedra[4][6], 22); checkEqual(hexahedra[4][7], 21);
// 	checkEqual(hexahedra[5][4], 19); checkEqual(hexahedra[5][5], 20); checkEqual(hexahedra[5][6], 23); checkEqual(hexahedra[5][7], 22);
// 	checkEqual(hexahedra[6][4], 21); checkEqual(hexahedra[6][5], 22); checkEqual(hexahedra[6][6], 25); checkEqual(hexahedra[6][7], 24);
// 	checkEqual(hexahedra[7][4], 22); checkEqual(hexahedra[7][5], 23); checkEqual(hexahedra[7][6], 26); checkEqual(hexahedra[7][7], 25);

// 	checkEqual(hexahedra[0][8], 0);
// 	checkEqual(hexahedra[1][8], 1);
// 	checkEqual(hexahedra[2][8], 2);
// 	checkEqual(hexahedra[3][8], 3);
// 	checkEqual(hexahedra[4][8], 4);
// 	checkEqual(hexahedra[5][8], 5);
// 	checkEqual(hexahedra[6][8], 6);
// 	checkEqual(hexahedra[7][8], 7);
// }

// TestCase(Facets) {
// 	auto quadrangles = this->gridData->quadrangleConnectivity;

// 	checkEqual(quadrangles[	0][0],  0); checkEqual(quadrangles[	0][1],  9); checkEqual(quadrangles[	0][2], 12); checkEqual(quadrangles[	0][3],  3); checkEqual(quadrangles[	0][4],  8);
// 	checkEqual(quadrangles[	1][0],  3); checkEqual(quadrangles[	1][1], 12); checkEqual(quadrangles[	1][2], 15); checkEqual(quadrangles[	1][3],  6); checkEqual(quadrangles[	1][4],  9);
// 	checkEqual(quadrangles[	2][0],  9); checkEqual(quadrangles[	2][1], 18); checkEqual(quadrangles[	2][2], 21); checkEqual(quadrangles[	2][3], 12); checkEqual(quadrangles[	2][4], 10);
// 	checkEqual(quadrangles[	3][0], 12); checkEqual(quadrangles[	3][1], 21); checkEqual(quadrangles[	3][2], 24); checkEqual(quadrangles[	3][3], 15); checkEqual(quadrangles[	3][4], 11);
// 	checkEqual(quadrangles[	4][0],  2); checkEqual(quadrangles[	4][1],  5); checkEqual(quadrangles[	4][2], 14); checkEqual(quadrangles[	4][3], 11); checkEqual(quadrangles[	4][4], 12);
// 	checkEqual(quadrangles[	5][0],  5); checkEqual(quadrangles[	5][1],  8); checkEqual(quadrangles[	5][2], 17); checkEqual(quadrangles[	5][3], 14); checkEqual(quadrangles[	5][4], 13);
// 	checkEqual(quadrangles[	6][0], 11); checkEqual(quadrangles[	6][1], 14); checkEqual(quadrangles[	6][2], 23); checkEqual(quadrangles[	6][3], 20); checkEqual(quadrangles[	6][4], 14);
// 	checkEqual(quadrangles[	7][0], 14); checkEqual(quadrangles[	7][1], 17); checkEqual(quadrangles[	7][2], 26); checkEqual(quadrangles[	7][3], 23); checkEqual(quadrangles[	7][4], 15);
// 	checkEqual(quadrangles[	8][0],  0); checkEqual(quadrangles[	8][1],  1); checkEqual(quadrangles[	8][2], 10); checkEqual(quadrangles[	8][3],  9); checkEqual(quadrangles[	8][4], 16);
// 	checkEqual(quadrangles[	9][0],  1); checkEqual(quadrangles[	9][1],  2); checkEqual(quadrangles[	9][2], 11); checkEqual(quadrangles[	9][3], 10); checkEqual(quadrangles[	9][4], 17);
// 	checkEqual(quadrangles[10][0],  9); checkEqual(quadrangles[10][1], 10); checkEqual(quadrangles[10][2], 19); checkEqual(quadrangles[10][3], 18); checkEqual(quadrangles[10][4], 18);
// 	checkEqual(quadrangles[11][0], 10); checkEqual(quadrangles[11][1], 11); checkEqual(quadrangles[11][2], 20); checkEqual(quadrangles[11][3], 19); checkEqual(quadrangles[11][4], 19);
// 	checkEqual(quadrangles[12][0],  7); checkEqual(quadrangles[12][1],  6); checkEqual(quadrangles[12][2], 15); checkEqual(quadrangles[12][3], 16); checkEqual(quadrangles[12][4], 20);
// 	checkEqual(quadrangles[13][0],  8); checkEqual(quadrangles[13][1],  7); checkEqual(quadrangles[13][2], 16); checkEqual(quadrangles[13][3], 17); checkEqual(quadrangles[13][4], 21);
// 	checkEqual(quadrangles[14][0], 16); checkEqual(quadrangles[14][1], 15); checkEqual(quadrangles[14][2], 24); checkEqual(quadrangles[14][3], 25); checkEqual(quadrangles[14][4], 22);
// 	checkEqual(quadrangles[15][0], 17); checkEqual(quadrangles[15][1], 16); checkEqual(quadrangles[15][2], 25); checkEqual(quadrangles[15][3], 26); checkEqual(quadrangles[15][4], 23);
// 	checkEqual(quadrangles[16][0],  1); checkEqual(quadrangles[16][1],  0); checkEqual(quadrangles[16][2],  3); checkEqual(quadrangles[16][3],  4); checkEqual(quadrangles[16][4], 24);
// 	checkEqual(quadrangles[17][0],  2); checkEqual(quadrangles[17][1],  1); checkEqual(quadrangles[17][2],  4); checkEqual(quadrangles[17][3],  5); checkEqual(quadrangles[17][4], 25);
// 	checkEqual(quadrangles[18][0],  4); checkEqual(quadrangles[18][1],  3); checkEqual(quadrangles[18][2],  6); checkEqual(quadrangles[18][3],  7); checkEqual(quadrangles[18][4], 26);
// 	checkEqual(quadrangles[19][0],  5); checkEqual(quadrangles[19][1],  4); checkEqual(quadrangles[19][2],  7); checkEqual(quadrangles[19][3],  8); checkEqual(quadrangles[19][4], 27);
// 	checkEqual(quadrangles[20][0], 18); checkEqual(quadrangles[20][1], 19); checkEqual(quadrangles[20][2], 22); checkEqual(quadrangles[20][3], 21); checkEqual(quadrangles[20][4], 28);
// 	checkEqual(quadrangles[21][0], 19); checkEqual(quadrangles[21][1], 20); checkEqual(quadrangles[21][2], 23); checkEqual(quadrangles[21][3], 22); checkEqual(quadrangles[21][4], 29);
// 	checkEqual(quadrangles[22][0], 21); checkEqual(quadrangles[22][1], 22); checkEqual(quadrangles[22][2], 25); checkEqual(quadrangles[22][3], 24); checkEqual(quadrangles[22][4], 30);
// 	checkEqual(quadrangles[23][0], 22); checkEqual(quadrangles[23][1], 23); checkEqual(quadrangles[23][2], 26); checkEqual(quadrangles[23][3], 25); checkEqual(quadrangles[23][4], 31);
// }

// TestCase(Regions) {
// 	checkEqual(this->gridData->regions.size(), 1u);
// }

// TestCase(Geometry) {
// 	auto region = this->gridData->regions[0];

// 	check(region.name == std::string("Geometry"));

// 	auto elementsOnRegion = region.elementsOnRegion;
// 	checkEqual(elementsOnRegion.size(), 8u);
// 	checkEqual(elementsOnRegion[0], 0);
// 	checkEqual(elementsOnRegion[1], 1);
// 	checkEqual(elementsOnRegion[2], 2);
// 	checkEqual(elementsOnRegion[3], 3);
// 	checkEqual(elementsOnRegion[4], 4);
// 	checkEqual(elementsOnRegion[5], 5);
// 	checkEqual(elementsOnRegion[6], 6);
// 	checkEqual(elementsOnRegion[7], 7);
// }

// TestCase(Boundaries) {
// 	auto boundaries = this->gridData->boundaries;

// 	checkEqual(boundaries.size(), 6u);
// }

// TestCase(West) {
// 	auto boundary = this->gridData->boundaries[0];

// 	check(boundary.name == std::string("West"));

// 	auto facetsOnBoundary = boundary.facetsOnBoundary;
// 	checkEqual(facetsOnBoundary.size(), 4u);
// 	checkEqual(facetsOnBoundary[0],  8);
// 	checkEqual(facetsOnBoundary[1],  9);
// 	checkEqual(facetsOnBoundary[2], 10);
// 	checkEqual(facetsOnBoundary[3], 11);

// 	auto vertices = boundary.vertices;
// 	checkEqual(vertices.size(), 9u);
// 	checkEqual(vertices[0],  0);
// 	checkEqual(vertices[1],  3);
// 	checkEqual(vertices[2],  6);
// 	checkEqual(vertices[3],  9);
// 	checkEqual(vertices[4], 12);
// 	checkEqual(vertices[5], 15);
// 	checkEqual(vertices[6], 18);
// 	checkEqual(vertices[7], 21);
// 	checkEqual(vertices[8], 24);
// }

// TestCase(East) {
// 	auto boundary = this->gridData->boundaries[1];

// 	check(boundary.name == std::string("East"));

// 	auto facetsOnBoundary = boundary.facetsOnBoundary;
// 	checkEqual(facetsOnBoundary.size(), 4u);
// 	checkEqual(facetsOnBoundary[0], 12);
// 	checkEqual(facetsOnBoundary[1], 13);
// 	checkEqual(facetsOnBoundary[2], 14);
// 	checkEqual(facetsOnBoundary[3], 15);

// 	auto vertices = boundary.vertices;
// 	checkEqual(vertices.size(), 9u);
// 	checkEqual(vertices[0],  2);
// 	checkEqual(vertices[1],  5);
// 	checkEqual(vertices[2],  8);
// 	checkEqual(vertices[3], 11);
// 	checkEqual(vertices[4], 14);
// 	checkEqual(vertices[5], 17);
// 	checkEqual(vertices[6], 20);
// 	checkEqual(vertices[7], 23);
// 	checkEqual(vertices[8], 26);
// }

// TestCase(South) {
// 	auto boundary = this->gridData->boundaries[2];

// 	check(boundary.name == std::string("South"));

// 	auto facetsOnBoundary = boundary.facetsOnBoundary;
// 	checkEqual(facetsOnBoundary.size(), 4u);
// 	checkEqual(facetsOnBoundary[0], 16);
// 	checkEqual(facetsOnBoundary[1], 17);
// 	checkEqual(facetsOnBoundary[2], 18);
// 	checkEqual(facetsOnBoundary[3], 19);

// 	auto vertices = boundary.vertices;
// 	checkEqual(vertices.size(), 9u);
// 	checkEqual(vertices[0],  0);
// 	checkEqual(vertices[1],  1);
// 	checkEqual(vertices[2],  2);
// 	checkEqual(vertices[3],  9);
// 	checkEqual(vertices[4], 10);
// 	checkEqual(vertices[5], 11);
// 	checkEqual(vertices[6], 18);
// 	checkEqual(vertices[7], 19);
// 	checkEqual(vertices[8], 20);
// }

// TestCase(North) {
// 	auto boundary = this->gridData->boundaries[3];

// 	check(boundary.name == std::string("North"));

// 	auto facetsOnBoundary = boundary.facetsOnBoundary;
// 	checkEqual(facetsOnBoundary.size(), 4u);
// 	checkEqual(facetsOnBoundary[0], 20);
// 	checkEqual(facetsOnBoundary[1], 21);
// 	checkEqual(facetsOnBoundary[2], 22);
// 	checkEqual(facetsOnBoundary[3], 23);

// 	auto vertices = boundary.vertices;
// 	checkEqual(vertices.size(), 9u);
// 	checkEqual(vertices[0],  6);
// 	checkEqual(vertices[1],  7);
// 	checkEqual(vertices[2],  8);
// 	checkEqual(vertices[3], 15);
// 	checkEqual(vertices[4], 16);
// 	checkEqual(vertices[5], 17);
// 	checkEqual(vertices[6], 24);
// 	checkEqual(vertices[7], 25);
// 	checkEqual(vertices[8], 26);
// }

// TestCase(Bottom) {
// 	auto boundary = this->gridData->boundaries[4];

// 	check(boundary.name == std::string("Bottom"));

// 	auto facetsOnBoundary = boundary.facetsOnBoundary;
// 	checkEqual(facetsOnBoundary.size(), 4u);
// 	checkEqual(facetsOnBoundary[0], 24);
// 	checkEqual(facetsOnBoundary[1], 25);
// 	checkEqual(facetsOnBoundary[2], 26);
// 	checkEqual(facetsOnBoundary[3], 27);

// 	auto vertices = boundary.vertices;
// 	checkEqual(vertices.size(), 9u);
// 	checkEqual(vertices[0], 0);
// 	checkEqual(vertices[1], 1);
// 	checkEqual(vertices[2], 2);
// 	checkEqual(vertices[3], 3);
// 	checkEqual(vertices[4], 4);
// 	checkEqual(vertices[5], 5);
// 	checkEqual(vertices[6], 6);
// 	checkEqual(vertices[7], 7);
// 	checkEqual(vertices[8], 8);
// }

// TestCase(Top) {
// 	auto boundary = this->gridData->boundaries[5];

// 	check(boundary.name == std::string("Top"));

// 	auto facetsOnBoundary = boundary.facetsOnBoundary;
// 	checkEqual(facetsOnBoundary.size(), 4u);
// 	checkEqual(facetsOnBoundary[0], 28);
// 	checkEqual(facetsOnBoundary[1], 29);
// 	checkEqual(facetsOnBoundary[2], 30);
// 	checkEqual(facetsOnBoundary[3], 31);

// 	auto vertices = boundary.vertices;
// 	checkEqual(vertices.size(), 9u);
// 	checkEqual(vertices[0], 18);
// 	checkEqual(vertices[1], 19);
// 	checkEqual(vertices[2], 20);
// 	checkEqual(vertices[3], 21);
// 	checkEqual(vertices[4], 22);
// 	checkEqual(vertices[5], 23);
// 	checkEqual(vertices[6], 24);
// 	checkEqual(vertices[7], 25);
// 	checkEqual(vertices[8], 26);
// }

TestSuiteEnd()