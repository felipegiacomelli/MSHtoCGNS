#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader2D.hpp>
#include <IO/CgnsReader2D.hpp>
#include <CgnsInterface/CgnsFile2D.hpp>

#define TOLERANCE 1e-12

struct Cgns2D {
	Cgns2D() {
		this->filePath = "/home/felipe/Felipe/cpp/MSHtoCGNS/Zeta/TestFiles/2D/5n_4e.cgns";
		CgnsReader2D cgnsReader2D(this->filePath);
		this->gridData = cgnsReader2D.getGridData();
		cg_open(this->filePath.c_str(), CG_MODE_READ, &this->cgnsFile);
	}

	~Cgns2D() {
		cg_close(this->cgnsFile);
	};

	std::string filePath;
	GridData gridData;
	int cgnsFile;
	char elementSectionName[100];
	ElementType_t type;
	cgsize_t elementStart;
	cgsize_t elementEnd;
	int nbndry;
	int parent_flag;
};

FixtureTestSuite(ReadCgns2D, Cgns2D)

TestCase(Coordinates) {
	std::vector<std::vector<double>> coordinates = this->gridData.coordinates;

	checkEqual(static_cast<int>(coordinates.size()), 5);
	checkClose(coordinates[0][0], 0.0, TOLERANCE); checkClose(coordinates[0][1], 0.0, TOLERANCE); checkClose(coordinates[0][2], 0.0, TOLERANCE);
	checkClose(coordinates[1][0], 1.0, TOLERANCE); checkClose(coordinates[1][1], 0.0, TOLERANCE); checkClose(coordinates[1][2], 0.0, TOLERANCE);
	checkClose(coordinates[2][0], 1.0, TOLERANCE); checkClose(coordinates[2][1], 1.0, TOLERANCE); checkClose(coordinates[2][2], 0.0, TOLERANCE);
	checkClose(coordinates[3][0], 0.0, TOLERANCE); checkClose(coordinates[3][1], 1.0, TOLERANCE); checkClose(coordinates[3][2], 0.0, TOLERANCE);
	checkClose(coordinates[4][0], 0.5, TOLERANCE); checkClose(coordinates[4][1], 0.5, TOLERANCE); checkClose(coordinates[4][2], 0.0, TOLERANCE);
}

TestCase(Elements) {
	std::vector<std::vector<int>> triangles = this->gridData.triangleConnectivity;

	checkEqual(static_cast<int>(triangles.size()), 4);
	checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 1); checkEqual(triangles[0][2], 4);
	checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1], 4); checkEqual(triangles[1][2], 3);
	checkEqual(triangles[2][0], 1); checkEqual(triangles[2][1], 2); checkEqual(triangles[2][2], 4);
	checkEqual(triangles[3][0], 2); checkEqual(triangles[3][1], 3); checkEqual(triangles[3][2], 4);
	cg_section_read(this->cgnsFile, 1, 1, 1, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 1);
	checkEqual(this->elementEnd  , 4);
}

TestCase(Boundaries) {
	std::vector<BoundaryData> boundaries = this->gridData.boundaries;

	checkEqual(static_cast<int>(boundaries.size()), 4);
}

TestCase(West) {
	BoundaryData west = this->gridData.boundaries[0];

	check(west.name == std::string("West"));

	std::vector<std::vector<int>> lines = west.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 3); checkEqual(lines[0][1], 0);
	cg_section_read(this->cgnsFile, 1, 1, 2, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 5);
	checkEqual(this->elementEnd  , 5);
}

TestCase(East) {
	BoundaryData east = this->gridData.boundaries[1];

	check(east.name == std::string("East"));

	std::vector<std::vector<int>> lines = east.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 1); checkEqual(lines[0][1], 2);
	cg_section_read(this->cgnsFile, 1, 1, 3, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 6);
	checkEqual(this->elementEnd  , 6);
}

TestCase(South) {
	BoundaryData south = this->gridData.boundaries[2];

	check(south.name == std::string("South"));

	std::vector<std::vector<int>> lines = south.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 0); checkEqual(lines[0][1], 1);
	cg_section_read(this->cgnsFile, 1, 1, 4, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 7);
	checkEqual(this->elementEnd  , 7);
}

TestCase(North) {
	BoundaryData north = this->gridData.boundaries[3];

	check(north.name == std::string("North"));

	std::vector<std::vector<int>> lines = north.lineConnectivity;
	checkEqual(static_cast<int>(lines.size()), 1);
	checkEqual(lines[0][0], 2); checkEqual(lines[0][1], 3);
	cg_section_read(this->cgnsFile, 1, 1, 5, this->elementSectionName, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
	checkEqual(this->elementStart, 8);
	checkEqual(this->elementEnd  , 8);
}

TestSuiteEnd()
