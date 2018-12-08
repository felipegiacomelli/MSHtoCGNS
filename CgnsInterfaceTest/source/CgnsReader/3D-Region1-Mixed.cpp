#include "BoostInterface/Test.hpp"
#include "CgnsInterface/CgnsReader/CgnsReader3D.hpp"

#define TOLERANCE 1e-12

struct Region1_Mixed_3D_Cgns {
    Region1_Mixed_3D_Cgns() {
        CgnsReader3D cgnsReader3D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/3D-Region1-Mixed/12523v_57072e.cgns");
        this->gridData = cgnsReader3D.gridData;
    }

    ~Region1_Mixed_3D_Cgns() = default;

    boost::shared_ptr<GridData> gridData;
};

FixtureTestSuite(ReadCgns_Region1_Mixed_3D, Region1_Mixed_3D_Cgns)

TestCase(CgnsReader3DTest) {
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

    auto region = this->gridData->regions[0];
    checkEqual(region.begin, 0);
    checkEqual(region.end  , 54300);

    region = this->gridData->regions[1];
    checkEqual(region.begin, 54300);
    checkEqual(region.end  , 57072);

    auto boundary = this->gridData->boundaries[0];
    checkEqual(boundary.begin, 57072);
    checkEqual(boundary.end  , 57286);
    checkEqual(boundary.vertices.size(), 135u);

    boundary = this->gridData->boundaries[1];
    checkEqual(boundary.begin, 57286);
    checkEqual(boundary.end  , 57492);
    checkEqual(boundary.vertices.size(), 131u);

    boundary = this->gridData->boundaries[2];
    checkEqual(boundary.begin, 57492);
    checkEqual(boundary.end  , 57704);
    checkEqual(boundary.vertices.size(), 134u);

    boundary = this->gridData->boundaries[3];
    checkEqual(boundary.begin, 57704);
    checkEqual(boundary.end  , 57916);
    checkEqual(boundary.vertices.size(), 134u);

    boundary = this->gridData->boundaries[4];
    checkEqual(boundary.begin, 57916);
    checkEqual(boundary.end  , 58858);
    checkEqual(boundary.vertices.size(), 516u);

    boundary = this->gridData->boundaries[5];
    checkEqual(boundary.begin, 58858);
    checkEqual(boundary.end  , 59958);
    checkEqual(boundary.vertices.size(), 607u);

    auto well = this->gridData->wells[0];
    checkEqual(well.begin, 59958);
    checkEqual(well.end  , 60035);
    checkEqual(well.vertices.size(), 78u);

    auto tetrahedron = this->gridData->tetrahedronConnectivity.front();
    checkEqual(tetrahedron[0], 12522); checkEqual(tetrahedron[1],  187); checkEqual(tetrahedron[2], 8793); checkEqual(tetrahedron[3], 3980);
    checkEqual(tetrahedron.back(), 0);

    tetrahedron = this->gridData->tetrahedronConnectivity.back();
    checkEqual(tetrahedron[0], 12150); checkEqual(tetrahedron[1],  10771); checkEqual(tetrahedron[2], 10772); checkEqual(tetrahedron[3], 3620);
    checkEqual(tetrahedron.back(), 53351);

    auto hexahedron = this->gridData->hexahedronConnectivity.front();
    checkEqual(hexahedron[0], 11573); checkEqual(hexahedron[1], 11567); checkEqual(hexahedron[2], 10039); checkEqual(hexahedron[3], 10052); checkEqual(hexahedron[4], 11574); checkEqual(hexahedron[5], 11568); checkEqual(hexahedron[6], 10041); checkEqual(hexahedron[7], 10054);
    checkEqual(hexahedron.back(), 54300);

    hexahedron = this->gridData->hexahedronConnectivity.back();
    checkEqual(hexahedron[0], 8689); checkEqual(hexahedron[1], 8690); checkEqual(hexahedron[2], 8692); checkEqual(hexahedron[3], 8691); checkEqual(hexahedron[4], 8693); checkEqual(hexahedron[5], 8694); checkEqual(hexahedron[6], 8696); checkEqual(hexahedron[7], 8695);
    checkEqual(hexahedron.back(), 56147);

    auto prism = this->gridData->prismConnectivity.front();
    checkEqual(prism[0], 9471); checkEqual(prism[1], 10275); checkEqual(prism[2], 10278); checkEqual(prism[3], 9484); checkEqual(prism[4], 10287); checkEqual(prism[5], 10290);
    checkEqual(prism.back(), 56148);

    prism = this->gridData->prismConnectivity.back();
    checkEqual(prism[0], 9770); checkEqual(prism[1], 10554); checkEqual(prism[2], 11061); checkEqual(prism[3], 9783); checkEqual(prism[4], 10566); checkEqual(prism[5], 11067);
    checkEqual(prism.back(), 57071);

    auto pyramid = this->gridData->pyramidConnectivity.front();
    checkEqual(pyramid[0], 8796); checkEqual(pyramid[1], 8794); checkEqual(pyramid[2], 8793); checkEqual(pyramid[3], 8795); checkEqual(pyramid[4], 12522);
    checkEqual(pyramid.back(), 53352);

    pyramid = this->gridData->pyramidConnectivity.back();
    checkEqual(pyramid[0], 10060); checkEqual(pyramid[1], 10047); checkEqual(pyramid[2], 10044); checkEqual(pyramid[3], 10057); checkEqual(pyramid[4], 11575);
    checkEqual(pyramid.back(), 54299);

    auto triangle = this->gridData->triangleConnectivity.front();
    checkEqual(triangle[0], 8688);  checkEqual(triangle[1], 911);   checkEqual(triangle[2], 1025);
    checkEqual(triangle.back(), 57072);

    triangle = this->gridData->triangleConnectivity.back();
    checkEqual(triangle[0], 8815);  checkEqual(triangle[1], 8893);  checkEqual(triangle[2], 8702);
    checkEqual(triangle.back(), 59933);

    auto quadrangle = this->gridData->quadrangleConnectivity.front();
    checkEqual(quadrangle[0], 8946);    checkEqual(quadrangle[1], 8948);    checkEqual(quadrangle[2], 8698);    checkEqual(quadrangle[3], 8694);
    checkEqual(quadrangle.back(), 59934);

    quadrangle = this->gridData->quadrangleConnectivity.back();
    checkEqual(quadrangle[0], 8811);    checkEqual(quadrangle[1], 8889);    checkEqual(quadrangle[2], 8891);    checkEqual(quadrangle[3], 8813);
    checkEqual(quadrangle.back(), 59957);

    auto line = this->gridData->lineConnectivity.front();
    checkEqual(line[0], 9056); checkEqual(line[1], 9069);
    checkEqual(line.back(), 59958);

    line = this->gridData->lineConnectivity.back();
    checkEqual(line[0], 9069); checkEqual(line[1], 9250);
    checkEqual(line.back(), 60034);
}

TestSuiteEnd()
