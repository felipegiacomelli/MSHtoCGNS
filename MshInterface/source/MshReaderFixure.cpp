#include "MSHtoCGNS/MshInterface/MshReaderFixture.hpp"

MshReaderFixture::MshReaderFixture() {
    MshReader2D mshReader2D(std::string(TEST_INPUT_DIRECTORY) + "MshInterface/2D-Region1-ElementType1/5v_4e.msh");
    this->gridData = mshReader2D.gridData;
}
