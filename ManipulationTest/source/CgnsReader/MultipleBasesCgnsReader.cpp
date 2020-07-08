#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Manipulation/CgnsReader/MultipleBasesCgnsReader.hpp"
#include <cgnslib.h>

struct MultipleBasesCgnsReaderFixture {
    MultipleBasesCgnsReader multipleBasesCgnsReader3D = MultipleBasesCgnsReader(std::string(TEST_INPUT_DIRECTORY) +  "Manipulation/MultipleBasesCgnsReader/110v_245e.cgns");
};

FixtureTestSuite(MultipleBasesCgnsReaderSuite, MultipleBasesCgnsReaderFixture)

TestCase(Base0Case) {
    auto gridData = this->multipleBasesCgnsReader3D.read("BASE");

    checkEqual(gridData->coordinates.size(), 110u);

    {
        auto connectivities = gridData->connectivities;
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 205);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == HEXA_8 ;}),  20);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;}),  10);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PYRA_5 ;}),  10);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 124);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == QUAD_4 ;}),  20);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == BAR_2  ;}),   2);
    }

    {
        auto sections = gridData->sections;
        checkEqual(sections.size(), 3u);

        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 3;}), 1);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 1);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 1);

        {
            auto section = sections[0];
            checkEqual(section.name, std::string("BODY"));
            checkEqual(section.dimension, 3);
            checkEqual(section.begin, 0);
            checkEqual(section.end, 245);
        }

        {
            auto section = sections[1];
            checkEqual(section.name, std::string("OUTER"));
            checkEqual(section.dimension, 2);
            checkEqual(section.begin, 245);
            checkEqual(section.end, 389);
        }

        {
            auto section = sections[2];
            checkEqual(section.name, std::string("LINE"));
            checkEqual(section.dimension, 1);
            checkEqual(section.begin, 389);
            checkEqual(section.end, 391);
        }
    }
}

TestCase(Base1Case) {
    auto gridData = this->multipleBasesCgnsReader3D.read(1);

    checkEqual(gridData->coordinates.size(), 48u);

    {
        auto connectivities = gridData->connectivities;
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == HEXA_8 ;}), 20);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;}), 10);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PYRA_5 ;}), 0);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 10);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == QUAD_4 ;}), 30);
        checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == BAR_2  ;}), 2);
    }

    {
        auto sections = gridData->sections;
        checkEqual(sections.size(), 5u);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 3;}), 1);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 3);
        checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 1);

        {
            auto section = sections[0];
            checkEqual(section.name, std::string("EXTRUDED"));
            checkEqual(section.dimension, 3);
            checkEqual(section.begin, 0);
            checkEqual(section.end, 30);
        }

        {
            auto section = sections[2];
            checkEqual(section.name, std::string("LID1"));
            checkEqual(section.dimension, 2);
            checkEqual(section.begin, 40);
            checkEqual(section.end, 55);
        }

        {
            auto section = sections[4];
            checkEqual(section.name, std::string("LINE"));
            checkEqual(section.dimension, 1);
            checkEqual(section.begin, 70);
            checkEqual(section.end, 72);
        }
    }
}

TestCase(ExistsCase) {
    checkEqual(this->multipleBasesCgnsReader3D.exists("SEGMENT"), true);
    checkEqual(this->multipleBasesCgnsReader3D.exists("YELLOW"), false);
}

TestSuiteEnd()
