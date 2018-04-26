#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader2D.hpp>
#include <IO/CgnsReader2D.hpp>
#include <CgnsInterface/CgnsFile2D.hpp>

#define TOLERANCE 1e-12

struct Region4_ElementType1_2D {
	Region4_ElementType1_2D() {
		CgnsReader2D inputReader("/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/2D/11v_10e.cgns");
		CgnsFile2D fileIndex2D(inputReader.getGridData(), "./");
		this->filePath = fileIndex2D.getFileName();
		CgnsReader2D outputReader(this->filePath);
		this->gridData = outputReader.getGridData();
		cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex);
	}

	~Region4_ElementType1_2D() {
		cg_close(this->fileIndex);
		deleteDirectory("./11v_10e/");
	};

	std::string filePath;
	GridDataShared gridData;
	int fileIndex;
	char elementSectionName[100];
	ElementType_t type;
	cgsize_t elementStart;
	cgsize_t elementEnd;
	int nbndry;
	int parent_flag;
};

FixtureTestSuite(Generate_Region4_ElementType1_2D, Region4_ElementType1_2D)

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
	checkEqual(triangles[0][0], 3); checkEqual(triangles[0][1],  4); checkEqual(triangles[0][2],  9);
	checkEqual(triangles[1][0], 3); checkEqual(triangles[1][1],  9); checkEqual(triangles[1][2],  6);
	checkEqual(triangles[2][0], 4); checkEqual(triangles[2][1],  7); checkEqual(triangles[2][2],  9);
	checkEqual(triangles[3][0], 6); checkEqual(triangles[3][1],  9); checkEqual(triangles[3][2],  7);
	checkEqual(triangles[4][0], 1); checkEqual(triangles[4][1],  2); checkEqual(triangles[4][2], 10);
	checkEqual(triangles[5][0], 1); checkEqual(triangles[5][1], 10); checkEqual(triangles[5][2],  4);
	checkEqual(triangles[6][0], 2); checkEqual(triangles[6][1],  5); checkEqual(triangles[6][2], 10);
	checkEqual(triangles[7][0], 4); checkEqual(triangles[7][1], 10); checkEqual(triangles[7][2],  5);

	auto quadrangles = this->gridData->quadrangleConnectivity;
	checkEqual(static_cast<int>(quadrangles.size()), 2);
	checkEqual(quadrangles[0][0], 4); checkEqual(quadrangles[0][1], 5); checkEqual(quadrangles[0][2], 8); checkEqual(quadrangles[0][3], 7);
	checkEqual(quadrangles[1][0], 0); checkEqual(quadrangles[1][1], 1); checkEqual(quadrangles[1][2], 4); checkEqual(quadrangles[1][3], 3);
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

	cg_section_read(this->fileIndex, 1, 1, 5, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 11);
	checkEqual(this->elementEnd  , 12); 
	check(this->type == BAR_2);
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

	cg_section_read(this->fileIndex, 1, 1, 6, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 13);
	checkEqual(this->elementEnd  , 14);
	check(this->type == BAR_2);
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

	cg_section_read(this->fileIndex, 1, 1, 7, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 15);
	checkEqual(this->elementEnd  , 16);
	check(this->type == BAR_2);
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
	
	cg_section_read(this->fileIndex, 1, 1, 8, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 17);
	checkEqual(this->elementEnd  , 18);
	check(this->type == BAR_2);
}

TestCase(Regions) {
	checkEqual(static_cast<int>(this->gridData->regions.size()), 4);
}

TestCase(A) {
	auto a = this->gridData->regions[0];

	check(a.name == std::string("A"));
	
	checkEqual(static_cast<int>(a.elementsOnRegion.size()), 4);
	checkEqual(a.elementsOnRegion[0], 0);
	checkEqual(a.elementsOnRegion[1], 1);
	checkEqual(a.elementsOnRegion[2], 2);
	checkEqual(a.elementsOnRegion[3], 3);

	checkEqual(a.elementType, 1);

	cg_section_read(this->fileIndex, 1, 1, 1, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 1);
	checkEqual(this->elementEnd  , 4);
	check(this->type == TRI_3);
}

TestCase(B) {
	auto b = this->gridData->regions[1];

	check(b.name == std::string("B"));
	
	checkEqual(static_cast<int>(b.elementsOnRegion.size()), 4);
	checkEqual(b.elementsOnRegion[0], 4);
	checkEqual(b.elementsOnRegion[1], 5);
	checkEqual(b.elementsOnRegion[2], 6);
	checkEqual(b.elementsOnRegion[3], 7);

	checkEqual(b.elementType, 1);

	cg_section_read(this->fileIndex, 1, 1, 2, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 5);
	checkEqual(this->elementEnd  , 8);
	check(this->type == TRI_3);
}

TestCase(C) {
	auto c = this->gridData->regions[2];

	check(c.name == std::string("C"));
	
	checkEqual(static_cast<int>(c.elementsOnRegion.size()), 1);
	checkEqual(c.elementsOnRegion[0], 8);

	checkEqual(c.elementType, 2);
	
	cg_section_read(this->fileIndex, 1, 1, 3, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 9);
	checkEqual(this->elementEnd  , 9);
	check(this->type == QUAD_4);
}

TestCase(D) {
	auto d = this->gridData->regions[3];

	check(d.name == std::string("D"));
	
	checkEqual(static_cast<int>(d.elementsOnRegion.size()), 1);
	checkEqual(d.elementsOnRegion[0], 9);

	checkEqual(d.elementType, 2);

	cg_section_read(this->fileIndex, 1, 1, 4, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 10);
	checkEqual(this->elementEnd  , 10);
	check(this->type == QUAD_4);
}

TestSuiteEnd()
