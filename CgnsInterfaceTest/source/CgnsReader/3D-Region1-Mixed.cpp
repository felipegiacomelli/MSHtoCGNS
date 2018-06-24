#include <BoostInterface/Test.hpp>
#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>

#define TOLERANCE 1e-12

struct Region1_Mixed_3D_Cgns {
	Region1_Mixed_3D_Cgns() {
		CgnsReader3D cgnsReader3D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Mixed/12523v_57072e.cgns");
		this->gridData = cgnsReader3D.gridData;
	}

	~Region1_Mixed_3D_Cgns() = default;

	GridDataShared gridData;
};

FixtureTestSuite(ReadCgns_Region1_Mixed_3D, Region1_Mixed_3D_Cgns)

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

// TestCase(ElementType_t_ENUM) {
//	std::cout << std::endl;
//	std::cout << std::endl;
//	std::cout << "ElementTypeNull: " << ElementTypeNull << std::endl;
// 	std::cout << "ElementTypeUserDefined: " << ElementTypeUserDefined << std::endl;
// 	std::cout << "NODE: " << NODE << std::endl;
// 	std::cout << "BAR_2: " << BAR_2 << std::endl;
// 	std::cout << "BAR_3: " << BAR_3 << std::endl;
// 	std::cout << "TRI_3: " << TRI_3 << std::endl;
// 	std::cout << "TRI_6: " << TRI_6 << std::endl;
// 	std::cout << "QUAD_4: " << QUAD_4 << std::endl;
// 	std::cout << "QUAD_8: " << QUAD_8 << std::endl;
// 	std::cout << "QUAD_9: " << QUAD_9 << std::endl;
// 	std::cout << "TETRA_4: " << TETRA_4 << std::endl;
// 	std::cout << "TETRA_10: " << TETRA_10 << std::endl;
// 	std::cout << "PYRA_5: " << PYRA_5 << std::endl;
// 	std::cout << "PYRA_14: " << PYRA_14 << std::endl;
// 	std::cout << "PENTA_6: " << PENTA_6 << std::endl;
// 	std::cout << "PENTA_15: " << PENTA_15 << std::endl;
// 	std::cout << "PENTA_18: " << PENTA_18 << std::endl;
// 	std::cout << "HEXA_8: " << HEXA_8 << std::endl;
// 	std::cout << "HEXA_20: " << HEXA_20 << std::endl;
// 	std::cout << "HEXA_27: " << HEXA_27 << std::endl;
// 	std::cout << "MIXED: " << MIXED << std::endl;
// 	std::cout << "PYRA_13: " << PYRA_13 << std::endl;
// 	std::cout << "NGON_n: " << NGON_n << std::endl;
// 	std::cout << "NFACE_n: " << NFACE_n << std::endl;
// 	std::cout << "BAR_4: " << BAR_4 << std::endl;
// 	std::cout << "TRI_9: " << TRI_9 << std::endl;
// 	std::cout << "TRI_10: " << TRI_10 << std::endl;
// 	std::cout << "QUAD_12: " << QUAD_12 << std::endl;
// 	std::cout << "QUAD_16: " << QUAD_16 << std::endl;
// 	std::cout << "TETRA_16: " << TETRA_16 << std::endl;
// 	std::cout << "TETRA_20: " << TETRA_20 << std::endl;
// 	std::cout << "PYRA_21: " << PYRA_21 << std::endl;
// 	std::cout << "PYRA_29: " << PYRA_29 << std::endl;
// 	std::cout << "PYRA_30: " << PYRA_30 << std::endl;
// 	std::cout << "PENTA_24: " << PENTA_24 << std::endl;
// 	std::cout << "PENTA_38: " << PENTA_38 << std::endl;
// 	std::cout << "PENTA_40: " << PENTA_40 << std::endl;
// 	std::cout << "HEXA_32: " << HEXA_32 << std::endl;
// 	std::cout << "HEXA_56: " << HEXA_56 << std::endl;
// 	std::cout << "HEXA_64: " << HEXA_64 << std::endl;
// 	std::cout << "BAR_5: " << BAR_5 << std::endl;
// 	std::cout << "TRI_12: " << TRI_12 << std::endl;
// 	std::cout << "TRI_15: " << TRI_15 << std::endl;
// 	std::cout << "QUAD_P4_16: " << QUAD_P4_16 << std::endl;
// 	std::cout << "QUAD_25: " << QUAD_25 << std::endl;
// 	std::cout << "TETRA_22: " << TETRA_22 << std::endl;
// 	std::cout << "TETRA_34: " << TETRA_34 << std::endl;
// 	std::cout << "TETRA_35: " << TETRA_35 << std::endl;
// 	std::cout << "PYRA_P4_29: " << PYRA_P4_29 << std::endl;
// 	std::cout << "PYRA_50: " << PYRA_50 << std::endl;
// 	std::cout << "PYRA_55: " << PYRA_55 << std::endl;
// 	std::cout << "PENTA_33: " << PENTA_33 << std::endl;
// 	std::cout << "PENTA_66: " << PENTA_66 << std::endl;
// 	std::cout << "PENTA_75: " << PENTA_75 << std::endl;
// 	std::cout << "HEXA_44: " << HEXA_44 << std::endl;
// 	std::cout << "HEXA_98: " << HEXA_98 << std::endl;
// 	std::cout << "HEXA_125: " << HEXA_125 << std::endl;
// }

TestSuiteEnd()