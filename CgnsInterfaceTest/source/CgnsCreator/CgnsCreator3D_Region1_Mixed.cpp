#include <BoostInterface/Test.hpp>
#include <Grid/GridData.hpp>
#include <IO/MshReader/MshReader3D.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>
#include <CgnsInterface/CgnsCreator/CgnsCreator3D.hpp>
#include <cgnslib.h>

#define TOLERANCE 1e-12

struct Region1_Mixed_3D {
	Region1_Mixed_3D() {
		CgnsReader3D inputReader(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Mixed/12523v_57072e.cgns");
		CgnsCreator3D fileIndex3D(inputReader.gridData, "./");
		this->filePath = fileIndex3D.getFileName();
		CgnsReader3D outputReader(this->filePath);
		this->gridData = outputReader.gridData;
		cg_open(this->filePath.c_str(), CG_MODE_READ, &this->fileIndex);
	}

	~Region1_Mixed_3D() {
		cg_close(this->fileIndex);
		deleteDirectory("./12523v_57072e/");
	};

	std::string filePath;
	GridDataShared gridData;
	int fileIndex;
	char name[100];
	ElementType_t type;
	int elementStart;
	int elementEnd;
	int nbndry;
	int parent_flag;
};

FixtureTestSuite(Generate_Region1_Mixed_3D, Region1_Mixed_3D)

TestCase(Sizes) {
	checkEqual(this->gridData->coordinates.size(), 12523u);
	checkEqual(this->gridData->tetrahedronConnectivity.size(), 53352u);
	checkEqual(this->gridData->hexahedronConnectivity.size(), 1848u);
	checkEqual(this->gridData->prismConnectivity.size(), 924u);
	checkEqual(this->gridData->pyramidConnectivity.size(), 948u);
	checkEqual(this->gridData->triangleConnectivity.size(), 2862u);
	checkEqual(this->gridData->quadrangleConnectivity.size(), 24u);
	checkEqual(this->gridData->lineConnectivity.size(), 77u);
	checkEqual(this->gridData->boundaries.size(), 6u);
	checkEqual(this->gridData->regions.size(), 2u);
	checkEqual(this->gridData->wells.size(), 1u);
}

TestCase(Tetrahedra) {
	auto connectivity = this->gridData->tetrahedronConnectivity.front();
	checkEqual(connectivity[0], 12522); checkEqual(connectivity[1],  187); checkEqual(connectivity[2], 8793); checkEqual(connectivity[3], 3980);
	checkEqual(connectivity.back(), 0);

	connectivity = this->gridData->tetrahedronConnectivity.back();
	checkEqual(connectivity[0], 12150); checkEqual(connectivity[1],  10771); checkEqual(connectivity[2], 10772); checkEqual(connectivity[3], 3620);
	checkEqual(connectivity.back(), 53351);
}

TestCase(Hexahedra) {
	auto connectivity = this->gridData->hexahedronConnectivity.front();
	checkEqual(connectivity[0], 11573); checkEqual(connectivity[1], 11567); checkEqual(connectivity[2], 10039); checkEqual(connectivity[3], 10052); checkEqual(connectivity[4], 11574); checkEqual(connectivity[5], 11568); checkEqual(connectivity[6], 10041); checkEqual(connectivity[7], 10054);
	checkEqual(connectivity.back(), 54300);

	connectivity = this->gridData->hexahedronConnectivity.back();
	checkEqual(connectivity.back(), 56147);
}

TestCase(Prisms) {
	auto connectivity = this->gridData->prismConnectivity.front();
	checkEqual(connectivity[0], 9471); checkEqual(connectivity[1], 10275); checkEqual(connectivity[2], 10278); checkEqual(connectivity[3], 9484); checkEqual(connectivity[4], 10287); checkEqual(connectivity[5], 10290);
	checkEqual(connectivity.back(), 56148);

	connectivity = this->gridData->prismConnectivity.back();
	checkEqual(connectivity[0], 9770); checkEqual(connectivity[1], 10554); checkEqual(connectivity[2], 11061); checkEqual(connectivity[3], 9783); checkEqual(connectivity[4], 10566); checkEqual(connectivity[5], 11067);
	checkEqual(connectivity.back(), 57071);
}

TestCase(Pyramids) {
	auto connectivity = this->gridData->pyramidConnectivity.front();
	checkEqual(connectivity[0], 8796); checkEqual(connectivity[1], 8794); checkEqual(connectivity[2], 8793); checkEqual(connectivity[3], 8795); checkEqual(connectivity[4], 12522);
	checkEqual(connectivity.back(), 53352);

	connectivity = this->gridData->pyramidConnectivity.back();
	checkEqual(connectivity[0], 10060); checkEqual(connectivity[1], 10047); checkEqual(connectivity[2], 10044); checkEqual(connectivity[3], 10057); checkEqual(connectivity[4], 11575);
	checkEqual(connectivity.back(), 54299);
}

TestCase(Triangles) {
	auto connectivity = this->gridData->triangleConnectivity.front();
	checkEqual(connectivity[0], 8688); 	checkEqual(connectivity[1], 911); 	checkEqual(connectivity[2], 1025);
	checkEqual(connectivity.back(), 57072);

	connectivity = this->gridData->triangleConnectivity.back();
	checkEqual(connectivity[0], 8815); 	checkEqual(connectivity[1], 8893); 	checkEqual(connectivity[2], 8702);
	checkEqual(connectivity.back(), 59933);
}

TestCase(Quadrangles) {
	auto connectivity = this->gridData->quadrangleConnectivity.front();
	checkEqual(connectivity[0], 8946); 	checkEqual(connectivity[1], 8948); 	checkEqual(connectivity[2], 8698); 	checkEqual(connectivity[3], 8694);
	checkEqual(connectivity.back(), 59934);

	connectivity = this->gridData->quadrangleConnectivity.back();
	checkEqual(connectivity[0], 8811); 	checkEqual(connectivity[1], 8889); 	checkEqual(connectivity[2], 8891); 	checkEqual(connectivity[3], 8813);
	checkEqual(connectivity.back(), 59957);
}

TestCase(Lines) {
	auto connectivity = this->gridData->lineConnectivity.front();
	checkEqual(connectivity[0], 9056); checkEqual(connectivity[1], 9069);
	checkEqual(connectivity.back(), 59958);

	connectivity = this->gridData->lineConnectivity.back();
	checkEqual(connectivity[0], 9069); checkEqual(connectivity[1], 9250);
	checkEqual(connectivity.back(), 60034);
}

TestSuiteEnd()