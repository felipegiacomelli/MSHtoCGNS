#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/FileMend/CgnsReader/SpecialCgnsReader3D.hpp"

#define TOLERANCE 1e-12

struct SpecialCgnsReader3DFixture {
    SpecialCgnsReader3DFixture() {
        SpecialCgnsReader3D specialCgnsReader3D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Mixed/12523v_57072e.cgns");
        this->gridData = specialCgnsReader3D.gridData;
    }

    boost::shared_ptr<GridData> gridData;
};

FixtureTestSuite(SpecialCgnsReader3DSuite, SpecialCgnsReader3DFixture)

TestCase(SpecialCgnsReader3DCase) {
    checkEqual(this->gridData->coordinates.size(), 12523u);
    checkEqual(this->gridData->tetrahedrons.size(), 53352u);
    checkEqual(this->gridData->hexahedrons.size(), 1848u);
    checkEqual(this->gridData->prisms.size(), 924u);
    checkEqual(this->gridData->pyramids.size(), 948u);
    checkEqual(this->gridData->triangles.size(), 2862u);
    checkEqual(this->gridData->quadrangles.size(), 24u);
    checkEqual(this->gridData->lines.size(), 0u);
    checkEqual(this->gridData->boundaries.size(), 6u);
    checkEqual(this->gridData->regions.size(), 2u);
    checkEqual(this->gridData->wells.size(), 0u);

    auto tetrahedron = this->gridData->tetrahedrons.front();
    checkEqual(tetrahedron[0], 12522); checkEqual(tetrahedron[1],  187); checkEqual(tetrahedron[2], 8793); checkEqual(tetrahedron[3], 3980);
    checkEqual(tetrahedron.back(), 0);

    tetrahedron = this->gridData->tetrahedrons.back();
    checkEqual(tetrahedron[0], 12150); checkEqual(tetrahedron[1],  10771); checkEqual(tetrahedron[2], 10772); checkEqual(tetrahedron[3], 3620);
    checkEqual(tetrahedron.back(), 53351);

    auto hexahedron = this->gridData->hexahedrons.front();
    checkEqual(hexahedron[0], 11573); checkEqual(hexahedron[1], 11567); checkEqual(hexahedron[2], 10039); checkEqual(hexahedron[3], 10052); checkEqual(hexahedron[4], 11574); checkEqual(hexahedron[5], 11568); checkEqual(hexahedron[6], 10041); checkEqual(hexahedron[7], 10054);
    checkEqual(hexahedron.back(), 54300);

    hexahedron = this->gridData->hexahedrons.back();
    checkEqual(hexahedron.back(), 56147);

    auto prism = this->gridData->prisms.front();
    checkEqual(prism[0], 9471); checkEqual(prism[1], 10275); checkEqual(prism[2], 10278); checkEqual(prism[3], 9484); checkEqual(prism[4], 10287); checkEqual(prism[5], 10290);
    checkEqual(prism.back(), 56148);

    prism = this->gridData->prisms.back();
    checkEqual(prism[0], 9770); checkEqual(prism[1], 10554); checkEqual(prism[2], 11061); checkEqual(prism[3], 9783); checkEqual(prism[4], 10566); checkEqual(prism[5], 11067);
    checkEqual(prism.back(), 57071);

    auto pyramid = this->gridData->pyramids.front();
    checkEqual(pyramid[0], 8796); checkEqual(pyramid[1], 8794); checkEqual(pyramid[2], 8793); checkEqual(pyramid[3], 8795); checkEqual(pyramid[4], 12522);
    checkEqual(pyramid.back(), 53352);

    pyramid = this->gridData->pyramids.back();
    checkEqual(pyramid[0], 10060); checkEqual(pyramid[1], 10047); checkEqual(pyramid[2], 10044); checkEqual(pyramid[3], 10057); checkEqual(pyramid[4], 11575);
    checkEqual(pyramid.back(), 54299);

    auto triangle = this->gridData->triangles.front();
    checkEqual(triangle[0], 8688);  checkEqual(triangle[1], 911);   checkEqual(triangle[2], 1025);
    checkEqual(triangle.back(), 57072);

    triangle = this->gridData->triangles.back();
    checkEqual(triangle[0], 8815);  checkEqual(triangle[1], 8893);  checkEqual(triangle[2], 8702);
    checkEqual(triangle.back(), 59933);

    auto quadrangle = this->gridData->quadrangles.front();
    checkEqual(quadrangle[0], 8946);    checkEqual(quadrangle[1], 8948);    checkEqual(quadrangle[2], 8698);    checkEqual(quadrangle[3], 8694);
    checkEqual(quadrangle.back(), 59934);

    quadrangle = this->gridData->quadrangles.back();
    checkEqual(quadrangle[0], 8811);    checkEqual(quadrangle[1], 8889);    checkEqual(quadrangle[2], 8891);    checkEqual(quadrangle[3], 8813);
    checkEqual(quadrangle.back(), 59957);
}

TestSuiteEnd()
