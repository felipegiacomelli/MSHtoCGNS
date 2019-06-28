#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsCreator3D.hpp"

struct FixtureHexahedrons : public FixtureCgnsCreator3D {
    FixtureHexahedrons() : FixtureCgnsCreator3D("CgnsInterface/3D-Region1-Hexahedra/27v_8e.cgns") {}
};

FixtureTestSuite(CgnsCreator3D_Hexahedrons, FixtureHexahedrons)

TestCase(Section) {
    int numberOfSections;
    checkEqual(cg_nsections(this->file, this->base, this->zone, &numberOfSections), false);
    checkEqual(numberOfSections, 7);

    int section = 1;
    std::vector<int> connectivities(64);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "GEOMETRY");
    checkEqual(this->start, 1);
    checkEqual(this->end, 8);
    checkEqual(this->elementType, HEXA_8);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 64);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[ 0],  1); checkEqual(connectivities[ 1],  2); checkEqual(connectivities[ 2],  5); checkEqual(connectivities[ 3],  4);
    checkEqual(connectivities[ 4], 10); checkEqual(connectivities[ 5], 11); checkEqual(connectivities[ 6], 14); checkEqual(connectivities[ 7], 13);
    checkEqual(connectivities[56], 14); checkEqual(connectivities[57], 15); checkEqual(connectivities[58], 18); checkEqual(connectivities[59], 17);
    checkEqual(connectivities[60], 23); checkEqual(connectivities[61], 24); checkEqual(connectivities[62], 27); checkEqual(connectivities[63], 26);

    section = 3;
    connectivities.resize(16);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "EAST");
    checkEqual(this->start, 13);
    checkEqual(this->end, 16);
    checkEqual(this->elementType, QUAD_4);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 16);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[ 0],  3); checkEqual(connectivities[ 1],  6); checkEqual(connectivities[ 2], 15); checkEqual(connectivities[ 3], 12);
    checkEqual(connectivities[12], 15); checkEqual(connectivities[13], 18); checkEqual(connectivities[14], 27); checkEqual(connectivities[15], 24);

    section = 4;
    connectivities.resize(12);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "SOUTH");
    checkEqual(this->start, 17);
    checkEqual(this->end, 20);
    checkEqual(this->elementType, QUAD_4);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 16);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[ 0],  1); checkEqual(connectivities[ 1],  2); checkEqual(connectivities[ 2], 11); checkEqual(connectivities[ 3], 10);
    checkEqual(connectivities[12], 11); checkEqual(connectivities[13], 12); checkEqual(connectivities[14], 21); checkEqual(connectivities[15], 20);

    section = 6;
    connectivities.resize(16);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "BOTTOM");
    checkEqual(this->start, 25);
    checkEqual(this->end, 28);
    checkEqual(this->elementType, QUAD_4);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 16);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[ 0], 2); checkEqual(connectivities[ 1], 1); checkEqual(connectivities[ 2], 4); checkEqual(connectivities[ 3], 5);
    checkEqual(connectivities[12], 6); checkEqual(connectivities[13], 5); checkEqual(connectivities[14], 8); checkEqual(connectivities[15], 9);
}

TestSuiteEnd()
