#ifndef __UNIT_TEST_FIXTURE_CGNS_READER_HPP__
#define __UNIT_TEST_FIXTURE_CGNS_READER_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"

#define TOLERANCE 1.0e-12

struct FixtureCgnsReader {
    FixtureCgnsReader(std::string path) {
        CgnsReader cgnsReader(std::string(TEST_INPUT_DIRECTORY) + path);
        this->gridData = cgnsReader.gridData;
    }

    boost::shared_ptr<GridData> gridData;
};

#endif
