#include <BoostInterface/Test.hpp>
#include <FileMend/CgnsReader/SpecialCgnsReader3D.hpp>
#include <FileMend/WellGenerator.hpp>

struct WellGeneratorFixture {
    WellGeneratorFixture() {
        SpecialCgnsReader3D reader(this->inputPath);
        this->gridData = reader.gridData;
    }

    std::string inputPath = std::string(TEST_INPUT_DIRECTORY) + "FileMend/WellGenerator/9821v_45841e.cgns";
    boost::shared_ptr<GridData> gridData;
    std::string wellGeneratorScript = std::string(TEST_INPUT_DIRECTORY) + "FileMend/WellGenerator/ScriptWellGenerator.json";
};

FixtureTestSuite(WellGeneratorSuite, WellGeneratorFixture)

TestCase(WellGeneratorTest) {
    checkEqual(this->gridData->wells.size(), 0u);
    checkEqual(this->gridData->lineConnectivity.size(), 0u);

    WellGenerator wellGenerator(this->gridData, this->wellGeneratorScript);

    checkEqual(this->gridData->wells.size(), 3u);

    auto lineConnectivity = this->gridData->lineConnectivity;
    checkEqual(this->gridData->lineConnectivity.size(), 37u);
    checkEqual(lineConnectivity[ 0][0], 8215);  checkEqual(lineConnectivity[ 0][1], 7858);  checkEqual(lineConnectivity[ 0][2], 50253);
    checkEqual(lineConnectivity[ 1][0], 7858);  checkEqual(lineConnectivity[ 1][1], 7865);  checkEqual(lineConnectivity[ 1][2], 50254);
    checkEqual(lineConnectivity[ 2][0], 7865);  checkEqual(lineConnectivity[ 2][1], 7872);  checkEqual(lineConnectivity[ 2][2], 50255);
    checkEqual(lineConnectivity[ 3][0], 7872);  checkEqual(lineConnectivity[ 3][1], 7879);  checkEqual(lineConnectivity[ 3][2], 50256);
    checkEqual(lineConnectivity[ 4][0], 7879);  checkEqual(lineConnectivity[ 4][1], 7886);  checkEqual(lineConnectivity[ 4][2], 50257);
    checkEqual(lineConnectivity[ 5][0], 7886);  checkEqual(lineConnectivity[ 5][1], 7893);  checkEqual(lineConnectivity[ 5][2], 50258);
    checkEqual(lineConnectivity[ 6][0], 7893);  checkEqual(lineConnectivity[ 6][1], 7900);  checkEqual(lineConnectivity[ 6][2], 50259);
    checkEqual(lineConnectivity[ 7][0], 7900);  checkEqual(lineConnectivity[ 7][1], 8213);  checkEqual(lineConnectivity[ 7][2], 50260);
    checkEqual(lineConnectivity[ 8][0], 8213);  checkEqual(lineConnectivity[ 8][1], 8214);  checkEqual(lineConnectivity[ 8][2], 50261);
    checkEqual(lineConnectivity[ 9][0], 8696);  checkEqual(lineConnectivity[ 9][1], 8691);  checkEqual(lineConnectivity[ 9][2], 50262);
    checkEqual(lineConnectivity[10][0], 8691);  checkEqual(lineConnectivity[10][1], 8692);  checkEqual(lineConnectivity[10][2], 50263);
    checkEqual(lineConnectivity[11][0], 8692);  checkEqual(lineConnectivity[11][1], 8685);  checkEqual(lineConnectivity[11][2], 50264);
    checkEqual(lineConnectivity[12][0], 8685);  checkEqual(lineConnectivity[12][1], 8686);  checkEqual(lineConnectivity[12][2], 50265);
    checkEqual(lineConnectivity[13][0], 8686);  checkEqual(lineConnectivity[13][1], 8687);  checkEqual(lineConnectivity[13][2], 50266);
    checkEqual(lineConnectivity[14][0], 8687);  checkEqual(lineConnectivity[14][1], 8688);  checkEqual(lineConnectivity[14][2], 50267);
    checkEqual(lineConnectivity[15][0], 8688);  checkEqual(lineConnectivity[15][1], 8689);  checkEqual(lineConnectivity[15][2], 50268);
    checkEqual(lineConnectivity[16][0], 8689);  checkEqual(lineConnectivity[16][1], 8690);  checkEqual(lineConnectivity[16][2], 50269);
    checkEqual(lineConnectivity[17][0], 8690);  checkEqual(lineConnectivity[17][1], 8684);  checkEqual(lineConnectivity[17][2], 50270);
    checkEqual(lineConnectivity[18][0], 8817);  checkEqual(lineConnectivity[18][1], 8818);  checkEqual(lineConnectivity[18][2], 50271);
    checkEqual(lineConnectivity[19][0], 8818);  checkEqual(lineConnectivity[19][1], 8825);  checkEqual(lineConnectivity[19][2], 50272);
    checkEqual(lineConnectivity[20][0], 8825);  checkEqual(lineConnectivity[20][1], 8832);  checkEqual(lineConnectivity[20][2], 50273);
    checkEqual(lineConnectivity[21][0], 8832);  checkEqual(lineConnectivity[21][1], 8839);  checkEqual(lineConnectivity[21][2], 50274);
    checkEqual(lineConnectivity[22][0], 8839);  checkEqual(lineConnectivity[22][1], 8846);  checkEqual(lineConnectivity[22][2], 50275);
    checkEqual(lineConnectivity[23][0], 8846);  checkEqual(lineConnectivity[23][1], 8853);  checkEqual(lineConnectivity[23][2], 50276);
    checkEqual(lineConnectivity[24][0], 8853);  checkEqual(lineConnectivity[24][1], 8860);  checkEqual(lineConnectivity[24][2], 50277);
    checkEqual(lineConnectivity[25][0], 8860);  checkEqual(lineConnectivity[25][1], 8867);  checkEqual(lineConnectivity[25][2], 50278);
    checkEqual(lineConnectivity[26][0], 8867);  checkEqual(lineConnectivity[26][1], 8874);  checkEqual(lineConnectivity[26][2], 50279);
    checkEqual(lineConnectivity[27][0], 8874);  checkEqual(lineConnectivity[27][1], 8881);  checkEqual(lineConnectivity[27][2], 50280);
    checkEqual(lineConnectivity[28][0], 8881);  checkEqual(lineConnectivity[28][1], 8888);  checkEqual(lineConnectivity[28][2], 50281);
    checkEqual(lineConnectivity[29][0], 8888);  checkEqual(lineConnectivity[29][1], 8895);  checkEqual(lineConnectivity[29][2], 50282);
    checkEqual(lineConnectivity[30][0], 8895);  checkEqual(lineConnectivity[30][1], 8902);  checkEqual(lineConnectivity[30][2], 50283);
    checkEqual(lineConnectivity[31][0], 8902);  checkEqual(lineConnectivity[31][1], 8909);  checkEqual(lineConnectivity[31][2], 50284);
    checkEqual(lineConnectivity[32][0], 8909);  checkEqual(lineConnectivity[32][1], 8916);  checkEqual(lineConnectivity[32][2], 50285);
    checkEqual(lineConnectivity[33][0], 8916);  checkEqual(lineConnectivity[33][1], 8923);  checkEqual(lineConnectivity[33][2], 50286);
    checkEqual(lineConnectivity[34][0], 8923);  checkEqual(lineConnectivity[34][1], 8930);  checkEqual(lineConnectivity[34][2], 50287);
    checkEqual(lineConnectivity[35][0], 8930);  checkEqual(lineConnectivity[35][1], 8937);  checkEqual(lineConnectivity[35][2], 50288);
    checkEqual(lineConnectivity[36][0], 8937);  checkEqual(lineConnectivity[36][1], 8944);  checkEqual(lineConnectivity[36][2], 50289);

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
