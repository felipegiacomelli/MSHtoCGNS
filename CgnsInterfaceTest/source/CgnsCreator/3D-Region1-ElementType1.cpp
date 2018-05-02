#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader3D.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <cgnslib.h>

#define TOLERANCE 1e-12

struct Region1_ElementType1_3D {
	Region1_ElementType1_3D() {
		CgnsReader3D inputReader(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-ElementType1/14v_24e.cgns");
		CgnsCreator3D fileIndex3D(inputReader.gridData, "./");
		this->filePath = fileIndex3D.getFileName();
		CgnsReader3D outputReader(this->filePath);
		this->gridData = outputReader.gridData;
		cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex);
	}

	~Region1_ElementType1_3D() {
		cg_close(this->fileIndex);
		deleteDirectory("./14v_24e/");
	};

	std::string filePath;
	GridDataShared gridData;
	int fileIndex;
	char elementSectionName[100];
	ElementType_t type;
	int elementStart;
	int elementEnd;
	int nbndry;
	int parent_flag;
};

FixtureTestSuite(Generate_Region1_ElementType1_3D, Region1_ElementType1_3D)

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
	checkEqual(tetrahedra[ 0][0], 13); checkEqual(tetrahedra[ 0][1],  9); checkEqual(tetrahedra[ 0][2], 10); checkEqual(tetrahedra[ 0][3], 12);
	checkEqual(tetrahedra[ 1][0], 13); checkEqual(tetrahedra[ 1][1], 11); checkEqual(tetrahedra[ 1][2],  9); checkEqual(tetrahedra[ 1][3], 12);
	checkEqual(tetrahedra[ 2][0],  8); checkEqual(tetrahedra[ 2][1], 13); checkEqual(tetrahedra[ 2][2], 10); checkEqual(tetrahedra[ 2][3], 12);
	checkEqual(tetrahedra[ 3][0], 13); checkEqual(tetrahedra[ 3][1],  8); checkEqual(tetrahedra[ 3][2], 11); checkEqual(tetrahedra[ 3][3], 12);
	checkEqual(tetrahedra[ 4][0],  0); checkEqual(tetrahedra[ 4][1],  3); checkEqual(tetrahedra[ 4][2],  8); checkEqual(tetrahedra[ 4][3], 12);
	checkEqual(tetrahedra[ 5][0],  8); checkEqual(tetrahedra[ 5][1],  7); checkEqual(tetrahedra[ 5][2],  4); checkEqual(tetrahedra[ 5][3], 13);
	checkEqual(tetrahedra[ 6][0],  5); checkEqual(tetrahedra[ 6][1],  9); checkEqual(tetrahedra[ 6][2],  1); checkEqual(tetrahedra[ 6][3], 10);
	checkEqual(tetrahedra[ 7][0],  9); checkEqual(tetrahedra[ 7][1],  2); checkEqual(tetrahedra[ 7][2],  1); checkEqual(tetrahedra[ 7][3], 12);
	checkEqual(tetrahedra[ 8][0],  3); checkEqual(tetrahedra[ 8][1],  7); checkEqual(tetrahedra[ 8][2],  8); checkEqual(tetrahedra[ 8][3], 11);
	checkEqual(tetrahedra[ 9][0], 13); checkEqual(tetrahedra[ 9][1],  7); checkEqual(tetrahedra[ 9][2],  6); checkEqual(tetrahedra[ 9][3], 11);
	checkEqual(tetrahedra[10][0],  5); checkEqual(tetrahedra[10][1], 13); checkEqual(tetrahedra[10][2],  6); checkEqual(tetrahedra[10][3],  9);
	checkEqual(tetrahedra[11][0],  6); checkEqual(tetrahedra[11][1], 11); checkEqual(tetrahedra[11][2],  2); checkEqual(tetrahedra[11][3],  9);
	checkEqual(tetrahedra[12][0], 11); checkEqual(tetrahedra[12][1],  3); checkEqual(tetrahedra[12][2],  2); checkEqual(tetrahedra[12][3], 12);
	checkEqual(tetrahedra[13][0],  5); checkEqual(tetrahedra[13][1],  4); checkEqual(tetrahedra[13][2], 13); checkEqual(tetrahedra[13][3], 10);
	checkEqual(tetrahedra[14][0],  0); checkEqual(tetrahedra[14][1],  8); checkEqual(tetrahedra[14][2],  4); checkEqual(tetrahedra[14][3], 10);
	checkEqual(tetrahedra[15][0],  1); checkEqual(tetrahedra[15][1],  0); checkEqual(tetrahedra[15][2], 10); checkEqual(tetrahedra[15][3], 12);
	checkEqual(tetrahedra[16][0],  8); checkEqual(tetrahedra[16][1],  3); checkEqual(tetrahedra[16][2], 11); checkEqual(tetrahedra[16][3], 12);
	checkEqual(tetrahedra[17][0],  8); checkEqual(tetrahedra[17][1],  7); checkEqual(tetrahedra[17][2], 13); checkEqual(tetrahedra[17][3], 11);
	checkEqual(tetrahedra[18][0],  6); checkEqual(tetrahedra[18][1], 13); checkEqual(tetrahedra[18][2], 11); checkEqual(tetrahedra[18][3],  9);
	checkEqual(tetrahedra[19][0], 11); checkEqual(tetrahedra[19][1],  2); checkEqual(tetrahedra[19][2],  9); checkEqual(tetrahedra[19][3], 12);
	checkEqual(tetrahedra[20][0],  5); checkEqual(tetrahedra[20][1], 13); checkEqual(tetrahedra[20][2],  9); checkEqual(tetrahedra[20][3], 10);
	checkEqual(tetrahedra[21][0],  4); checkEqual(tetrahedra[21][1],  8); checkEqual(tetrahedra[21][2], 13); checkEqual(tetrahedra[21][3], 10);
	checkEqual(tetrahedra[22][0],  9); checkEqual(tetrahedra[22][1],  1); checkEqual(tetrahedra[22][2], 10); checkEqual(tetrahedra[22][3], 12);
	checkEqual(tetrahedra[23][0],  0); checkEqual(tetrahedra[23][1],  8); checkEqual(tetrahedra[23][2], 10); checkEqual(tetrahedra[23][3], 12);
	cg_section_read(this->fileIndex, 1, 1, 1, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 1);
	checkEqual(this->elementEnd  , 24);
}

TestCase(Boundaries) {
	std::vector<BoundaryData> boundaries = this->gridData->boundaries;

	checkEqual(static_cast<int>(boundaries.size()), 6);
}

TestCase(West) {
	BoundaryData west = this->gridData->boundaries[0];

	check(west.name == std::string("West"));

	auto triangles = west.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 8); checkEqual(triangles[0][2], 3);
	checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1], 4); checkEqual(triangles[1][2], 8);
	checkEqual(triangles[2][0], 3); checkEqual(triangles[2][1], 8); checkEqual(triangles[2][2], 7);
	checkEqual(triangles[3][0], 4); checkEqual(triangles[3][1], 7); checkEqual(triangles[3][2], 8);
	cg_section_read(this->fileIndex, 1, 1, 2, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 25);
	checkEqual(this->elementEnd  , 28);

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
	checkEqual(triangles[0][0], 2); checkEqual(triangles[0][1], 9); checkEqual(triangles[0][2], 1);
	checkEqual(triangles[1][0], 9); checkEqual(triangles[1][1], 5); checkEqual(triangles[1][2], 1);
	checkEqual(triangles[2][0], 6); checkEqual(triangles[2][1], 9); checkEqual(triangles[2][2], 2);
	checkEqual(triangles[3][0], 9); checkEqual(triangles[3][1], 6); checkEqual(triangles[3][2], 5);
	cg_section_read(this->fileIndex, 1, 1, 3, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 29);
	checkEqual(this->elementEnd  , 32);

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
	checkEqual(triangles[0][0],  1); checkEqual(triangles[0][1], 10); checkEqual(triangles[0][2], 0);
	checkEqual(triangles[1][0], 10); checkEqual(triangles[1][1],  4); checkEqual(triangles[1][2], 0);
	checkEqual(triangles[2][0],  5); checkEqual(triangles[2][1], 10); checkEqual(triangles[2][2], 1);
	checkEqual(triangles[3][0], 10); checkEqual(triangles[3][1],  5); checkEqual(triangles[3][2], 4);
	cg_section_read(this->fileIndex, 1, 1, 4, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 33);
	checkEqual(this->elementEnd  , 36);

	auto vertices = south.vertices;
	checkEqual(static_cast<int>(vertices.size()), 5);
	checkEqual(vertices[0],  0);
	checkEqual(vertices[1],  1);
	checkEqual(vertices[2],  4);
	checkEqual(vertices[3],  5);
	checkEqual(vertices[4], 10);
}

TestCase(North) {
	BoundaryData north = this->gridData->boundaries[3];

	check(north.name == std::string("North"));

	auto triangles = north.triangleConnectivity;
	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0],  3); checkEqual(triangles[0][1], 11); checkEqual(triangles[0][2], 2);
	checkEqual(triangles[1][0], 11); checkEqual(triangles[1][1],  6); checkEqual(triangles[1][2], 2);
	checkEqual(triangles[2][0],  7); checkEqual(triangles[2][1], 11); checkEqual(triangles[2][2], 3);
	checkEqual(triangles[3][0], 11); checkEqual(triangles[3][1],  7); checkEqual(triangles[3][2], 6);
	cg_section_read(this->fileIndex, 1, 1, 5, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 37);
	checkEqual(this->elementEnd  , 40);

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
	checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 12); checkEqual(triangles[0][2],  1);
	checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1],  3); checkEqual(triangles[1][2], 12);
	checkEqual(triangles[2][0], 1); checkEqual(triangles[2][1], 12); checkEqual(triangles[2][2],  2);
	checkEqual(triangles[3][0], 2); checkEqual(triangles[3][1], 12); checkEqual(triangles[3][2],  3);
	cg_section_read(this->fileIndex, 1, 1, 6, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 41);
	checkEqual(this->elementEnd  , 44);

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
	checkEqual(triangles[0][0],  5); checkEqual(triangles[0][1], 13); checkEqual(triangles[0][2], 4);
	checkEqual(triangles[1][0], 13); checkEqual(triangles[1][1],  7); checkEqual(triangles[1][2], 4);
	checkEqual(triangles[2][0],  6); checkEqual(triangles[2][1], 13); checkEqual(triangles[2][2], 5);
	checkEqual(triangles[3][0],  7); checkEqual(triangles[3][1], 13); checkEqual(triangles[3][2], 6);
	cg_section_read(this->fileIndex, 1, 1, 7, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 45);
	checkEqual(this->elementEnd  , 48);

	auto vertices = top.vertices;
	checkEqual(static_cast<int>(vertices.size()), 5);
	checkEqual(vertices[0],  4);
	checkEqual(vertices[1],  5);
	checkEqual(vertices[2],  6);
	checkEqual(vertices[3],  7);
	checkEqual(vertices[4], 13);
}

TestSuiteEnd()