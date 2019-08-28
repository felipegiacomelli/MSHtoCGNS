#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsCreator.hpp"

struct FixtureMixed2D : public FixtureCgnsCreator {
    FixtureMixed2D() : FixtureCgnsCreator("CgnsInterface/2D-Region2-ElementType2/11v_10e.cgns") {}
};

FixtureTestSuite(CgnsCreator_Mixed2D, FixtureMixed2D)

TestCase(Section) {
    int section = 1;
    std::vector<int> connectivities(21);
    std::vector<int> offsets(6);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "A");
    checkEqual(this->start, 1);
    checkEqual(this->end, 5);
    checkEqual(this->elementType, MIXED);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 21);
    checkEqual(cg_poly_elements_read(this->file, this->base, this->zone, section, &connectivities[0], &offsets[0], nullptr), false);
    checkEqual(connectivities[ 0], 5); checkEqual(connectivities[ 1], 4); checkEqual(connectivities[ 2],  5); checkEqual(connectivities[ 3], 10);
    checkEqual(connectivities[ 4], 5); checkEqual(connectivities[ 5], 4); checkEqual(connectivities[ 6], 10); checkEqual(connectivities[ 7],  7);
    checkEqual(connectivities[16], 7); checkEqual(connectivities[17], 5); checkEqual(connectivities[18],  6); checkEqual(connectivities[19],  9); checkEqual(connectivities[20], 8);

    section = 2;
    connectivities.resize(21);
    offsets.resize(6);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "B");
    checkEqual(this->start, 6);
    checkEqual(this->end, 10);
    checkEqual(this->elementType, MIXED);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 21);
    checkEqual(cg_poly_elements_read(this->file, this->base, this->zone, section, &connectivities[0], &offsets[0], nullptr), false);
    checkEqual(connectivities[ 0], 5); checkEqual(connectivities[ 1], 2); checkEqual(connectivities[ 2],  3); checkEqual(connectivities[ 3], 11);
    checkEqual(connectivities[ 4], 5); checkEqual(connectivities[ 5], 2); checkEqual(connectivities[ 6], 11); checkEqual(connectivities[ 7],  5);
    checkEqual(connectivities[16], 7); checkEqual(connectivities[17], 1); checkEqual(connectivities[18],  2); checkEqual(connectivities[19],  5); checkEqual(connectivities[20], 4);

    section = 5;
    connectivities.resize(4);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "SOUTH");
    checkEqual(this->start, 15);
    checkEqual(this->end, 16);
    checkEqual(this->elementType, BAR_2);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 4);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[0], 1); checkEqual(connectivities[1], 2);
    checkEqual(connectivities[2], 2); checkEqual(connectivities[3], 3);
}

TestSuiteEnd()
