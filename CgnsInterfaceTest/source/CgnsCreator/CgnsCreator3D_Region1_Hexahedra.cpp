#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"
#include <cgnslib.h>

#define TOLERANCE 1e-12

struct Region1_Hexahedra_3D {
    Region1_Hexahedra_3D() {
        CgnsReader3D inputReader(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Hexahedra/27v_8e.cgns");
        CgnsCreator3D fileIndex3D(inputReader.gridData, "./");
        this->filePath = fileIndex3D.getFileName();
        CgnsReader3D outputReader(this->filePath);
        this->gridData = outputReader.gridData;
        cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex);
    }

    ~Region1_Hexahedra_3D() {
        cg_close(this->fileIndex);
        deleteDirectory("./27v_8e/");
    };

    std::string filePath;
    boost::shared_ptr<GridData> gridData;
    int fileIndex;
    char name[100];
    ElementType_t type;
    int elementStart;
    int elementEnd;
    int nbndry;
    int parent_flag;
};

FixtureTestSuite(Generate_Region1_Hexahedra_3D, Region1_Hexahedra_3D)

TestCase(Coordinates) {
    auto coordinates = this->gridData->coordinates;

    checkEqual(coordinates.size(), 27u);
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

    checkEqual(hexahedra.size(), 8u);

    checkEqual(hexahedra[0][0],  0); checkEqual(hexahedra[0][1],  1); checkEqual(hexahedra[0][2],  4); checkEqual(hexahedra[0][3],  3);
    checkEqual(hexahedra[1][0],  1); checkEqual(hexahedra[1][1],  2); checkEqual(hexahedra[1][2],  5); checkEqual(hexahedra[1][3],  4);
    checkEqual(hexahedra[2][0],  3); checkEqual(hexahedra[2][1],  4); checkEqual(hexahedra[2][2],  7); checkEqual(hexahedra[2][3],  6);
    checkEqual(hexahedra[3][0],  4); checkEqual(hexahedra[3][1],  5); checkEqual(hexahedra[3][2],  8); checkEqual(hexahedra[3][3],  7);
    checkEqual(hexahedra[4][0],  9); checkEqual(hexahedra[4][1], 10); checkEqual(hexahedra[4][2], 13); checkEqual(hexahedra[4][3], 12);
    checkEqual(hexahedra[5][0], 10); checkEqual(hexahedra[5][1], 11); checkEqual(hexahedra[5][2], 14); checkEqual(hexahedra[5][3], 13);
    checkEqual(hexahedra[6][0], 12); checkEqual(hexahedra[6][1], 13); checkEqual(hexahedra[6][2], 16); checkEqual(hexahedra[6][3], 15);
    checkEqual(hexahedra[7][0], 13); checkEqual(hexahedra[7][1], 14); checkEqual(hexahedra[7][2], 17); checkEqual(hexahedra[7][3], 16);

    checkEqual(hexahedra[0][4],  9); checkEqual(hexahedra[0][5], 10); checkEqual(hexahedra[0][6], 13); checkEqual(hexahedra[0][7], 12);
    checkEqual(hexahedra[1][4], 10); checkEqual(hexahedra[1][5], 11); checkEqual(hexahedra[1][6], 14); checkEqual(hexahedra[1][7], 13);
    checkEqual(hexahedra[2][4], 12); checkEqual(hexahedra[2][5], 13); checkEqual(hexahedra[2][6], 16); checkEqual(hexahedra[2][7], 15);
    checkEqual(hexahedra[3][4], 13); checkEqual(hexahedra[3][5], 14); checkEqual(hexahedra[3][6], 17); checkEqual(hexahedra[3][7], 16);
    checkEqual(hexahedra[4][4], 18); checkEqual(hexahedra[4][5], 19); checkEqual(hexahedra[4][6], 22); checkEqual(hexahedra[4][7], 21);
    checkEqual(hexahedra[5][4], 19); checkEqual(hexahedra[5][5], 20); checkEqual(hexahedra[5][6], 23); checkEqual(hexahedra[5][7], 22);
    checkEqual(hexahedra[6][4], 21); checkEqual(hexahedra[6][5], 22); checkEqual(hexahedra[6][6], 25); checkEqual(hexahedra[6][7], 24);
    checkEqual(hexahedra[7][4], 22); checkEqual(hexahedra[7][5], 23); checkEqual(hexahedra[7][6], 26); checkEqual(hexahedra[7][7], 25);

    checkEqual(hexahedra[0][8], 0);
    checkEqual(hexahedra[1][8], 1);
    checkEqual(hexahedra[2][8], 2);
    checkEqual(hexahedra[3][8], 3);
    checkEqual(hexahedra[4][8], 4);
    checkEqual(hexahedra[5][8], 5);
    checkEqual(hexahedra[6][8], 6);
    checkEqual(hexahedra[7][8], 7);

    cg_section_read(this->fileIndex, 1, 1, 1, this->name, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
    check(std::string(this->name) == std::string("GEOMETRY"));
    check(this->type == HEXA_8);
    checkEqual(this->elementStart, 1);
    checkEqual(this->elementEnd  , 8);
}

TestCase(Facets) {
    auto quadrangles = this->gridData->quadrangleConnectivity;

    checkEqual(quadrangles[ 0][0],  0); checkEqual(quadrangles[ 0][1],  9); checkEqual(quadrangles[ 0][2], 12); checkEqual(quadrangles[ 0][3],  3); checkEqual(quadrangles[ 0][4],  8);
    checkEqual(quadrangles[ 1][0],  3); checkEqual(quadrangles[ 1][1], 12); checkEqual(quadrangles[ 1][2], 15); checkEqual(quadrangles[ 1][3],  6); checkEqual(quadrangles[ 1][4],  9);
    checkEqual(quadrangles[ 2][0],  9); checkEqual(quadrangles[ 2][1], 18); checkEqual(quadrangles[ 2][2], 21); checkEqual(quadrangles[ 2][3], 12); checkEqual(quadrangles[ 2][4], 10);
    checkEqual(quadrangles[ 3][0], 12); checkEqual(quadrangles[ 3][1], 21); checkEqual(quadrangles[ 3][2], 24); checkEqual(quadrangles[ 3][3], 15); checkEqual(quadrangles[ 3][4], 11);
    checkEqual(quadrangles[ 4][0],  2); checkEqual(quadrangles[ 4][1],  5); checkEqual(quadrangles[ 4][2], 14); checkEqual(quadrangles[ 4][3], 11); checkEqual(quadrangles[ 4][4], 12);
    checkEqual(quadrangles[ 5][0],  5); checkEqual(quadrangles[ 5][1],  8); checkEqual(quadrangles[ 5][2], 17); checkEqual(quadrangles[ 5][3], 14); checkEqual(quadrangles[ 5][4], 13);
    checkEqual(quadrangles[ 6][0], 11); checkEqual(quadrangles[ 6][1], 14); checkEqual(quadrangles[ 6][2], 23); checkEqual(quadrangles[ 6][3], 20); checkEqual(quadrangles[ 6][4], 14);
    checkEqual(quadrangles[ 7][0], 14); checkEqual(quadrangles[ 7][1], 17); checkEqual(quadrangles[ 7][2], 26); checkEqual(quadrangles[ 7][3], 23); checkEqual(quadrangles[ 7][4], 15);
    checkEqual(quadrangles[ 8][0],  0); checkEqual(quadrangles[ 8][1],  1); checkEqual(quadrangles[ 8][2], 10); checkEqual(quadrangles[ 8][3],  9); checkEqual(quadrangles[ 8][4], 16);
    checkEqual(quadrangles[ 9][0],  1); checkEqual(quadrangles[ 9][1],  2); checkEqual(quadrangles[ 9][2], 11); checkEqual(quadrangles[ 9][3], 10); checkEqual(quadrangles[ 9][4], 17);
    checkEqual(quadrangles[10][0],  9); checkEqual(quadrangles[10][1], 10); checkEqual(quadrangles[10][2], 19); checkEqual(quadrangles[10][3], 18); checkEqual(quadrangles[10][4], 18);
    checkEqual(quadrangles[11][0], 10); checkEqual(quadrangles[11][1], 11); checkEqual(quadrangles[11][2], 20); checkEqual(quadrangles[11][3], 19); checkEqual(quadrangles[11][4], 19);
    checkEqual(quadrangles[12][0],  7); checkEqual(quadrangles[12][1],  6); checkEqual(quadrangles[12][2], 15); checkEqual(quadrangles[12][3], 16); checkEqual(quadrangles[12][4], 20);
    checkEqual(quadrangles[13][0],  8); checkEqual(quadrangles[13][1],  7); checkEqual(quadrangles[13][2], 16); checkEqual(quadrangles[13][3], 17); checkEqual(quadrangles[13][4], 21);
    checkEqual(quadrangles[14][0], 16); checkEqual(quadrangles[14][1], 15); checkEqual(quadrangles[14][2], 24); checkEqual(quadrangles[14][3], 25); checkEqual(quadrangles[14][4], 22);
    checkEqual(quadrangles[15][0], 17); checkEqual(quadrangles[15][1], 16); checkEqual(quadrangles[15][2], 25); checkEqual(quadrangles[15][3], 26); checkEqual(quadrangles[15][4], 23);
    checkEqual(quadrangles[16][0],  1); checkEqual(quadrangles[16][1],  0); checkEqual(quadrangles[16][2],  3); checkEqual(quadrangles[16][3],  4); checkEqual(quadrangles[16][4], 24);
    checkEqual(quadrangles[17][0],  2); checkEqual(quadrangles[17][1],  1); checkEqual(quadrangles[17][2],  4); checkEqual(quadrangles[17][3],  5); checkEqual(quadrangles[17][4], 25);
    checkEqual(quadrangles[18][0],  4); checkEqual(quadrangles[18][1],  3); checkEqual(quadrangles[18][2],  6); checkEqual(quadrangles[18][3],  7); checkEqual(quadrangles[18][4], 26);
    checkEqual(quadrangles[19][0],  5); checkEqual(quadrangles[19][1],  4); checkEqual(quadrangles[19][2],  7); checkEqual(quadrangles[19][3],  8); checkEqual(quadrangles[19][4], 27);
    checkEqual(quadrangles[20][0], 18); checkEqual(quadrangles[20][1], 19); checkEqual(quadrangles[20][2], 22); checkEqual(quadrangles[20][3], 21); checkEqual(quadrangles[20][4], 28);
    checkEqual(quadrangles[21][0], 19); checkEqual(quadrangles[21][1], 20); checkEqual(quadrangles[21][2], 23); checkEqual(quadrangles[21][3], 22); checkEqual(quadrangles[21][4], 29);
    checkEqual(quadrangles[22][0], 21); checkEqual(quadrangles[22][1], 22); checkEqual(quadrangles[22][2], 25); checkEqual(quadrangles[22][3], 24); checkEqual(quadrangles[22][4], 30);
    checkEqual(quadrangles[23][0], 22); checkEqual(quadrangles[23][1], 23); checkEqual(quadrangles[23][2], 26); checkEqual(quadrangles[23][3], 25); checkEqual(quadrangles[23][4], 31);
}

TestCase(Regions) {
    checkEqual(this->gridData->regions.size(), 1u);
}

TestCase(Geometry) {
    auto region = this->gridData->regions[0];

    check(region.name == std::string("GEOMETRY"));

    checkEqual(region.begin, 0);
    checkEqual(region.end, 8);
}

TestCase(Boundaries) {
    checkEqual(this->gridData->boundaries.size(), 6u);
}

TestCase(West) {
    auto boundary = this->gridData->boundaries[0];

    check(boundary.name == std::string("WEST"));

    checkEqual(boundary.begin, 8);
    checkEqual(boundary.end, 12);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 9u);
    checkEqual(vertices[0],  0);
    checkEqual(vertices[1],  3);
    checkEqual(vertices[2],  6);
    checkEqual(vertices[3],  9);
    checkEqual(vertices[4], 12);
    checkEqual(vertices[5], 15);
    checkEqual(vertices[6], 18);
    checkEqual(vertices[7], 21);
    checkEqual(vertices[8], 24);

    cg_section_read(this->fileIndex, 1, 1, 2, this->name, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart,  9);
    checkEqual(this->elementEnd  , 12);
}

TestCase(East) {
    auto boundary = this->gridData->boundaries[1];

    check(boundary.name == std::string("EAST"));

    checkEqual(boundary.begin, 12);
    checkEqual(boundary.end, 16);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 9u);
    checkEqual(vertices[0],  2);
    checkEqual(vertices[1],  5);
    checkEqual(vertices[2],  8);
    checkEqual(vertices[3], 11);
    checkEqual(vertices[4], 14);
    checkEqual(vertices[5], 17);
    checkEqual(vertices[6], 20);
    checkEqual(vertices[7], 23);
    checkEqual(vertices[8], 26);

    cg_section_read(this->fileIndex, 1, 1, 3, this->name, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 13);
    checkEqual(this->elementEnd  , 16);
}

TestCase(South) {
    auto boundary = this->gridData->boundaries[2];

    check(boundary.name == std::string("SOUTH"));

    checkEqual(boundary.begin, 16);
    checkEqual(boundary.end, 20);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 9u);
    checkEqual(vertices[0],  0);
    checkEqual(vertices[1],  1);
    checkEqual(vertices[2],  2);
    checkEqual(vertices[3],  9);
    checkEqual(vertices[4], 10);
    checkEqual(vertices[5], 11);
    checkEqual(vertices[6], 18);
    checkEqual(vertices[7], 19);
    checkEqual(vertices[8], 20);

    cg_section_read(this->fileIndex, 1, 1, 4, this->name, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 17);
    checkEqual(this->elementEnd  , 20);
}

TestCase(North) {
    auto boundary = this->gridData->boundaries[3];

    check(boundary.name == std::string("NORTH"));

    checkEqual(boundary.begin, 20);
    checkEqual(boundary.end, 24);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 9u);
    checkEqual(vertices[0],  6);
    checkEqual(vertices[1],  7);
    checkEqual(vertices[2],  8);
    checkEqual(vertices[3], 15);
    checkEqual(vertices[4], 16);
    checkEqual(vertices[5], 17);
    checkEqual(vertices[6], 24);
    checkEqual(vertices[7], 25);
    checkEqual(vertices[8], 26);

    cg_section_read(this->fileIndex, 1, 1, 5, this->name, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 21);
    checkEqual(this->elementEnd  , 24);
}

TestCase(Bottom) {
    auto boundary = this->gridData->boundaries[4];

    check(boundary.name == std::string("BOTTOM"));

    checkEqual(boundary.begin, 24);
    checkEqual(boundary.end, 28);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 9u);
    checkEqual(vertices[0], 0);
    checkEqual(vertices[1], 1);
    checkEqual(vertices[2], 2);
    checkEqual(vertices[3], 3);
    checkEqual(vertices[4], 4);
    checkEqual(vertices[5], 5);
    checkEqual(vertices[6], 6);
    checkEqual(vertices[7], 7);
    checkEqual(vertices[8], 8);

    cg_section_read(this->fileIndex, 1, 1, 6, this->name, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 25);
    checkEqual(this->elementEnd  , 28);
}

TestCase(Top) {
    auto boundary = this->gridData->boundaries[5];

    check(boundary.name == std::string("TOP"));

    checkEqual(boundary.begin, 28);
    checkEqual(boundary.end, 32);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 9u);
    checkEqual(vertices[0], 18);
    checkEqual(vertices[1], 19);
    checkEqual(vertices[2], 20);
    checkEqual(vertices[3], 21);
    checkEqual(vertices[4], 22);
    checkEqual(vertices[5], 23);
    checkEqual(vertices[6], 24);
    checkEqual(vertices[7], 25);
    checkEqual(vertices[8], 26);

    cg_section_read(this->fileIndex, 1, 1, 7, this->name, &this->type, &this->elementStart, &this->elementEnd, &this->nbndry, &this->parent_flag);
    checkEqual(this->elementStart, 29);
    checkEqual(this->elementEnd  , 32);
}

TestSuiteEnd()
