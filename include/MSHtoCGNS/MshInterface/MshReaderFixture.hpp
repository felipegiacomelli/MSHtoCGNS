#ifndef __MSH_READER_FIXTURE_HPP__
#define __MSH_READER_FIXTURE_HPP__

#include "MSHtoCGNS/MshInterface/MshReader/MshReader2D.hpp"
#include "MSHtoCGNS/MshInterface/MshReader/MshReader3D.hpp"

struct MshReaderFixture {
    MshReaderFixture(std::string path);

    ~MshReaderFixture() = default;

    std::string path;
    boost::shared_ptr<GridData> gridData;
};

#endif
