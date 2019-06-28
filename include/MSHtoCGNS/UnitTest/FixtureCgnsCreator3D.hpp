#ifndef __UNIT_TEST_FIXTURE_CGNS_CREATOR_3D_HPP__
#define __UNIT_TEST_FIXTURE_CGNS_CREATOR_3D_HPP__

#include <cgnslib.h>
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"

#define TOLERANCE 1.0e-12

struct FixtureCgnsCreator3D {
    FixtureCgnsCreator3D(std::string path) {
        CgnsReader3D cgnsReader3D(std::string(TEST_INPUT_DIRECTORY) + path);
        CgnsCreator3D cgnsCreator3D(cgnsReader3D.gridData, "./");
        this->output = cgnsCreator3D.getFileName();
        cg_open(cgnsCreator3D.getFileName().c_str(), CG_MODE_READ, &this->file);
    }

    ~FixtureCgnsCreator3D() {
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
