#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"
#include "MSHtoCGNS/FileMend/GridDataExtractor.hpp"

#define TOLERANCE 1e-6

struct GridDataExtractorFixture {
    GridDataExtractorFixture() {
        MshReader3D mshReader3D(this->inputPath);
        this->gridData = mshReader3D.gridData;
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractor/4x4x4_2x2x2.msh";
    boost::shared_ptr<GridData> gridData;
    std::string gridDataExtractorScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractor/ScriptGridDataExtractor.json";
};

FixtureTestSuite(GridDataExtractorSuite, GridDataExtractorFixture)

TestCase(GridDataExtractorTest) {
    checkEqual(this->gridData->coordinates.size(), 152u);

    GridDataExtractor gridDataExtractor(this->gridData, this->gridDataExtractorScript);

    checkEqual(this->gridData->coordinates.size(), 152u);

    checkEqual(this->gridData->tetrahedrons.size(), 0u);
    checkEqual(this->gridData->hexahedrons.size(), 72u);
    checkEqual(this->gridData->prisms.size(), 0u);
    checkEqual(this->gridData->pyramids.size(), 0u);

    checkEqual(this->gridData->triangles.size(), 0u);
    checkEqual(this->gridData->quadrangles.size(), 96u);

    checkEqual(this->gridData->lines.size(), 0u);

    checkEqual(this->gridData->boundaries.size(), 6u);
    checkEqual(this->gridData->regions.size(), 2u);
    checkEqual(this->gridData->wells.size(), 0u);

    checkEqual(this->gridData->boundaries[0].begin, 72);
    checkEqual(this->gridData->boundaries[0].end, 88);

    checkEqual(this->gridData->boundaries[1].begin,  88);
    checkEqual(this->gridData->boundaries[1].end, 104);

    checkEqual(this->gridData->boundaries[2].begin, 104);
    checkEqual(this->gridData->boundaries[2].end, 120);

    checkEqual(this->gridData->boundaries[3].begin, 120);
    checkEqual(this->gridData->boundaries[3].end, 136);

    checkEqual(this->gridData->boundaries[4].begin, 136);
    checkEqual(this->gridData->boundaries[4].end, 152);

    checkEqual(this->gridData->boundaries[5].begin, 152);
    checkEqual(this->gridData->boundaries[5].end, 168);

    checkEqual(this->gridData->regions[0].begin,  0);
    checkEqual(this->gridData->regions[0].end, 64);

    checkEqual(this->gridData->regions[1].begin, 64);
    checkEqual(this->gridData->regions[1].end, 72);

    boost::shared_ptr<GridData> extract = gridDataExtractor.extract;

    checkEqual(extract->coordinates.size(), 27u);

    checkEqual(extract->tetrahedrons.size(), 0u);
    checkEqual(extract->hexahedrons.size(), 8u);
    checkEqual(extract->prisms.size(), 0u);
    checkEqual(extract->pyramids.size(), 0u);

    checkEqual(extract->triangles.size(), 0u);
    checkEqual(extract->quadrangles.size(), 24u);

    checkEqual(extract->lines.size(), 0u);

    checkEqual(extract->boundaries.size(), 6u);
    checkEqual(extract->regions.size(), 1u);
    checkEqual(extract->wells.size(), 0u);

    checkEqual(extract->boundaries[0].begin,  8);
    checkEqual(extract->boundaries[0].end, 12);

    checkEqual(extract->boundaries[1].begin, 12);
    checkEqual(extract->boundaries[1].end, 16);

    checkEqual(extract->boundaries[2].begin, 16);
    checkEqual(extract->boundaries[2].end, 20);

    checkEqual(extract->boundaries[3].begin, 20);
    checkEqual(extract->boundaries[3].end, 24);

    checkEqual(extract->boundaries[4].begin, 24);
    checkEqual(extract->boundaries[4].end, 28);

    checkEqual(extract->boundaries[5].begin, 28);
    checkEqual(extract->boundaries[5].end, 32);

    checkEqual(extract->regions[0].begin, 0);
    checkEqual(extract->regions[0].end, 8);

    checkClose(extract->coordinates[ 0][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 0][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 0][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 1][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 1][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 1][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 2][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 2][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 2][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 3][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 3][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 3][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 4][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 4][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 4][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 5][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 5][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 5][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 6][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 6][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 6][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 7][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 7][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 7][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 8][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 8][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[ 8][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[ 9][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[ 9][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[ 9][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[10][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[10][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[10][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[11][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[11][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[11][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[12][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[12][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[12][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[13][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[13][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[13][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[14][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[14][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[14][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[15][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[15][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[15][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[16][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[16][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[16][2], 5.0000e-01, TOLERANCE);
    checkClose(extract->coordinates[17][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[17][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[17][2], 5.0000e-01, TOLERANCE);
    checkClose(extract->coordinates[18][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[18][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[18][2], 5.0000e-01, TOLERANCE);
    checkClose(extract->coordinates[19][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[19][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[19][2], 5.0000e-01, TOLERANCE);
    checkClose(extract->coordinates[20][0], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[20][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[20][2], 5.0000e-01, TOLERANCE);
    checkClose(extract->coordinates[21][0], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[21][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[21][2], 5.0000e-01, TOLERANCE);
    checkClose(extract->coordinates[22][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[22][1], 2.5000e-01, TOLERANCE); checkClose(extract->coordinates[22][2], 5.0000e-01, TOLERANCE);
    checkClose(extract->coordinates[23][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[23][1], 7.5000e-01, TOLERANCE); checkClose(extract->coordinates[23][2], 5.0000e-01, TOLERANCE);
    checkClose(extract->coordinates[24][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[24][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[24][2], 2.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[25][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[25][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[25][2], 7.5000e-01, TOLERANCE);
    checkClose(extract->coordinates[26][0], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[26][1], 5.0000e-01, TOLERANCE); checkClose(extract->coordinates[26][2], 5.0000e-01, TOLERANCE);

    auto hexahedra = extract->hexahedrons;

    checkEqual(hexahedra[0][0], 20); checkEqual(hexahedra[0][1], 19); checkEqual(hexahedra[0][2],  7); checkEqual(hexahedra[0][3], 12);
    checkEqual(hexahedra[1][0], 26); checkEqual(hexahedra[1][1], 23); checkEqual(hexahedra[1][2], 15); checkEqual(hexahedra[1][3], 25);
    checkEqual(hexahedra[2][0], 18); checkEqual(hexahedra[2][1], 20); checkEqual(hexahedra[2][2], 12); checkEqual(hexahedra[2][3],  4);
    checkEqual(hexahedra[3][0], 22); checkEqual(hexahedra[3][1], 26); checkEqual(hexahedra[3][2], 25); checkEqual(hexahedra[3][3], 13);
    checkEqual(hexahedra[4][0],  8); checkEqual(hexahedra[4][1],  3); checkEqual(hexahedra[4][2], 19); checkEqual(hexahedra[4][3], 20);
    checkEqual(hexahedra[5][0], 24); checkEqual(hexahedra[5][1], 11); checkEqual(hexahedra[5][2], 23); checkEqual(hexahedra[5][3], 26);
    checkEqual(hexahedra[6][0],  0); checkEqual(hexahedra[6][1],  8); checkEqual(hexahedra[6][2], 20); checkEqual(hexahedra[6][3], 18);
    checkEqual(hexahedra[7][0],  9); checkEqual(hexahedra[7][1], 24); checkEqual(hexahedra[7][2], 26); checkEqual(hexahedra[7][3], 22);

    checkEqual(hexahedra[0][4], 26); checkEqual(hexahedra[0][5], 23); checkEqual(hexahedra[0][6], 15); checkEqual(hexahedra[0][7], 25);
    checkEqual(hexahedra[1][4], 21); checkEqual(hexahedra[1][5], 17); checkEqual(hexahedra[1][6],  6); checkEqual(hexahedra[1][7], 14);
    checkEqual(hexahedra[2][4], 22); checkEqual(hexahedra[2][5], 26); checkEqual(hexahedra[2][6], 25); checkEqual(hexahedra[2][7], 13);
    checkEqual(hexahedra[3][4], 16); checkEqual(hexahedra[3][5], 21); checkEqual(hexahedra[3][6], 14); checkEqual(hexahedra[3][7],  5);
    checkEqual(hexahedra[4][4], 24); checkEqual(hexahedra[4][5], 11); checkEqual(hexahedra[4][6], 23); checkEqual(hexahedra[4][7], 26);
    checkEqual(hexahedra[5][4], 10); checkEqual(hexahedra[5][5],  2); checkEqual(hexahedra[5][6], 17); checkEqual(hexahedra[5][7], 21);
    checkEqual(hexahedra[6][4],  9); checkEqual(hexahedra[6][5], 24); checkEqual(hexahedra[6][6], 26); checkEqual(hexahedra[6][7], 22);
    checkEqual(hexahedra[7][4],  1); checkEqual(hexahedra[7][5], 10); checkEqual(hexahedra[7][6], 21); checkEqual(hexahedra[7][7], 16);

    checkEqual(hexahedra[0][8], 0);
    checkEqual(hexahedra[1][8], 1);
    checkEqual(hexahedra[2][8], 2);
    checkEqual(hexahedra[3][8], 3);
    checkEqual(hexahedra[4][8], 4);
    checkEqual(hexahedra[5][8], 5);
    checkEqual(hexahedra[6][8], 6);
    checkEqual(hexahedra[7][8], 7);

    auto quadrangles = extract->quadrangles;

    checkEqual(quadrangles[ 0][0],  7); checkEqual(quadrangles[ 0][1], 19); checkEqual(quadrangles[ 0][2], 20); checkEqual(quadrangles[ 0][3], 12); checkEqual(quadrangles[ 0][4],  8);
    checkEqual(quadrangles[ 1][0], 12); checkEqual(quadrangles[ 1][1], 20); checkEqual(quadrangles[ 1][2], 18); checkEqual(quadrangles[ 1][3],  4); checkEqual(quadrangles[ 1][4],  9);
    checkEqual(quadrangles[ 2][0], 19); checkEqual(quadrangles[ 2][1],  3); checkEqual(quadrangles[ 2][2],  8); checkEqual(quadrangles[ 2][3], 20); checkEqual(quadrangles[ 2][4], 10);
    checkEqual(quadrangles[ 3][0], 20); checkEqual(quadrangles[ 3][1],  8); checkEqual(quadrangles[ 3][2],  0); checkEqual(quadrangles[ 3][3], 18); checkEqual(quadrangles[ 3][4], 11);
    checkEqual(quadrangles[ 4][0],  2); checkEqual(quadrangles[ 4][1], 17); checkEqual(quadrangles[ 4][2], 21); checkEqual(quadrangles[ 4][3], 10); checkEqual(quadrangles[ 4][4], 12);
    checkEqual(quadrangles[ 5][0], 10); checkEqual(quadrangles[ 5][1], 21); checkEqual(quadrangles[ 5][2], 16); checkEqual(quadrangles[ 5][3],  1); checkEqual(quadrangles[ 5][4], 13);
    checkEqual(quadrangles[ 6][0], 17); checkEqual(quadrangles[ 6][1],  6); checkEqual(quadrangles[ 6][2], 14); checkEqual(quadrangles[ 6][3], 21); checkEqual(quadrangles[ 6][4], 14);
    checkEqual(quadrangles[ 7][0], 21); checkEqual(quadrangles[ 7][1], 14); checkEqual(quadrangles[ 7][2],  5); checkEqual(quadrangles[ 7][3], 16); checkEqual(quadrangles[ 7][4], 15);
    checkEqual(quadrangles[ 8][0],  4); checkEqual(quadrangles[ 8][1], 18); checkEqual(quadrangles[ 8][2], 22); checkEqual(quadrangles[ 8][3], 13); checkEqual(quadrangles[ 8][4], 16);
    checkEqual(quadrangles[ 9][0], 18); checkEqual(quadrangles[ 9][1],  0); checkEqual(quadrangles[ 9][2],  9); checkEqual(quadrangles[ 9][3], 22); checkEqual(quadrangles[ 9][4], 17);
    checkEqual(quadrangles[10][0], 13); checkEqual(quadrangles[10][1], 22); checkEqual(quadrangles[10][2], 16); checkEqual(quadrangles[10][3],  5); checkEqual(quadrangles[10][4], 18);
    checkEqual(quadrangles[11][0], 22); checkEqual(quadrangles[11][1],  9); checkEqual(quadrangles[11][2],  1); checkEqual(quadrangles[11][3], 16); checkEqual(quadrangles[11][4], 19);
    checkEqual(quadrangles[12][0],  6); checkEqual(quadrangles[12][1], 17); checkEqual(quadrangles[12][2], 23); checkEqual(quadrangles[12][3], 15); checkEqual(quadrangles[12][4], 20);
    checkEqual(quadrangles[13][0], 17); checkEqual(quadrangles[13][1],  2); checkEqual(quadrangles[13][2], 11); checkEqual(quadrangles[13][3], 23); checkEqual(quadrangles[13][4], 21);
    checkEqual(quadrangles[14][0], 15); checkEqual(quadrangles[14][1], 23); checkEqual(quadrangles[14][2], 19); checkEqual(quadrangles[14][3],  7); checkEqual(quadrangles[14][4], 22);
    checkEqual(quadrangles[15][0], 23); checkEqual(quadrangles[15][1], 11); checkEqual(quadrangles[15][2],  3); checkEqual(quadrangles[15][3], 19); checkEqual(quadrangles[15][4], 23);
    checkEqual(quadrangles[16][0],  0); checkEqual(quadrangles[16][1],  8); checkEqual(quadrangles[16][2], 24); checkEqual(quadrangles[16][3],  9); checkEqual(quadrangles[16][4], 24);
    checkEqual(quadrangles[17][0],  8); checkEqual(quadrangles[17][1],  3); checkEqual(quadrangles[17][2], 11); checkEqual(quadrangles[17][3], 24); checkEqual(quadrangles[17][4], 25);
    checkEqual(quadrangles[18][0],  9); checkEqual(quadrangles[18][1], 24); checkEqual(quadrangles[18][2], 10); checkEqual(quadrangles[18][3],  1); checkEqual(quadrangles[18][4], 26);
    checkEqual(quadrangles[19][0], 24); checkEqual(quadrangles[19][1], 11); checkEqual(quadrangles[19][2],  2); checkEqual(quadrangles[19][3], 10); checkEqual(quadrangles[19][4], 27);
    checkEqual(quadrangles[20][0],  7); checkEqual(quadrangles[20][1], 12); checkEqual(quadrangles[20][2], 25); checkEqual(quadrangles[20][3], 15); checkEqual(quadrangles[20][4], 28);
    checkEqual(quadrangles[21][0], 15); checkEqual(quadrangles[21][1], 25); checkEqual(quadrangles[21][2], 14); checkEqual(quadrangles[21][3],  6); checkEqual(quadrangles[21][4], 29);
    checkEqual(quadrangles[22][0], 12); checkEqual(quadrangles[22][1],  4); checkEqual(quadrangles[22][2], 13); checkEqual(quadrangles[22][3], 25); checkEqual(quadrangles[22][4], 30);
    checkEqual(quadrangles[23][0], 25); checkEqual(quadrangles[23][1], 13); checkEqual(quadrangles[23][2],  5); checkEqual(quadrangles[23][3], 14); checkEqual(quadrangles[23][4], 31);
}

TestSuiteEnd()

struct GridDataExtractorWithWellFixture {
    GridDataExtractorWithWellFixture() {
        CgnsReader3D cgnsReader3D(this->inputPath);
        this->gridData = cgnsReader3D.gridData;
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractorWithWell/12523v_57072e.cgns";
    boost::shared_ptr<GridData> gridData;
    std::string gridDataExtractorScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractorWithWell/ScriptGridDataExtractor.json";
};

FixtureTestSuite(GridDataExtractorWithWellSuite, GridDataExtractorWithWellFixture)

TestCase(GridDataExtractorWithWellTest) {
    GridDataExtractor gridDataExtractor(this->gridData, this->gridDataExtractorScript);

    boost::shared_ptr<GridData> extract = gridDataExtractor.extract;

    checkEqual(extract->coordinates.size(), 2886u);

    checkEqual(extract->tetrahedrons.size(), 0u);
    checkEqual(extract->hexahedrons.size(),  1848u);
    checkEqual(extract->prisms.size(),   924u);
    checkEqual(extract->pyramids.size(), 0u);

    checkEqual(extract->triangles.size(), 0u);
    checkEqual(extract->quadrangles.size(), 0u);

    checkEqual(extract->lines.size(), 77u);

    checkEqual(extract->boundaries.size(), 0u);
    checkEqual(extract->regions.size(), 1u);
    checkEqual(extract->wells.size(), 1u);

    checkEqual(extract->regions[0].begin,    0);
    checkEqual(extract->regions[0].end, 2772);
}

TestSuiteEnd()
