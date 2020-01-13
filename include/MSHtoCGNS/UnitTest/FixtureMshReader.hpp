#ifndef __UNIT_TEST_FIXTURE_MSH_READER_HPP__
#define __UNIT_TEST_FIXTURE_MSH_READER_HPP__

#include "MSHtoCGNS/MshInterface/MshReader.hpp"

struct FixtureMshReader {
    FixtureMshReader(std::string path) {
        MshReader mshReader(std::string(TEST_INPUT_DIRECTORY) + path);
        this->gridData = mshReader.gridData;
    }

    ~FixtureMshReader() = default;

    std::string path;
    boost::shared_ptr<GridData> gridData;
};

#endif
