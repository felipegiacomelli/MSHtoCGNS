#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader2D.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader2D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsFile2D.hpp>

#define TOLERANCE 1e-12

struct Region1_ElementType1_2D {
	Region1_ElementType1_2D() {
		CgnsReader2D inputReader("/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/2D/5v_4e.cgns");
		CgnsFile2D fileIndex2D(inputReader.gridData, "./");
		this->filePath = fileIndex2D.getFileName();
		CgnsReader2D outputReader(this->filePath);
		this->gridData = outputReader.gridData;
		cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex);
	}

	~Region1_ElementType1_2D() {
		cg_close(this->fileIndex);
		deleteDirectory("./5v_4e/");
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

FixtureTestSuite(Generate_Region1_ElementType1_2D, Region1_ElementType1_2D)

TestCase(Coordinates) {
	auto coordinates = this->gridData->coordinates;

	checkEqual(static_cast<int>(coordinates.size()), 5);
	checkClose(coordinates[0][0], 0.0, TOLERANCE); checkClose(coordinates[0][1], 0.0, TOLERANCE); checkClose(coordinates[0][2], 0.0, TOLERANCE);
	checkClose(coordinates[1][0], 1.0, TOLERANCE); checkClose(coordinates[1][1], 0.0, TOLERANCE); checkClose(coordinates[1][2], 0.0, TOLERANCE);
	checkClose(coordinates[2][0], 1.0, TOLERANCE); checkClose(coordinates[2][1], 1.0, TOLERANCE); checkClose(coordinates[2][2], 0.0, TOLERANCE);
	checkClose(coordinates[3][0], 0.0, TOLERANCE); checkClose(coordinates[3][1], 1.0, TOLERANCE); checkClose(coordinates[3][2], 0.0, TOLERANCE);
	checkClose(coordinates[4][0], 0.5, TOLERANCE); checkClose(coordinates[4][1], 0.5, TOLERANCE); checkClose(coordinates[4][2], 0.0, TOLERANCE);
}

TestCase(Elements) {
	auto triangles = this->gridData->triangleConnectivity;

	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 1); checkEqual(triangles[0][2], 4);
	checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1], 4); checkEqual(triangles[1][2], 3);
	checkEqual(triangles[2][0], 1); checkEqual(triangles[2][1], 2); checkEqual(triangles[2][2], 4);
	checkEqual(triangles[3][0], 2); checkEqual(triangles[3][1], 3); checkEqual(triangles[3][2], 4);
	cg_section_read(this->fileIndex, 1, 1, 1, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 1);
	checkEqual(this->elementEnd  , 4);
}

TestCase(Boundaries) {
	auto boundaries = this->gridData->boundaries;

	checkEqual(static_cast<int>(boundaries.size()), 4);
}

TestCase(West) {
	BoundaryData west = this->gridData->boundaries[0];

	check(west.name == std::string("West"));

	auto lines = west.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 3); checkEqual(lines[0][1], 0);
	
	cg_section_read(this->fileIndex, 1, 1, 2, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 5);
	checkEqual(this->elementEnd  , 5);

	auto vertices = west.vertices;
	checkEqual(static_cast<int>(vertices.size()), 2);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 3); 
}

TestCase(East) {
	BoundaryData east = this->gridData->boundaries[1];

	check(east.name == std::string("East"));

	auto lines = east.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 1); checkEqual(lines[0][1], 2);
	cg_section_read(this->fileIndex, 1, 1, 3, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 6);
	checkEqual(this->elementEnd  , 6);

	auto vertices = east.vertices;
	checkEqual(static_cast<int>(vertices.size()), 2);
	checkEqual(vertices[0], 1);
	checkEqual(vertices[1], 2); 
}

TestCase(South) {
	BoundaryData south = this->gridData->boundaries[2];

	check(south.name == std::string("South"));

	auto lines = south.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 0); checkEqual(lines[0][1], 1);
	cg_section_read(this->fileIndex, 1, 1, 4, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 7);
	checkEqual(this->elementEnd  , 7);

	auto vertices = south.vertices;
	checkEqual(static_cast<int>(vertices.size()), 2);
	checkEqual(vertices[0], 0);
	checkEqual(vertices[1], 1); 
}

TestCase(North) {
	BoundaryData north = this->gridData->boundaries[3];

	check(north.name == std::string("North"));

	auto lines = north.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 2); checkEqual(lines[0][1], 3);
	cg_section_read(this->fileIndex, 1, 1, 5, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 8);
	checkEqual(this->elementEnd  , 8);

	auto vertices = north.vertices;
	checkEqual(static_cast<int>(vertices.size()), 2);
	checkEqual(vertices[0], 2);
	checkEqual(vertices[1], 3); 	
}

TestCase(Regions) {
	checkEqual(static_cast<int>(this->gridData->regions.size()), 1);
}

TestCase(Geometry) {
	auto a = this->gridData->regions[0];

	check(a.name == std::string("Geometry"));
	
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

TestSuiteEnd()
