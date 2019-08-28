#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Manipulation/CgnsReader/MultipleBasesCgnsReader.hpp"

struct MultipleBasesCgnsReaderFixture {
    MultipleBasesCgnsReader multipleBasesCgnsReader3D = MultipleBasesCgnsReader(std::string(TEST_INPUT_DIRECTORY) +  "Manipulation/MultipleBasesCgnsReader/110v_245e.cgns");
};

FixtureTestSuite(MultipleBasesCgnsReaderSuite, MultipleBasesCgnsReaderFixture)

TestCase(Base1Case) {
    auto gridData = this->multipleBasesCgnsReader3D.read("BASE");

    checkEqual(gridData->coordinates.size(), 110u);
    checkEqual(gridData->tetrahedrons.size(), 205u);
    checkEqual(gridData->hexahedrons.size(), 20u);
    checkEqual(gridData->prisms.size(), 10u);
    checkEqual(gridData->pyramids.size(), 10u);
    checkEqual(gridData->quadrangles.size(), 20u);
    checkEqual(gridData->triangles.size(), 124u);
    checkEqual(gridData->lines.size(), 2u);

    checkEqual(gridData->regions.size(), 1u);
    checkEqual(gridData->boundaries.size(), 1u);
    checkEqual(gridData->wells.size(), 1u);

    checkEqual(gridData->regions[0].name, std::string("BODY"));
    checkEqual(gridData->regions[0].begin, 0);
    checkEqual(gridData->regions[0].end, 245);

    checkEqual(gridData->boundaries[0].name, std::string("OUTER"));
    checkEqual(gridData->boundaries[0].begin, 245);
    checkEqual(gridData->boundaries[0].end, 389);

    checkEqual(gridData->wells[0].name, std::string("LINE"));
    checkEqual(gridData->wells[0].begin, 389);
    checkEqual(gridData->wells[0].end, 391);
}

TestCase(Base2Case) {
    auto gridData = this->multipleBasesCgnsReader3D.read(2);

    checkEqual(gridData->coordinates.size(), 48u);
    checkEqual(gridData->tetrahedrons.size(), 0u);
    checkEqual(gridData->hexahedrons.size(), 20u);
    checkEqual(gridData->prisms.size(), 10u);
    checkEqual(gridData->pyramids.size(), 0u);
    checkEqual(gridData->quadrangles.size(), 30u);
    checkEqual(gridData->triangles.size(), 10u);
    checkEqual(gridData->lines.size(), 2u);

    checkEqual(gridData->regions.size(), 1u);
    checkEqual(gridData->boundaries.size(), 3u);
    checkEqual(gridData->wells.size(), 1u);

    checkEqual(gridData->regions[0].name, std::string("EXTRUDED"));
    checkEqual(gridData->regions[0].begin, 0);
    checkEqual(gridData->regions[0].end, 30);

    checkEqual(gridData->boundaries[0].name, std::string("OUTER"));
    checkEqual(gridData->boundaries[0].begin, 30);
    checkEqual(gridData->boundaries[0].end, 40);

    checkEqual(gridData->boundaries[1].name, std::string("LID1"));
    checkEqual(gridData->boundaries[1].begin, 40);
    checkEqual(gridData->boundaries[1].end, 55);

    checkEqual(gridData->boundaries[2].name, std::string("LID2"));
    checkEqual(gridData->boundaries[2].begin, 55);
    checkEqual(gridData->boundaries[2].end, 70);

    checkEqual(gridData->wells[0].name, std::string("LINE"));
    checkEqual(gridData->wells[0].begin, 70);
    checkEqual(gridData->wells[0].end, 72);
}

TestCase(Base3Case) {
    auto gridData = this->multipleBasesCgnsReader3D.read("SEGMENT");

    checkEqual(gridData->coordinates.size(), 32u);
    checkEqual(gridData->tetrahedrons.size(), 0u);
    checkEqual(gridData->hexahedrons.size(), 10u);
    checkEqual(gridData->prisms.size(), 5u);
    checkEqual(gridData->pyramids.size(), 0u);
    checkEqual(gridData->quadrangles.size(), 25u);
    checkEqual(gridData->triangles.size(), 10u);
    checkEqual(gridData->lines.size(), 1u);

    checkEqual(gridData->regions.size(), 1u);
    checkEqual(gridData->boundaries.size(), 3u);
    checkEqual(gridData->wells.size(), 1u);

    checkEqual(gridData->regions[0].name, std::string("EXTRUDED"));
    checkEqual(gridData->regions[0].begin, 0);
    checkEqual(gridData->regions[0].end, 15);

    checkEqual(gridData->boundaries[0].name, std::string("OUTER"));
    checkEqual(gridData->boundaries[0].begin, 15);
    checkEqual(gridData->boundaries[0].end, 20);

    checkEqual(gridData->boundaries[1].name, std::string("LID1"));
    checkEqual(gridData->boundaries[1].begin, 20);
    checkEqual(gridData->boundaries[1].end, 35);

    checkEqual(gridData->boundaries[2].name, std::string("LID2"));
    checkEqual(gridData->boundaries[2].begin, 35);
    checkEqual(gridData->boundaries[2].end, 50);

    checkEqual(gridData->wells[0].name, std::string("LINE"));
    checkEqual(gridData->wells[0].begin, 50);
    checkEqual(gridData->wells[0].end, 51);
}

TestCase(ExistsCase) {
    check(this->multipleBasesCgnsReader3D.exists("SEGMENT"));
    check(!this->multipleBasesCgnsReader3D.exists("YELLOW"));
}

TestSuiteEnd()
