#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureMshReader.hpp"

#define TOLERANCE 1e-12

struct Read_Region3_Hexahedrons_Well1_3D_Fixture : public FixtureMshReader {
    Read_Region3_Hexahedrons_Well1_3D_Fixture() : FixtureMshReader("MshInterface/3D-Region3-Hexahedrons-Well1/88v_30e.msh") {}
};

FixtureTestSuite(Read_Region3_Hexahedrons_Well1_3D_Suite, Read_Region3_Hexahedrons_Well1_3D_Fixture)

TestCase(Connectivities) {
    std::vector<std::vector<int>> expected{
        {17, 2, 1,   0,  3, 34, 25, 16, 43, 0},
        { 7, 1, 2,  34, 25, 30},
        { 3, 4, 3, 116}
    };

    auto connectivities = this->gridData->connectivities;
    checkEqualCollections(connectivities[  0].cbegin(), connectivities[  0].cend(), expected[0].cbegin(), expected[0].cend());
    checkEqualCollections(connectivities[ 30].cbegin(), connectivities[ 30].cend(), expected[1].cbegin(), expected[1].cend());
    checkEqualCollections(connectivities[116].cbegin(), connectivities[116].cend(), expected[2].cbegin(), expected[2].cend());
}

TestCase(Entities) {
    checkEqual(this->gridData->sections.size(), 8u);

    {
        auto section = this->gridData->sections[7];
        checkEqual(section.name, std::string("LINE"));
        checkEqual(section.dimension, 1);
        checkEqual(section.begin, 116);
        checkEqual(section.end, 117);

        std::vector<int> expected{3, 4};
        auto vertices = section.vertices;
        checkEqualCollections(vertices.cbegin(), vertices.cend(), expected.cbegin(), expected.cend());
    }
}

TestSuiteEnd()
