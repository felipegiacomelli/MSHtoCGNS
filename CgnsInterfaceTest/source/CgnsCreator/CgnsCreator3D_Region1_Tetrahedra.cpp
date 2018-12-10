#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include <cgnslib.h>

#define TOLERANCE 1e-12

struct Region1_Tetrahedra_3D {
    Region1_Tetrahedra_3D() {
        CgnsReader3D inputReader(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Tetrahedra/14v_24e.cgns");
        CgnsCreator3D fileIndex3D(inputReader.gridData, "./");
        this->filePath = fileIndex3D.getFileName();
        CgnsReader3D outputReader(this->filePath);
        this->gridData = outputReader.gridData;
        cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex);
    }

    ~Region1_Tetrahedra_3D() {
        cg_close(this->fileIndex);
        deleteDirectory("./14v_24e/");
    };

    std::string filePath;
    boost::shared_ptr<GridData> gridData;
    int fileIndex;
    char name[100];
    ElementType_t type;
    int elementStart;
    int end;
    int nbndry;
    int parent_flag;
};

FixtureTestSuite(Generate_Region1_Tetrahedra_3D, Region1_Tetrahedra_3D)

TestCase(Coordinates) {
    auto coordinates = this->gridData->coordinates;

    checkEqual(coordinates.size(), 14u);
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

    checkEqual(tetrahedra.size(), 24u);
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

    cg_section_read(this->fileIndex, 1, 1, 1, this->name, &this->type, &this->elementStart, &this->end, &this->nbndry, &this->parent_flag);
    check(std::string(this->name) == std::string("GEOMETRY"));
    check(this->type == TETRA_4);
    checkEqual(this->elementStart, 1);
    checkEqual(this->end  , 24);
}

TestCase(Facets) {
    auto triangles = this->gridData->triangleConnectivity;

    checkEqual(triangles.size(), 24u);
    checkEqual(triangles[ 0][0],  0); checkEqual(triangles[ 0][1],  8); checkEqual(triangles[ 0][2],  3); checkEqual(triangles[ 0][3], 24);
    checkEqual(triangles[ 1][0],  0); checkEqual(triangles[ 1][1],  4); checkEqual(triangles[ 1][2],  8); checkEqual(triangles[ 1][3], 25);
    checkEqual(triangles[ 2][0],  3); checkEqual(triangles[ 2][1],  8); checkEqual(triangles[ 2][2],  7); checkEqual(triangles[ 2][3], 26);
    checkEqual(triangles[ 3][0],  4); checkEqual(triangles[ 3][1],  7); checkEqual(triangles[ 3][2],  8); checkEqual(triangles[ 3][3], 27);
    checkEqual(triangles[ 4][0],  2); checkEqual(triangles[ 4][1],  9); checkEqual(triangles[ 4][2],  1); checkEqual(triangles[ 4][3], 28);
    checkEqual(triangles[ 5][0],  9); checkEqual(triangles[ 5][1],  5); checkEqual(triangles[ 5][2],  1); checkEqual(triangles[ 5][3], 29);
    checkEqual(triangles[ 6][0],  6); checkEqual(triangles[ 6][1],  9); checkEqual(triangles[ 6][2],  2); checkEqual(triangles[ 6][3], 30);
    checkEqual(triangles[ 7][0],  9); checkEqual(triangles[ 7][1],  6); checkEqual(triangles[ 7][2],  5); checkEqual(triangles[ 7][3], 31);
    checkEqual(triangles[ 8][0],  1); checkEqual(triangles[ 8][1], 10); checkEqual(triangles[ 8][2],  0); checkEqual(triangles[ 8][3], 32);
    checkEqual(triangles[ 9][0], 10); checkEqual(triangles[ 9][1],  4); checkEqual(triangles[ 9][2],  0); checkEqual(triangles[ 9][3], 33);
    checkEqual(triangles[10][0],  5); checkEqual(triangles[10][1], 10); checkEqual(triangles[10][2],  1); checkEqual(triangles[10][3], 34);
    checkEqual(triangles[11][0], 10); checkEqual(triangles[11][1],  5); checkEqual(triangles[11][2],  4); checkEqual(triangles[11][3], 35);
    checkEqual(triangles[12][0],  3); checkEqual(triangles[12][1], 11); checkEqual(triangles[12][2],  2); checkEqual(triangles[12][3], 36);
    checkEqual(triangles[13][0], 11); checkEqual(triangles[13][1],  6); checkEqual(triangles[13][2],  2); checkEqual(triangles[13][3], 37);
    checkEqual(triangles[14][0],  7); checkEqual(triangles[14][1], 11); checkEqual(triangles[14][2],  3); checkEqual(triangles[14][3], 38);
    checkEqual(triangles[15][0], 11); checkEqual(triangles[15][1],  7); checkEqual(triangles[15][2],  6); checkEqual(triangles[15][3], 39);
    checkEqual(triangles[16][0],  0); checkEqual(triangles[16][1], 12); checkEqual(triangles[16][2],  1); checkEqual(triangles[16][3], 40);
    checkEqual(triangles[17][0],  0); checkEqual(triangles[17][1],  3); checkEqual(triangles[17][2], 12); checkEqual(triangles[17][3], 41);
    checkEqual(triangles[18][0],  1); checkEqual(triangles[18][1], 12); checkEqual(triangles[18][2],  2); checkEqual(triangles[18][3], 42);
    checkEqual(triangles[19][0],  2); checkEqual(triangles[19][1], 12); checkEqual(triangles[19][2],  3); checkEqual(triangles[19][3], 43);
    checkEqual(triangles[20][0],  5); checkEqual(triangles[20][1], 13); checkEqual(triangles[20][2],  4); checkEqual(triangles[20][3], 44);
    checkEqual(triangles[21][0], 13); checkEqual(triangles[21][1],  7); checkEqual(triangles[21][2],  4); checkEqual(triangles[21][3], 45);
    checkEqual(triangles[22][0],  6); checkEqual(triangles[22][1], 13); checkEqual(triangles[22][2],  5); checkEqual(triangles[22][3], 46);
    checkEqual(triangles[23][0],  7); checkEqual(triangles[23][1], 13); checkEqual(triangles[23][2],  6); checkEqual(triangles[23][3], 47);
}

TestCase(Regions) {
    checkEqual(this->gridData->regions.size(), 1u);
}

TestCase(Geometry) {
    auto region = this->gridData->regions[0];

    check(region.name == std::string("GEOMETRY"));

    checkEqual(region.begin, 0);
    checkEqual(region.end, 24);
}

TestCase(Boundaries) {
    checkEqual(this->gridData->boundaries.size(), 6u);
}

TestCase(West) {
    auto boundary = this->gridData->boundaries[0];

    check(boundary.name == std::string("WEST"));

    checkEqual(boundary.begin, 24);
    checkEqual(boundary.end, 28);

    auto vertices = boundary.vertices;
    checkEqual((vertices.size()), 5u);
    checkEqual(vertices[0], 0);
    checkEqual(vertices[1], 3);
    checkEqual(vertices[2], 4);
    checkEqual(vertices[3], 7);
    checkEqual(vertices[4], 8);

    cg_section_read(this->fileIndex, 1, 1, 2, this->name, &this->type, &this->elementStart, &this->end, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 25);
    checkEqual(this->end  , 28);
}

TestCase(East) {
    auto boundary = this->gridData->boundaries[1];

    check(boundary.name == std::string("EAST"));

    checkEqual(boundary.begin, 28);
    checkEqual(boundary.end, 32);

    auto vertices = boundary.vertices;
    checkEqual((vertices.size()), 5u);
    checkEqual(vertices[0], 1);
    checkEqual(vertices[1], 2);
    checkEqual(vertices[2], 5);
    checkEqual(vertices[3], 6);
    checkEqual(vertices[4], 9);

    cg_section_read(this->fileIndex, 1, 1, 3, this->name, &this->type, &this->elementStart, &this->end, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 29);
    checkEqual(this->end  , 32);
}

TestCase(South) {
    auto boundary = this->gridData->boundaries[2];

    check(boundary.name == std::string("SOUTH"));

    checkEqual(boundary.begin, 32);
    checkEqual(boundary.end, 36);

    auto vertices = boundary.vertices;
    checkEqual((vertices.size()), 5u);
    checkEqual(vertices[0], 0);
    checkEqual(vertices[1], 1);
    checkEqual(vertices[2], 4);
    checkEqual(vertices[3], 5);
    checkEqual(vertices[4], 10);

    cg_section_read(this->fileIndex, 1, 1, 4, this->name, &this->type, &this->elementStart, &this->end, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 33);
    checkEqual(this->end  , 36);
}

TestCase(North) {
    auto boundary = this->gridData->boundaries[3];

    check(boundary.name == std::string("NORTH"));

    checkEqual(boundary.begin, 36);
    checkEqual(boundary.end, 40);

    auto vertices = boundary.vertices;
    checkEqual((vertices.size()), 5u);
    checkEqual(vertices[0],  2);
    checkEqual(vertices[1],  3);
    checkEqual(vertices[2],  6);
    checkEqual(vertices[3],  7);
    checkEqual(vertices[4], 11);

    cg_section_read(this->fileIndex, 1, 1, 5, this->name, &this->type, &this->elementStart, &this->end, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 37);
    checkEqual(this->end  , 40);
}

TestCase(Bottom) {
    auto boundary = this->gridData->boundaries[4];

    check(boundary.name == std::string("BOTTOM"));

    checkEqual(boundary.begin, 40);
    checkEqual(boundary.end, 44);

    auto vertices = boundary.vertices;
    checkEqual((vertices.size()), 5u);
    checkEqual(vertices[0],  0);
    checkEqual(vertices[1],  1);
    checkEqual(vertices[2],  2);
    checkEqual(vertices[3],  3);
    checkEqual(vertices[4], 12);

    cg_section_read(this->fileIndex, 1, 1, 6, this->name, &this->type, &this->elementStart, &this->end, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 41);
    checkEqual(this->end  , 44);
}

TestCase(Top) {
    auto boundary = this->gridData->boundaries[5];

    check(boundary.name == std::string("TOP"));

    checkEqual(boundary.begin, 44);
    checkEqual(boundary.end, 48);

    auto vertices = boundary.vertices;
    checkEqual((vertices.size()), 5u);
    checkEqual(vertices[0],  4);
    checkEqual(vertices[1],  5);
    checkEqual(vertices[2],  6);
    checkEqual(vertices[3],  7);
    checkEqual(vertices[4], 13);

    cg_section_read(this->fileIndex, 1, 1, 7, this->name, &this->type, &this->elementStart, &this->end, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 45);
    checkEqual(this->end  , 48);
}

TestSuiteEnd()
