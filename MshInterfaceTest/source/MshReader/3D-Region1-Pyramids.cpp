#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/MshInterface/MshReaderFixture.hpp"

#define TOLERANCE 1e-12

struct Read_Region1_Pyramids_3D_Fixture : public MshReaderFixture {
    Read_Region1_Pyramids_3D_Fixture() : MshReaderFixture("MshInterface/3D-Region1-Pyramids/35v_48e.msh") {}
};

FixtureTestSuite(Read_Region1_Pyramids_3D_Suite, Read_Region1_Pyramids_3D_Fixture)

TestCase(Elements) {
    auto pyramids = this->gridData->pyramids;

    checkEqual((pyramids.size()), 48u);
    // checkEqual(pyramids[ 0][0], 13); checkEqual(pyramids[ 0][1],  9); checkEqual(pyramids[ 0][2], 10); checkEqual(pyramids[ 0][3], 12); checkEqual(pyramids[ 0][4],  0); checkEqual(pyramids[ 0][5],  0);
    // checkEqual(pyramids[ 1][0], 13); checkEqual(pyramids[ 1][1], 11); checkEqual(pyramids[ 1][2],  9); checkEqual(pyramids[ 1][3], 12); checkEqual(pyramids[ 1][4],  1); checkEqual(pyramids[ 1][5],  1);
    // checkEqual(pyramids[ 2][0],  8); checkEqual(pyramids[ 2][1], 13); checkEqual(pyramids[ 2][2], 10); checkEqual(pyramids[ 2][3], 12); checkEqual(pyramids[ 2][4],  2); checkEqual(pyramids[ 2][5],  2);
    // checkEqual(pyramids[ 3][0], 13); checkEqual(pyramids[ 3][1],  8); checkEqual(pyramids[ 3][2], 11); checkEqual(pyramids[ 3][3], 12); checkEqual(pyramids[ 3][4],  3); checkEqual(pyramids[ 3][5],  3);
    // checkEqual(pyramids[ 4][0],  0); checkEqual(pyramids[ 4][1],  3); checkEqual(pyramids[ 4][2],  8); checkEqual(pyramids[ 4][3], 12); checkEqual(pyramids[ 4][4],  4); checkEqual(pyramids[ 4][5],  4);
    // checkEqual(pyramids[ 5][0],  8); checkEqual(pyramids[ 5][1],  7); checkEqual(pyramids[ 5][2],  4); checkEqual(pyramids[ 5][3], 13); checkEqual(pyramids[ 5][4],  5); checkEqual(pyramids[ 5][5],  5);
    // checkEqual(pyramids[ 6][0],  5); checkEqual(pyramids[ 6][1],  9); checkEqual(pyramids[ 6][2],  1); checkEqual(pyramids[ 6][3], 10); checkEqual(pyramids[ 6][4],  6); checkEqual(pyramids[ 6][5],  6);
    // checkEqual(pyramids[ 7][0],  9); checkEqual(pyramids[ 7][1],  2); checkEqual(pyramids[ 7][2],  1); checkEqual(pyramids[ 7][3], 12); checkEqual(pyramids[ 7][4],  7); checkEqual(pyramids[ 7][5],  7);
    // checkEqual(pyramids[ 8][0],  3); checkEqual(pyramids[ 8][1],  7); checkEqual(pyramids[ 8][2],  8); checkEqual(pyramids[ 8][3], 11); checkEqual(pyramids[ 8][4],  8); checkEqual(pyramids[ 8][5],  8);
    // checkEqual(pyramids[ 9][0], 13); checkEqual(pyramids[ 9][1],  7); checkEqual(pyramids[ 9][2],  6); checkEqual(pyramids[ 9][3], 11); checkEqual(pyramids[ 9][4],  9); checkEqual(pyramids[ 9][5],  9);
    // checkEqual(pyramids[10][0],  5); checkEqual(pyramids[10][1], 13); checkEqual(pyramids[10][2],  6); checkEqual(pyramids[10][3],  9); checkEqual(pyramids[10][4], 10); checkEqual(pyramids[10][5], 10);
    // checkEqual(pyramids[11][0],  6); checkEqual(pyramids[11][1], 11); checkEqual(pyramids[11][2],  2); checkEqual(pyramids[11][3],  9); checkEqual(pyramids[11][4], 11); checkEqual(pyramids[11][5], 11);
    // checkEqual(pyramids[12][0], 11); checkEqual(pyramids[12][1],  3); checkEqual(pyramids[12][2],  2); checkEqual(pyramids[12][3], 12); checkEqual(pyramids[12][4], 12); checkEqual(pyramids[12][5], 12);
    // checkEqual(pyramids[13][0],  5); checkEqual(pyramids[13][1],  4); checkEqual(pyramids[13][2], 13); checkEqual(pyramids[13][3], 10); checkEqual(pyramids[13][4], 13); checkEqual(pyramids[13][5], 13);
    // checkEqual(pyramids[14][0],  0); checkEqual(pyramids[14][1],  8); checkEqual(pyramids[14][2],  4); checkEqual(pyramids[14][3], 10); checkEqual(pyramids[14][4], 14); checkEqual(pyramids[14][5], 14);
    // checkEqual(pyramids[15][0],  1); checkEqual(pyramids[15][1],  0); checkEqual(pyramids[15][2], 10); checkEqual(pyramids[15][3], 12); checkEqual(pyramids[15][4], 15); checkEqual(pyramids[15][5], 15);
    // checkEqual(pyramids[16][0],  8); checkEqual(pyramids[16][1],  3); checkEqual(pyramids[16][2], 11); checkEqual(pyramids[16][3], 12); checkEqual(pyramids[16][4], 16); checkEqual(pyramids[16][5], 16);
    // checkEqual(pyramids[17][0],  8); checkEqual(pyramids[17][1],  7); checkEqual(pyramids[17][2], 13); checkEqual(pyramids[17][3], 11); checkEqual(pyramids[17][4], 17); checkEqual(pyramids[17][5], 17);
    // checkEqual(pyramids[18][0],  6); checkEqual(pyramids[18][1], 13); checkEqual(pyramids[18][2], 11); checkEqual(pyramids[18][3],  9); checkEqual(pyramids[18][4], 18); checkEqual(pyramids[18][5], 18);
    // checkEqual(pyramids[19][0], 11); checkEqual(pyramids[19][1],  2); checkEqual(pyramids[19][2],  9); checkEqual(pyramids[19][3], 12); checkEqual(pyramids[19][4], 19); checkEqual(pyramids[19][5], 19);
    // checkEqual(pyramids[20][0],  5); checkEqual(pyramids[20][1], 13); checkEqual(pyramids[20][2],  9); checkEqual(pyramids[20][3], 10); checkEqual(pyramids[20][4], 20); checkEqual(pyramids[20][5], 20);
    // checkEqual(pyramids[21][0],  4); checkEqual(pyramids[21][1],  8); checkEqual(pyramids[21][2], 13); checkEqual(pyramids[21][3], 10); checkEqual(pyramids[21][4], 21); checkEqual(pyramids[21][5], 21);
    // checkEqual(pyramids[22][0],  9); checkEqual(pyramids[22][1],  1); checkEqual(pyramids[22][2], 10); checkEqual(pyramids[22][3], 12); checkEqual(pyramids[22][4], 22); checkEqual(pyramids[22][5], 22);
    // checkEqual(pyramids[23][0],  0); checkEqual(pyramids[23][1],  8); checkEqual(pyramids[23][2], 10); checkEqual(pyramids[23][3], 12); checkEqual(pyramids[23][4], 23); checkEqual(pyramids[23][5], 23);
}

// TestCase(Facets) {
//     auto triangles = this->gridData->triangles;

//     checkEqual(triangles.size(), 24u);
//     checkEqual(triangles[ 0][0],  0); checkEqual(triangles[ 0][1],  8); checkEqual(triangles[ 0][2],  3); checkEqual(triangles[ 0][3], 24);
//     checkEqual(triangles[ 1][0],  0); checkEqual(triangles[ 1][1],  4); checkEqual(triangles[ 1][2],  8); checkEqual(triangles[ 1][3], 25);
//     checkEqual(triangles[ 2][0],  3); checkEqual(triangles[ 2][1],  8); checkEqual(triangles[ 2][2],  7); checkEqual(triangles[ 2][3], 26);
//     checkEqual(triangles[ 3][0],  4); checkEqual(triangles[ 3][1],  7); checkEqual(triangles[ 3][2],  8); checkEqual(triangles[ 3][3], 27);
//     checkEqual(triangles[ 4][0],  2); checkEqual(triangles[ 4][1],  9); checkEqual(triangles[ 4][2],  1); checkEqual(triangles[ 4][3], 28);
//     checkEqual(triangles[ 5][0],  9); checkEqual(triangles[ 5][1],  5); checkEqual(triangles[ 5][2],  1); checkEqual(triangles[ 5][3], 29);
//     checkEqual(triangles[ 6][0],  6); checkEqual(triangles[ 6][1],  9); checkEqual(triangles[ 6][2],  2); checkEqual(triangles[ 6][3], 30);
//     checkEqual(triangles[ 7][0],  9); checkEqual(triangles[ 7][1],  6); checkEqual(triangles[ 7][2],  5); checkEqual(triangles[ 7][3], 31);
//     checkEqual(triangles[ 8][0],  1); checkEqual(triangles[ 8][1], 10); checkEqual(triangles[ 8][2],  0); checkEqual(triangles[ 8][3], 32);
//     checkEqual(triangles[ 9][0], 10); checkEqual(triangles[ 9][1],  4); checkEqual(triangles[ 9][2],  0); checkEqual(triangles[ 9][3], 33);
//     checkEqual(triangles[10][0],  5); checkEqual(triangles[10][1], 10); checkEqual(triangles[10][2],  1); checkEqual(triangles[10][3], 34);
//     checkEqual(triangles[11][0], 10); checkEqual(triangles[11][1],  5); checkEqual(triangles[11][2],  4); checkEqual(triangles[11][3], 35);
//     checkEqual(triangles[12][0],  3); checkEqual(triangles[12][1], 11); checkEqual(triangles[12][2],  2); checkEqual(triangles[12][3], 36);
//     checkEqual(triangles[13][0], 11); checkEqual(triangles[13][1],  6); checkEqual(triangles[13][2],  2); checkEqual(triangles[13][3], 37);
//     checkEqual(triangles[14][0],  7); checkEqual(triangles[14][1], 11); checkEqual(triangles[14][2],  3); checkEqual(triangles[14][3], 38);
//     checkEqual(triangles[15][0], 11); checkEqual(triangles[15][1],  7); checkEqual(triangles[15][2],  6); checkEqual(triangles[15][3], 39);
//     checkEqual(triangles[16][0],  0); checkEqual(triangles[16][1], 12); checkEqual(triangles[16][2],  1); checkEqual(triangles[16][3], 40);
//     checkEqual(triangles[17][0],  0); checkEqual(triangles[17][1],  3); checkEqual(triangles[17][2], 12); checkEqual(triangles[17][3], 41);
//     checkEqual(triangles[18][0],  1); checkEqual(triangles[18][1], 12); checkEqual(triangles[18][2],  2); checkEqual(triangles[18][3], 42);
//     checkEqual(triangles[19][0],  2); checkEqual(triangles[19][1], 12); checkEqual(triangles[19][2],  3); checkEqual(triangles[19][3], 43);
//     checkEqual(triangles[20][0],  5); checkEqual(triangles[20][1], 13); checkEqual(triangles[20][2],  4); checkEqual(triangles[20][3], 44);
//     checkEqual(triangles[21][0], 13); checkEqual(triangles[21][1],  7); checkEqual(triangles[21][2],  4); checkEqual(triangles[21][3], 45);
//     checkEqual(triangles[22][0],  6); checkEqual(triangles[22][1], 13); checkEqual(triangles[22][2],  5); checkEqual(triangles[22][3], 46);
//     checkEqual(triangles[23][0],  7); checkEqual(triangles[23][1], 13); checkEqual(triangles[23][2],  6); checkEqual(triangles[23][3], 47);
// }

// TestCase(Regions) {
//     checkEqual(this->gridData->regions.size(), 1u);
// }

// TestCase(Geometry) {
//     auto region = this->gridData->regions[0];

//     checkEqual(region.name, std::string("GEOMETRY"));

//     checkEqual(region.begin, 0);
//     checkEqual(region.end, 24);

//     auto vertices = region.vertices;
//     checkEqual(vertices.size(), 14u);
//     for (int v = 0; v < 14; ++v) {
//         checkEqual(vertices[v],  v);
//     }
// }

// TestCase(Boundaries) {
//     checkEqual(this->gridData->boundaries.size(), 6u);
// }

// TestCase(West) {
//     auto boundary = this->gridData->boundaries[0];

//     checkEqual(boundary.name, std::string("WEST"));

//     checkEqual(boundary.begin, 24);
//     checkEqual(boundary.end, 28);

//     auto vertices = boundary.vertices;
//     checkEqual((vertices.size()), 5u);
//     checkEqual(vertices[0], 0);
//     checkEqual(vertices[1], 3);
//     checkEqual(vertices[2], 4);
//     checkEqual(vertices[3], 7);
//     checkEqual(vertices[4], 8);
// }

// TestCase(East) {
//     auto boundary = this->gridData->boundaries[1];

//     checkEqual(boundary.name, std::string("EAST"));

//     checkEqual(boundary.begin, 28);
//     checkEqual(boundary.end, 32);

//     auto vertices = boundary.vertices;
//     checkEqual((vertices.size()), 5u);
//     checkEqual(vertices[0], 1);
//     checkEqual(vertices[1], 2);
//     checkEqual(vertices[2], 5);
//     checkEqual(vertices[3], 6);
//     checkEqual(vertices[4], 9);
// }

// TestCase(South) {
//     auto boundary = this->gridData->boundaries[2];

//     checkEqual(boundary.name, std::string("SOUTH"));

//     checkEqual(boundary.begin, 32);
//     checkEqual(boundary.end, 36);

//     auto vertices = boundary.vertices;
//     checkEqual((vertices.size()), 5u);
//     checkEqual(vertices[0], 0);
//     checkEqual(vertices[1], 1);
//     checkEqual(vertices[2], 4);
//     checkEqual(vertices[3], 5);
//     checkEqual(vertices[4], 10);
// }

// TestCase(North) {
//     auto boundary = this->gridData->boundaries[3];

//     checkEqual(boundary.name, std::string("NORTH"));

//     checkEqual(boundary.begin, 36);
//     checkEqual(boundary.end, 40);

//     auto vertices = boundary.vertices;
//     checkEqual((vertices.size()), 5u);
//     checkEqual(vertices[0],  2);
//     checkEqual(vertices[1],  3);
//     checkEqual(vertices[2],  6);
//     checkEqual(vertices[3],  7);
//     checkEqual(vertices[4], 11);
// }

// TestCase(Bottom) {
//     auto boundary = this->gridData->boundaries[4];

//     checkEqual(boundary.name, std::string("BOTTOM"));

//     checkEqual(boundary.begin, 40);
//     checkEqual(boundary.end, 44);

//     auto vertices = boundary.vertices;
//     checkEqual((vertices.size()), 5u);
//     checkEqual(vertices[0],  0);
//     checkEqual(vertices[1],  1);
//     checkEqual(vertices[2],  2);
//     checkEqual(vertices[3],  3);
//     checkEqual(vertices[4], 12);
// }

// TestCase(Top) {
//     auto boundary = this->gridData->boundaries[5];

//     checkEqual(boundary.name, std::string("TOP"));

//     checkEqual(boundary.begin, 44);
//     checkEqual(boundary.end, 48);

//     auto vertices = boundary.vertices;
//     checkEqual((vertices.size()), 5u);
//     checkEqual(vertices[0],  4);
//     checkEqual(vertices[1],  5);
//     checkEqual(vertices[2],  6);
//     checkEqual(vertices[3],  7);
//     checkEqual(vertices[4], 13);
// }

TestSuiteEnd()
