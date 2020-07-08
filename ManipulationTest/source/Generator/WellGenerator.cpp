#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Manipulation/CgnsReader/SpecialCgnsReader.hpp"
#include "MSHtoCGNS/Manipulation/Generator/WellGenerator.hpp"
#include <cgnslib.h>

struct WellGeneratorFixture {
    WellGeneratorFixture() {
        SpecialCgnsReader specialCgnsReader(this->inputPath);
        this->gridData = specialCgnsReader.gridData;
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/WellGenerator/9821v_45841e.cgns";
    boost::shared_ptr<GridData> gridData;
    std::string wellGeneratorScript = std::string(TEST_INPUT_DIRECTORY) + "Manipulation/WellGenerator/WellGenerator.json";
};

FixtureTestSuite(WellGeneratorSuite, WellGeneratorFixture)

TestCase(WellGeneratorTest) {
    checkEqual(std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 1;}), 0);
    checkEqual(std::count_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), [](const auto& c){return c.front() == BAR_2;}), 0);

    WellGenerator wellGenerator(this->gridData, this->wellGeneratorScript);

    checkEqual(std::count_if(this->gridData->sections.cbegin(), this->gridData->sections.cend(), [](const auto& e){return e.dimension == 1;}), 3);
    checkEqual(std::count_if(this->gridData->connectivities.cbegin(), this->gridData->connectivities.cend(), [](const auto& c){return c.front() == BAR_2;}), 37);

    {
        std::vector<std::vector<int>> expected{
            {3, 8215, 7858, 50253},
            {3, 7858, 7865, 50254},
            {3, 7865, 7872, 50255},
            {3, 7872, 7879, 50256},
            {3, 7879, 7886, 50257},
            {3, 7886, 7893, 50258},
            {3, 7893, 7900, 50259},
            {3, 7900, 8213, 50260},
            {3, 8213, 8214, 50261},
            {3, 8696, 8691, 50262},
            {3, 8691, 8692, 50263},
            {3, 8692, 8685, 50264},
            {3, 8685, 8686, 50265},
            {3, 8686, 8687, 50266},
            {3, 8687, 8688, 50267},
            {3, 8688, 8689, 50268},
            {3, 8689, 8690, 50269},
            {3, 8690, 8684, 50270},
            {3, 8817, 8818, 50271},
            {3, 8818, 8825, 50272},
            {3, 8825, 8832, 50273},
            {3, 8832, 8839, 50274},
            {3, 8839, 8846, 50275},
            {3, 8846, 8853, 50276},
            {3, 8853, 8860, 50277},
            {3, 8860, 8867, 50278},
            {3, 8867, 8874, 50279},
            {3, 8874, 8881, 50280},
            {3, 8881, 8888, 50281},
            {3, 8888, 8895, 50282},
            {3, 8895, 8902, 50283},
            {3, 8902, 8909, 50284},
            {3, 8909, 8916, 50285},
            {3, 8916, 8923, 50286},
            {3, 8923, 8930, 50287},
            {3, 8930, 8937, 50288},
            {3, 8937, 8944, 50289}
        };

        auto connectivities = this->gridData->connectivities;
        checkEqualCollections(connectivities[50253].cbegin(), connectivities[50253].cend(), expected[ 0].cbegin(), expected[ 0].cend());
        checkEqualCollections(connectivities[50254].cbegin(), connectivities[50254].cend(), expected[ 1].cbegin(), expected[ 1].cend());
        checkEqualCollections(connectivities[50255].cbegin(), connectivities[50255].cend(), expected[ 2].cbegin(), expected[ 2].cend());
        checkEqualCollections(connectivities[50256].cbegin(), connectivities[50256].cend(), expected[ 3].cbegin(), expected[ 3].cend());
        checkEqualCollections(connectivities[50257].cbegin(), connectivities[50257].cend(), expected[ 4].cbegin(), expected[ 4].cend());
        checkEqualCollections(connectivities[50258].cbegin(), connectivities[50258].cend(), expected[ 5].cbegin(), expected[ 5].cend());
        checkEqualCollections(connectivities[50259].cbegin(), connectivities[50259].cend(), expected[ 6].cbegin(), expected[ 6].cend());
        checkEqualCollections(connectivities[50260].cbegin(), connectivities[50260].cend(), expected[ 7].cbegin(), expected[ 7].cend());
        checkEqualCollections(connectivities[50261].cbegin(), connectivities[50261].cend(), expected[ 8].cbegin(), expected[ 8].cend());
        checkEqualCollections(connectivities[50262].cbegin(), connectivities[50262].cend(), expected[ 9].cbegin(), expected[ 9].cend());
        checkEqualCollections(connectivities[50263].cbegin(), connectivities[50263].cend(), expected[10].cbegin(), expected[10].cend());
        checkEqualCollections(connectivities[50264].cbegin(), connectivities[50264].cend(), expected[11].cbegin(), expected[11].cend());
        checkEqualCollections(connectivities[50265].cbegin(), connectivities[50265].cend(), expected[12].cbegin(), expected[12].cend());
        checkEqualCollections(connectivities[50266].cbegin(), connectivities[50266].cend(), expected[13].cbegin(), expected[13].cend());
        checkEqualCollections(connectivities[50267].cbegin(), connectivities[50267].cend(), expected[14].cbegin(), expected[14].cend());
        checkEqualCollections(connectivities[50268].cbegin(), connectivities[50268].cend(), expected[15].cbegin(), expected[15].cend());
        checkEqualCollections(connectivities[50269].cbegin(), connectivities[50269].cend(), expected[16].cbegin(), expected[16].cend());
        checkEqualCollections(connectivities[50270].cbegin(), connectivities[50270].cend(), expected[17].cbegin(), expected[17].cend());
        checkEqualCollections(connectivities[50271].cbegin(), connectivities[50271].cend(), expected[18].cbegin(), expected[18].cend());
        checkEqualCollections(connectivities[50272].cbegin(), connectivities[50272].cend(), expected[19].cbegin(), expected[19].cend());
        checkEqualCollections(connectivities[50273].cbegin(), connectivities[50273].cend(), expected[20].cbegin(), expected[20].cend());
        checkEqualCollections(connectivities[50274].cbegin(), connectivities[50274].cend(), expected[21].cbegin(), expected[21].cend());
        checkEqualCollections(connectivities[50275].cbegin(), connectivities[50275].cend(), expected[22].cbegin(), expected[22].cend());
        checkEqualCollections(connectivities[50276].cbegin(), connectivities[50276].cend(), expected[23].cbegin(), expected[23].cend());
        checkEqualCollections(connectivities[50277].cbegin(), connectivities[50277].cend(), expected[24].cbegin(), expected[24].cend());
        checkEqualCollections(connectivities[50278].cbegin(), connectivities[50278].cend(), expected[25].cbegin(), expected[25].cend());
        checkEqualCollections(connectivities[50279].cbegin(), connectivities[50279].cend(), expected[26].cbegin(), expected[26].cend());
        checkEqualCollections(connectivities[50280].cbegin(), connectivities[50280].cend(), expected[27].cbegin(), expected[27].cend());
        checkEqualCollections(connectivities[50281].cbegin(), connectivities[50281].cend(), expected[28].cbegin(), expected[28].cend());
        checkEqualCollections(connectivities[50282].cbegin(), connectivities[50282].cend(), expected[29].cbegin(), expected[29].cend());
        checkEqualCollections(connectivities[50283].cbegin(), connectivities[50283].cend(), expected[30].cbegin(), expected[30].cend());
        checkEqualCollections(connectivities[50284].cbegin(), connectivities[50284].cend(), expected[31].cbegin(), expected[31].cend());
        checkEqualCollections(connectivities[50285].cbegin(), connectivities[50285].cend(), expected[32].cbegin(), expected[32].cend());
        checkEqualCollections(connectivities[50286].cbegin(), connectivities[50286].cend(), expected[33].cbegin(), expected[33].cend());
        checkEqualCollections(connectivities[50287].cbegin(), connectivities[50287].cend(), expected[34].cbegin(), expected[34].cend());
        checkEqualCollections(connectivities[50288].cbegin(), connectivities[50288].cend(), expected[35].cbegin(), expected[35].cend());
        checkEqualCollections(connectivities[50289].cbegin(), connectivities[50289].cend(), expected[36].cbegin(), expected[36].cend());
    }

    {
        auto section = this->gridData->sections[8];
        checkEqual(section.name, std::string("LINE_INJECTOR_WEST"));
        checkEqual(section.dimension, 1);
        checkEqual(section.begin, 50253);
        checkEqual(section.end, 50262);
        std::vector<int> expected{7858, 7865, 7872, 7879, 7886, 7893, 7900, 8213, 8214, 8215};
        checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
    }

    {
        auto section = this->gridData->sections[9];
        checkEqual(section.name, std::string("LINE_INJECTOR_EAST"));
        checkEqual(section.dimension, 1);
        checkEqual(section.begin, 50262);
        checkEqual(section.end, 50271);
        std::vector<int> expected{8684, 8685, 8686, 8687, 8688, 8689, 8690, 8691, 8692, 8696};
        checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
    }


    {
        auto section = this->gridData->sections[10];
        checkEqual(section.name, std::string("LINE_WELL"));
        checkEqual(section.dimension, 1);
        checkEqual(section.begin, 50271);
        checkEqual(section.end, 50290);
        std::vector<int> expected{8817, 8818, 8825, 8832, 8839, 8846, 8853, 8860, 8867, 8874, 8881, 8888, 8895, 8902, 8909, 8916, 8923, 8930, 8937, 8944};
        checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
    }
}

TestSuiteEnd()
