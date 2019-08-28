#ifndef __UNIT_TEST_FIXTURE_CGNS_CREATOR_HPP__
#define __UNIT_TEST_FIXTURE_CGNS_CREATOR_HPP__

#include <cgnslib.h>
#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"

#define TOLERANCE 1.0e-12

struct FixtureCgnsCreator {
    FixtureCgnsCreator(std::string path) {
        CgnsReader cgnsReader(std::string(TEST_INPUT_DIRECTORY) + path);
        CgnsCreator cgnsCreator(cgnsReader.gridData, "./");
        this->output = cgnsCreator.getFileName();
        cg_open(cgnsCreator.getFileName().c_str(), CG_MODE_READ, &this->file);
    }

    ~FixtureCgnsCreator() {
        cg_close(this->file);
        boost::filesystem::remove_all(this->output.parent_path());
    };

    boost::filesystem::path output;
    int file, base = 1, zone = 1;
    char name[1024];
    int one = 1;
    ElementType_t elementType;
    int start, end, nbndry, parentFlag;
    int size;
};

#endif
