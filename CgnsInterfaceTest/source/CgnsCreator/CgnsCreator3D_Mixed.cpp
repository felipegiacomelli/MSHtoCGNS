#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsCreator3D.hpp"

struct FixtureMixed : public FixtureCgnsCreator3D {
    FixtureMixed() : FixtureCgnsCreator3D("CgnsInterface/3D-Region2-Mixed/29v_39e.cgns") {}
};

FixtureTestSuite(CgnsCreator3D_Mixed, FixtureMixed)

TestCase(Section) {
    int numberOfSections;
    checkEqual(cg_nsections(this->file, this->base, this->zone, &numberOfSections), false);
    checkEqual(numberOfSections, 9);

    int section = 1;
    std::vector<int> connectivities(177);
    std::vector<int> offsets(36);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "NONPAY");
    checkEqual(this->start, 1);
    checkEqual(this->end, 35);
    checkEqual(this->elementType, MIXED);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 177);
    checkEqual(cg_poly_elements_read(this->file, this->base, this->zone, section, &connectivities[0], &offsets[0], nullptr), false);

    checkEqual(connectivities[0], 10);
    checkEqual(connectivities[1], 15); checkEqual(connectivities[2], 16); checkEqual(connectivities[3], 12); checkEqual(connectivities[4], 26);

    checkEqual(connectivities[5], 10);
    checkEqual(connectivities[6], 16); checkEqual(connectivities[7], 23); checkEqual(connectivities[8], 12); checkEqual(connectivities[9], 26);

    checkEqual(connectivities[165], 12);
    checkEqual(connectivities[166], 3); checkEqual(connectivities[167], 5); checkEqual(connectivities[168], 12); checkEqual(connectivities[169], 10); checkEqual(connectivities[170], 15);

    checkEqual(connectivities[171], 12);
    checkEqual(connectivities[172], 5); checkEqual(connectivities[173], 7); checkEqual(connectivities[174], 14); checkEqual(connectivities[175], 12); checkEqual(connectivities[176], 16);

    checkEqual(offsets[0], 0); checkEqual(offsets[1], 5); checkEqual(offsets[33], 165); checkEqual(offsets[34], 171); checkEqual(offsets[35], 177);

    section = 2;
    connectivities.resize(32);
    offsets.resize(5);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "PAY");
    checkEqual(this->start, 36);
    checkEqual(this->end, 39);
    checkEqual(this->elementType, MIXED);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 32);
    checkEqual(cg_poly_elements_read(this->file, this->base, this->zone, section, &connectivities[0], &offsets[0], nullptr), false);

    checkEqual(connectivities[0], 17);
    checkEqual(connectivities[1], 2); checkEqual(connectivities[2],  3); checkEqual(connectivities[3],  5); checkEqual(connectivities[4],  4);
    checkEqual(connectivities[5], 9); checkEqual(connectivities[6], 10); checkEqual(connectivities[7], 12); checkEqual(connectivities[8], 11);

    checkEqual(connectivities[ 9], 17);
    checkEqual(connectivities[10],  4); checkEqual(connectivities[11],  5); checkEqual(connectivities[12],  7); checkEqual(connectivities[13],  6);
    checkEqual(connectivities[14], 11); checkEqual(connectivities[15], 12); checkEqual(connectivities[16], 14); checkEqual(connectivities[17], 13);

    checkEqual(connectivities[18], 14);
    checkEqual(connectivities[19], 2); checkEqual(connectivities[20],  4); checkEqual(connectivities[21], 1);
    checkEqual(connectivities[22], 9); checkEqual(connectivities[23], 11); checkEqual(connectivities[24], 8);

    checkEqual(connectivities[25], 14);
    checkEqual(connectivities[26],  4); checkEqual(connectivities[27],  6); checkEqual(connectivities[28], 1);
    checkEqual(connectivities[29], 11); checkEqual(connectivities[30], 13); checkEqual(connectivities[31], 8);

    checkEqual(offsets[0], 0); checkEqual(offsets[1], 9); checkEqual(offsets[2], 18); checkEqual(offsets[3], 25); checkEqual(offsets[4], 32);

    section = 7;
    connectivities.resize(22);
    offsets.resize(6);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "WEST");
    checkEqual(this->start, 71);
    checkEqual(this->end, 75);
    checkEqual(this->elementType, MIXED);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 22);
    checkEqual(cg_poly_elements_read(this->file, this->base, this->zone, section, &connectivities[0], &offsets[0], nullptr), false);

    checkEqual(connectivities[0], 7);
    checkEqual(connectivities[1], 6); checkEqual(connectivities[2], 1); checkEqual(connectivities[3], 8); checkEqual(connectivities[4], 13);

    checkEqual(connectivities[10], 5);
    checkEqual(connectivities[11], 14); checkEqual(connectivities[12], 7); checkEqual(connectivities[13], 28);

    checkEqual(offsets[0], 0); checkEqual(offsets[1], 5); checkEqual(offsets[2], 10); checkEqual(offsets[3], 14); checkEqual(offsets[4], 18); checkEqual(offsets[5], 22);

    section = 5;
    connectivities.resize(22);
    offsets.resize(5);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "SOUTH");
    checkEqual(this->start, 60);
    checkEqual(this->end, 64);
    checkEqual(this->elementType, MIXED);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 22);
    checkEqual(cg_poly_elements_read(this->file, this->base, this->zone, section, &connectivities[0], &offsets[0], nullptr), false);

    checkEqual(connectivities[0], 7);
    checkEqual(connectivities[1], 1); checkEqual(connectivities[2], 2); checkEqual(connectivities[3], 9); checkEqual(connectivities[4], 8);

    checkEqual(connectivities[10], 5);
    checkEqual(connectivities[11], 3); checkEqual(connectivities[12], 10); checkEqual(connectivities[13], 27);

    checkEqual(offsets[0], 0); checkEqual(offsets[1], 5); checkEqual(offsets[2], 10); checkEqual(offsets[3], 14); checkEqual(offsets[4], 18); checkEqual(offsets[5], 22);
}

TestSuiteEnd()
