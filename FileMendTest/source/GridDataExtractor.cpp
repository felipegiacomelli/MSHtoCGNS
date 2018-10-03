#include <BoostInterface/Test.hpp>
#include <IO/MshReader/MshReader3D.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <FileMend/GridDataExtractor.hpp>

#define TOLERANCE 1e-6

struct GridDataExtractorFixture {
	GridDataExtractorFixture() {
		MshReader3D reader(this->inputPath);
		this->gridData = reader.gridData;
	}

	std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractor/4x4x4_2x2x2.msh";
	GridDataShared gridData;
	std::string wellGeneratorScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractor/ScriptGridDataExtractor.json";
};

FixtureTestSuite(GridDataExtractorSuite, GridDataExtractorFixture)

TestCase(GridDataExtractorTest) {
	GridDataExtractor gridDataExtractor(this->gridData, this->wellGeneratorScript);

	checkEqual(this->gridData->coordinates.size(), 152u);

	checkEqual(this->gridData->tetrahedronConnectivity.size(),  0u);
	checkEqual(this->gridData->hexahedronConnectivity.size() , 72u);
	checkEqual(this->gridData->prismConnectivity.size()      ,  0u);
	checkEqual(this->gridData->pyramidConnectivity.size()    ,  0u);

	checkEqual(this->gridData->triangleConnectivity.size()  ,  0u);
	checkEqual(this->gridData->quadrangleConnectivity.size(), 96u);

	checkEqual(this->gridData->lineConnectivity.size(), 0u);

	checkEqual(this->gridData->boundaries.size(),  6u);
	checkEqual(this->gridData->regions.size()   ,  2u);
	checkEqual(this->gridData->wells.size()     ,  0u);

	checkEqual(this->gridData->boundaries[0].facetsOnBoundary.size() , 16u);
	checkEqual(this->gridData->boundaries[0].facetsOnBoundary.front(), 72 );
	checkEqual(this->gridData->boundaries[0].facetsOnBoundary.back() , 87 );

	checkEqual(this->gridData->boundaries[1].facetsOnBoundary.size() , 16u);
	checkEqual(this->gridData->boundaries[1].facetsOnBoundary.front(), 88 );
	checkEqual(this->gridData->boundaries[1].facetsOnBoundary.back() , 103 );

	checkEqual(this->gridData->boundaries[2].facetsOnBoundary.size() ,  16u);
	checkEqual(this->gridData->boundaries[2].facetsOnBoundary.front(), 104 );
	checkEqual(this->gridData->boundaries[2].facetsOnBoundary.back() , 119 );

	checkEqual(this->gridData->boundaries[3].facetsOnBoundary.size() ,  16u);
	checkEqual(this->gridData->boundaries[3].facetsOnBoundary.front(), 120 );
	checkEqual(this->gridData->boundaries[3].facetsOnBoundary.back() , 135 );

	checkEqual(this->gridData->boundaries[4].facetsOnBoundary.size() ,  16u);
	checkEqual(this->gridData->boundaries[4].facetsOnBoundary.front(), 136 );
	checkEqual(this->gridData->boundaries[4].facetsOnBoundary.back() , 151 );

	checkEqual(this->gridData->boundaries[5].facetsOnBoundary.size() ,  16u);
	checkEqual(this->gridData->boundaries[5].facetsOnBoundary.front(), 152 );
	checkEqual(this->gridData->boundaries[5].facetsOnBoundary.back() , 167 );

	checkEqual(this->gridData->regions[0].elementsOnRegion.size() , 64u);
	checkEqual(this->gridData->regions[0].elementsOnRegion.front(),  0 );
	checkEqual(this->gridData->regions[0].elementsOnRegion.back() , 63 );

	checkEqual(this->gridData->regions[1].elementsOnRegion.size() ,  8u);
	checkEqual(this->gridData->regions[1].elementsOnRegion.front(), 64 );
	checkEqual(this->gridData->regions[1].elementsOnRegion.back() , 71 );

	GridDataShared extract = gridDataExtractor.extract;

	checkEqual(extract->coordinates.size(), 27u);

	checkEqual(extract->tetrahedronConnectivity.size(),  0u);
	checkEqual(extract->hexahedronConnectivity.size() ,  8u);
	checkEqual(extract->prismConnectivity.size()      ,  0u);
	checkEqual(extract->pyramidConnectivity.size()    ,  0u);

	checkEqual(extract->triangleConnectivity.size()  ,  0u);
	checkEqual(extract->quadrangleConnectivity.size(), 24u);

	checkEqual(extract->lineConnectivity.size(), 0u);

	checkEqual(extract->boundaries.size(),  6u);
	checkEqual(extract->regions.size()   ,  1u);
	checkEqual(extract->wells.size()     ,  0u);

	checkEqual(extract->boundaries[0].facetsOnBoundary.size() ,  4u);
	checkEqual(extract->boundaries[0].facetsOnBoundary.front(),  8 );
	checkEqual(extract->boundaries[0].facetsOnBoundary.back() , 11 );

	checkEqual(extract->boundaries[1].facetsOnBoundary.size() ,  4u);
	checkEqual(extract->boundaries[1].facetsOnBoundary.front(), 12 );
	checkEqual(extract->boundaries[1].facetsOnBoundary.back() , 15 );

	checkEqual(extract->boundaries[2].facetsOnBoundary.size() ,  4u);
	checkEqual(extract->boundaries[2].facetsOnBoundary.front(), 16 );
	checkEqual(extract->boundaries[2].facetsOnBoundary.back() , 19 );

	checkEqual(extract->boundaries[3].facetsOnBoundary.size() ,  4u);
	checkEqual(extract->boundaries[3].facetsOnBoundary.front(), 20 );
	checkEqual(extract->boundaries[3].facetsOnBoundary.back() , 23 );

	checkEqual(extract->boundaries[4].facetsOnBoundary.size() ,  4u);
	checkEqual(extract->boundaries[4].facetsOnBoundary.front(), 24 );
	checkEqual(extract->boundaries[4].facetsOnBoundary.back() , 27 );

	checkEqual(extract->boundaries[5].facetsOnBoundary.size() ,  4u);
	checkEqual(extract->boundaries[5].facetsOnBoundary.front(), 28 );
	checkEqual(extract->boundaries[5].facetsOnBoundary.back() , 31 );

	checkEqual(extract->regions[0].elementsOnRegion.size() , 8u);
	checkEqual(extract->regions[0].elementsOnRegion.front(), 0 );
	checkEqual(extract->regions[0].elementsOnRegion.back() , 7 );

	checkClose(extract->coordinates[ 0][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 0][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 0][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 1][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 1][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 1][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 2][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 2][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 2][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 3][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 3][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 3][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 4][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 4][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 4][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 5][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 5][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 5][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 6][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 6][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 6][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 7][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 7][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 7][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 8][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 8][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[ 8][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[ 9][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[ 9][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[ 9][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[10][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[10][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[10][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[11][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[11][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[11][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[12][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[12][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[12][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[13][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[13][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[13][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[14][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[14][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[14][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[15][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[15][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[15][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[16][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[16][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[16][2], 5.0000e-01, TOLERANCE);
	checkClose(extract->coordinates[17][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[17][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[17][2], 5.0000e-01, TOLERANCE);
	checkClose(extract->coordinates[18][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[18][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[18][2], 5.0000e-01, TOLERANCE);
	checkClose(extract->coordinates[19][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[19][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[19][2], 5.0000e-01, TOLERANCE);
	checkClose(extract->coordinates[20][0], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[20][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[20][2], 5.0000e-01, TOLERANCE);
	checkClose(extract->coordinates[21][0], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[21][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[21][2], 5.0000e-01, TOLERANCE);
	checkClose(extract->coordinates[22][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[22][1], 2.5000e-01, TOLERANCE);	checkClose(extract->coordinates[22][2], 5.0000e-01, TOLERANCE);
	checkClose(extract->coordinates[23][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[23][1], 7.5000e-01, TOLERANCE);	checkClose(extract->coordinates[23][2], 5.0000e-01, TOLERANCE);
	checkClose(extract->coordinates[24][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[24][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[24][2], 2.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[25][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[25][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[25][2], 7.5000e-01, TOLERANCE);
	checkClose(extract->coordinates[26][0], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[26][1], 5.0000e-01, TOLERANCE);	checkClose(extract->coordinates[26][2], 5.0000e-01, TOLERANCE);

	auto hexahedra = extract->hexahedronConnectivity;

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

	auto quadrangles = extract->quadrangleConnectivity;

	checkEqual(quadrangles[	0][0],  7); checkEqual(quadrangles[	0][1], 19); checkEqual(quadrangles[	0][2], 20); checkEqual(quadrangles[	0][3], 12); checkEqual(quadrangles[	0][4],  8);
	checkEqual(quadrangles[	1][0], 12); checkEqual(quadrangles[	1][1], 20); checkEqual(quadrangles[	1][2], 18); checkEqual(quadrangles[	1][3],  4); checkEqual(quadrangles[	1][4],  9);
	checkEqual(quadrangles[	2][0], 19); checkEqual(quadrangles[	2][1],  3); checkEqual(quadrangles[	2][2],  8); checkEqual(quadrangles[	2][3], 20); checkEqual(quadrangles[	2][4], 10);
	checkEqual(quadrangles[	3][0], 20); checkEqual(quadrangles[	3][1],  8); checkEqual(quadrangles[	3][2],  0); checkEqual(quadrangles[	3][3], 18); checkEqual(quadrangles[	3][4], 11);
	checkEqual(quadrangles[	4][0],  2); checkEqual(quadrangles[	4][1], 17); checkEqual(quadrangles[	4][2], 21); checkEqual(quadrangles[	4][3], 10); checkEqual(quadrangles[	4][4], 12);
	checkEqual(quadrangles[	5][0], 10); checkEqual(quadrangles[	5][1], 21); checkEqual(quadrangles[	5][2], 16); checkEqual(quadrangles[	5][3],  1); checkEqual(quadrangles[	5][4], 13);
	checkEqual(quadrangles[	6][0], 17); checkEqual(quadrangles[	6][1],  6); checkEqual(quadrangles[	6][2], 14); checkEqual(quadrangles[	6][3], 21); checkEqual(quadrangles[	6][4], 14);
	checkEqual(quadrangles[	7][0], 21); checkEqual(quadrangles[	7][1], 14); checkEqual(quadrangles[	7][2],  5); checkEqual(quadrangles[	7][3], 16); checkEqual(quadrangles[	7][4], 15);
	checkEqual(quadrangles[	8][0],  4); checkEqual(quadrangles[	8][1], 18); checkEqual(quadrangles[	8][2], 22); checkEqual(quadrangles[	8][3], 13); checkEqual(quadrangles[	8][4], 16);
	checkEqual(quadrangles[	9][0], 18); checkEqual(quadrangles[	9][1],  0); checkEqual(quadrangles[	9][2],  9); checkEqual(quadrangles[	9][3], 22); checkEqual(quadrangles[	9][4], 17);
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
		CgnsReader3D reader(this->inputPath);
		this->gridData = reader.gridData;
	}

	std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractorWithWell/12523v_57072e.cgns";
	GridDataShared gridData;
	std::string wellGeneratorScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/GridDataExtractorWithWell/ScriptGridDataExtractor.json";
};

FixtureTestSuite(GridDataExtractorWithWellSuite, GridDataExtractorWithWellFixture)

TestCase(GridDataExtractorWithWellTest) {
	GridDataExtractor gridDataExtractor(this->gridData, this->wellGeneratorScript);

	GridDataShared extract = gridDataExtractor.extract;

	checkEqual(extract->coordinates.size(), 2886u);

	checkEqual(extract->tetrahedronConnectivity.size(),     0u);
	checkEqual(extract->hexahedronConnectivity.size() ,  1848u);
	checkEqual(extract->prismConnectivity.size()      ,   924u);
	checkEqual(extract->pyramidConnectivity.size()    ,     0u);

	checkEqual(extract->triangleConnectivity.size()  , 0u);
	checkEqual(extract->quadrangleConnectivity.size(), 0u);

	checkEqual(extract->lineConnectivity.size(), 77u);

	checkEqual(extract->boundaries.size(),  0u);
	checkEqual(extract->regions.size()   ,  1u);
	checkEqual(extract->wells.size()     ,  1u);

	checkEqual(extract->regions[0].elementsOnRegion.size() , 2772u);
	checkEqual(extract->regions[0].elementsOnRegion.front(),    0 );
	checkEqual(extract->regions[0].elementsOnRegion.back() , 2771 );
}

TestSuiteEnd()