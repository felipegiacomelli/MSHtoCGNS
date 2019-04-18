#include "MSHtoCGNS/MshInterface/MshReaderFixture.hpp"

MshReaderFixture::MshReaderFixture(std::string path) : path(std::string(TEST_INPUT_DIRECTORY) + path) {
    switch (msh::getMshGridDimension(this->path)) {
        case 2: {
            MshReader2D mshReader2D(this->path);
            this->gridData = mshReader2D.gridData;
            break;
        }
        case 3: {
            MshReader3D mshReader3D(this->path);
            this->gridData = mshReader3D.gridData;
            break;
        }
    }
}
