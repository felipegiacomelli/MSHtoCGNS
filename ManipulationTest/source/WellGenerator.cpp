#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/Manipulation/CgnsReader/SpecialCgnsReader.hpp"
#include "MSHtoCGNS/Manipulation/WellGenerator.hpp"

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
    checkEqual(this->gridData->wells.size(), 0u);
    checkEqual(this->gridData->lines.size(), 0u);

    WellGenerator wellGenerator(this->gridData, this->wellGeneratorScript);

    checkEqual(this->gridData->wells.size(), 3u);

    auto lines = this->gridData->lines;
    checkEqual(this->gridData->lines.size(), 37u);
    checkEqual(lines[ 0][0], 8215);  checkEqual(lines[ 0][1], 7858);  checkEqual(lines[ 0][2], 50253);
    checkEqual(lines[ 1][0], 7858);  checkEqual(lines[ 1][1], 7865);  checkEqual(lines[ 1][2], 50254);
    checkEqual(lines[ 2][0], 7865);  checkEqual(lines[ 2][1], 7872);  checkEqual(lines[ 2][2], 50255);
    checkEqual(lines[ 3][0], 7872);  checkEqual(lines[ 3][1], 7879);  checkEqual(lines[ 3][2], 50256);
    checkEqual(lines[ 4][0], 7879);  checkEqual(lines[ 4][1], 7886);  checkEqual(lines[ 4][2], 50257);
    checkEqual(lines[ 5][0], 7886);  checkEqual(lines[ 5][1], 7893);  checkEqual(lines[ 5][2], 50258);
    checkEqual(lines[ 6][0], 7893);  checkEqual(lines[ 6][1], 7900);  checkEqual(lines[ 6][2], 50259);
    checkEqual(lines[ 7][0], 7900);  checkEqual(lines[ 7][1], 8213);  checkEqual(lines[ 7][2], 50260);
    checkEqual(lines[ 8][0], 8213);  checkEqual(lines[ 8][1], 8214);  checkEqual(lines[ 8][2], 50261);
    checkEqual(lines[ 9][0], 8696);  checkEqual(lines[ 9][1], 8691);  checkEqual(lines[ 9][2], 50262);
    checkEqual(lines[10][0], 8691);  checkEqual(lines[10][1], 8692);  checkEqual(lines[10][2], 50263);
    checkEqual(lines[11][0], 8692);  checkEqual(lines[11][1], 8685);  checkEqual(lines[11][2], 50264);
    checkEqual(lines[12][0], 8685);  checkEqual(lines[12][1], 8686);  checkEqual(lines[12][2], 50265);
    checkEqual(lines[13][0], 8686);  checkEqual(lines[13][1], 8687);  checkEqual(lines[13][2], 50266);
    checkEqual(lines[14][0], 8687);  checkEqual(lines[14][1], 8688);  checkEqual(lines[14][2], 50267);
    checkEqual(lines[15][0], 8688);  checkEqual(lines[15][1], 8689);  checkEqual(lines[15][2], 50268);
    checkEqual(lines[16][0], 8689);  checkEqual(lines[16][1], 8690);  checkEqual(lines[16][2], 50269);
    checkEqual(lines[17][0], 8690);  checkEqual(lines[17][1], 8684);  checkEqual(lines[17][2], 50270);
    checkEqual(lines[18][0], 8817);  checkEqual(lines[18][1], 8818);  checkEqual(lines[18][2], 50271);
    checkEqual(lines[19][0], 8818);  checkEqual(lines[19][1], 8825);  checkEqual(lines[19][2], 50272);
    checkEqual(lines[20][0], 8825);  checkEqual(lines[20][1], 8832);  checkEqual(lines[20][2], 50273);
    checkEqual(lines[21][0], 8832);  checkEqual(lines[21][1], 8839);  checkEqual(lines[21][2], 50274);
    checkEqual(lines[22][0], 8839);  checkEqual(lines[22][1], 8846);  checkEqual(lines[22][2], 50275);
    checkEqual(lines[23][0], 8846);  checkEqual(lines[23][1], 8853);  checkEqual(lines[23][2], 50276);
    checkEqual(lines[24][0], 8853);  checkEqual(lines[24][1], 8860);  checkEqual(lines[24][2], 50277);
    checkEqual(lines[25][0], 8860);  checkEqual(lines[25][1], 8867);  checkEqual(lines[25][2], 50278);
    checkEqual(lines[26][0], 8867);  checkEqual(lines[26][1], 8874);  checkEqual(lines[26][2], 50279);
    checkEqual(lines[27][0], 8874);  checkEqual(lines[27][1], 8881);  checkEqual(lines[27][2], 50280);
    checkEqual(lines[28][0], 8881);  checkEqual(lines[28][1], 8888);  checkEqual(lines[28][2], 50281);
    checkEqual(lines[29][0], 8888);  checkEqual(lines[29][1], 8895);  checkEqual(lines[29][2], 50282);
    checkEqual(lines[30][0], 8895);  checkEqual(lines[30][1], 8902);  checkEqual(lines[30][2], 50283);
    checkEqual(lines[31][0], 8902);  checkEqual(lines[31][1], 8909);  checkEqual(lines[31][2], 50284);
    checkEqual(lines[32][0], 8909);  checkEqual(lines[32][1], 8916);  checkEqual(lines[32][2], 50285);
    checkEqual(lines[33][0], 8916);  checkEqual(lines[33][1], 8923);  checkEqual(lines[33][2], 50286);
    checkEqual(lines[34][0], 8923);  checkEqual(lines[34][1], 8930);  checkEqual(lines[34][2], 50287);
    checkEqual(lines[35][0], 8930);  checkEqual(lines[35][1], 8937);  checkEqual(lines[35][2], 50288);
    checkEqual(lines[36][0], 8937);  checkEqual(lines[36][1], 8944);  checkEqual(lines[36][2], 50289);

    auto well = this->gridData->wells[0];
    checkEqual(well.name, std::string("LINE_INJECTOR_WEST"));
    checkEqual(well.begin, 50253);
    checkEqual(well.end  , 50262);
    checkEqual(well.vertices.size(), 10u);
    checkEqual(well.vertices[0], 7858);
    checkEqual(well.vertices[1], 7865);
    checkEqual(well.vertices[2], 7872);
    checkEqual(well.vertices[3], 7879);
    checkEqual(well.vertices[4], 7886);
    checkEqual(well.vertices[5], 7893);
    checkEqual(well.vertices[6], 7900);
    checkEqual(well.vertices[7], 8213);
    checkEqual(well.vertices[8], 8214);
    checkEqual(well.vertices[9], 8215);

    well = this->gridData->wells[1];
    checkEqual(well.name, std::string("LINE_INJECTOR_EAST"));
    checkEqual(well.begin, 50262);
    checkEqual(well.end  , 50271);
    checkEqual(well.vertices.size(), 10u);
    checkEqual(well.vertices[0], 8684);
    checkEqual(well.vertices[1], 8685);
    checkEqual(well.vertices[2], 8686);
    checkEqual(well.vertices[3], 8687);
    checkEqual(well.vertices[4], 8688);
    checkEqual(well.vertices[5], 8689);
    checkEqual(well.vertices[6], 8690);
    checkEqual(well.vertices[7], 8691);
    checkEqual(well.vertices[8], 8692);
    checkEqual(well.vertices[9], 8696);

    well = this->gridData->wells[2];
    checkEqual(well.name, std::string("LINE_WELL"));
    checkEqual(well.begin, 50271);
    checkEqual(well.end  , 50290);
    checkEqual(well.vertices.size(), 20u);
    checkEqual(well.vertices[ 0], 8817);
    checkEqual(well.vertices[ 1], 8818);
    checkEqual(well.vertices[ 2], 8825);
    checkEqual(well.vertices[ 3], 8832);
    checkEqual(well.vertices[ 4], 8839);
    checkEqual(well.vertices[ 5], 8846);
    checkEqual(well.vertices[ 6], 8853);
    checkEqual(well.vertices[ 7], 8860);
    checkEqual(well.vertices[ 8], 8867);
    checkEqual(well.vertices[ 9], 8874);
    checkEqual(well.vertices[10], 8881);
    checkEqual(well.vertices[11], 8888);
    checkEqual(well.vertices[12], 8895);
    checkEqual(well.vertices[13], 8902);
    checkEqual(well.vertices[14], 8909);
    checkEqual(well.vertices[15], 8916);
    checkEqual(well.vertices[16], 8923);
    checkEqual(well.vertices[17], 8930);
    checkEqual(well.vertices[18], 8937);
    checkEqual(well.vertices[19], 8944);
}

TestSuiteEnd()
