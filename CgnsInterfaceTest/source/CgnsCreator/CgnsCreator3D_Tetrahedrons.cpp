#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/UnitTest/FixtureCgnsCreator.hpp"

struct FixtureTetrahedrons : public FixtureCgnsCreator {
    FixtureTetrahedrons() : FixtureCgnsCreator("CgnsInterface/3D-Region1-Tetrahedra/14v_24e.cgns") {}
};

FixtureTestSuite(CgnsCreator_Tetrahedrons, FixtureTetrahedrons)

TestCase(Base) {
    int numberOfBases;
    checkEqual(cg_nbases(this->file, &numberOfBases), false);
    checkEqual(numberOfBases, 1);

    int cellDimension, physicalDimension;
    checkEqual(cg_base_read(this->file, this->base, this->name, &cellDimension, &physicalDimension), false);
    checkEqual(this->name, "BASE");
    checkEqual(cellDimension, 3);
    checkEqual(physicalDimension, 3);
}

TestCase(Zone) {
    int numberOfZones;
    checkEqual(cg_nzones(this->file, this->base, &numberOfZones), false);
    checkEqual(numberOfZones, 1);

    int sizes[3];
    checkEqual(cg_zone_read(this->file, this->base, this->zone, this->name, sizes), false);
    checkEqual(this->name, "ZONE");
    checkEqual(sizes[0], 14);
    checkEqual(sizes[1], 24);
    checkEqual(sizes[2], 0);

    ZoneType_t zoneType;
    checkEqual(cg_zone_type(this->file, this->base, this->zone, &zoneType), false);
    checkEqual(zoneType, Unstructured);

    std::vector<double> coordinatesX(sizes[0]), coordinatesY(sizes[0]), coordinatesZ(sizes[0]);
    checkEqual(cg_coord_read(this->file, this->base, this->zone, "CoordinateX", RealDouble, &this->one, sizes, &coordinatesX[0]), false);
    checkEqual(cg_coord_read(this->file, this->base, this->zone, "CoordinateY", RealDouble, &this->one, sizes, &coordinatesY[0]), false);
    checkEqual(cg_coord_read(this->file, this->base, this->zone, "CoordinateZ", RealDouble, &this->one, sizes, &coordinatesZ[0]), false);

    checkClose(coordinatesX[ 0], 0.0, TOLERANCE); checkClose(coordinatesY[ 0], 0.0, TOLERANCE); checkClose(coordinatesZ[ 0], 0.0, TOLERANCE);
    checkClose(coordinatesX[ 1], 1.0, TOLERANCE); checkClose(coordinatesY[ 1], 0.0, TOLERANCE); checkClose(coordinatesZ[ 1], 0.0, TOLERANCE);
    checkClose(coordinatesX[ 2], 1.0, TOLERANCE); checkClose(coordinatesY[ 2], 1.0, TOLERANCE); checkClose(coordinatesZ[ 2], 0.0, TOLERANCE);
    checkClose(coordinatesX[ 3], 0.0, TOLERANCE); checkClose(coordinatesY[ 3], 1.0, TOLERANCE); checkClose(coordinatesZ[ 3], 0.0, TOLERANCE);
    checkClose(coordinatesX[ 4], 0.0, TOLERANCE); checkClose(coordinatesY[ 4], 0.0, TOLERANCE); checkClose(coordinatesZ[ 4], 1.0, TOLERANCE);
    checkClose(coordinatesX[ 5], 1.0, TOLERANCE); checkClose(coordinatesY[ 5], 0.0, TOLERANCE); checkClose(coordinatesZ[ 5], 1.0, TOLERANCE);
    checkClose(coordinatesX[ 6], 1.0, TOLERANCE); checkClose(coordinatesY[ 6], 1.0, TOLERANCE); checkClose(coordinatesZ[ 6], 1.0, TOLERANCE);
    checkClose(coordinatesX[ 7], 0.0, TOLERANCE); checkClose(coordinatesY[ 7], 1.0, TOLERANCE); checkClose(coordinatesZ[ 7], 1.0, TOLERANCE);
    checkClose(coordinatesX[ 8], 0.0, TOLERANCE); checkClose(coordinatesY[ 8], 0.5, TOLERANCE); checkClose(coordinatesZ[ 8], 0.5, TOLERANCE);
    checkClose(coordinatesX[ 9], 1.0, TOLERANCE); checkClose(coordinatesY[ 9], 0.5, TOLERANCE); checkClose(coordinatesZ[ 9], 0.5, TOLERANCE);
    checkClose(coordinatesX[10], 0.5, TOLERANCE); checkClose(coordinatesY[10], 0.0, TOLERANCE); checkClose(coordinatesZ[10], 0.5, TOLERANCE);
    checkClose(coordinatesX[11], 0.5, TOLERANCE); checkClose(coordinatesY[11], 1.0, TOLERANCE); checkClose(coordinatesZ[11], 0.5, TOLERANCE);
    checkClose(coordinatesX[12], 0.5, TOLERANCE); checkClose(coordinatesY[12], 0.5, TOLERANCE); checkClose(coordinatesZ[12], 0.0, TOLERANCE);
    checkClose(coordinatesX[13], 0.5, TOLERANCE); checkClose(coordinatesY[13], 0.5, TOLERANCE); checkClose(coordinatesZ[13], 1.0, TOLERANCE);
}

TestCase(Section) {
    int numberOfSections;
    checkEqual(cg_nsections(this->file, this->base, this->zone, &numberOfSections), false);
    checkEqual(numberOfSections, 7);

    int section = 1;
    std::vector<int> connectivities(96);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "GEOMETRY");
    checkEqual(this->start, 1);
    checkEqual(this->end, 24);
    checkEqual(this->elementType, TETRA_4);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 96);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[ 0], 14); checkEqual(connectivities[ 1], 10); checkEqual(connectivities[ 2], 11); checkEqual(connectivities[ 3], 13);
    checkEqual(connectivities[48], 12); checkEqual(connectivities[49],  4); checkEqual(connectivities[50],  3); checkEqual(connectivities[51], 13);
    checkEqual(connectivities[92],  1); checkEqual(connectivities[93],  9); checkEqual(connectivities[94], 11); checkEqual(connectivities[95], 13);

    section = 2;
    connectivities.resize(12);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "WEST");
    checkEqual(this->start, 25);
    checkEqual(this->end, 28);
    checkEqual(this->elementType, TRI_3);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 12);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[ 0], 1); checkEqual(connectivities[ 1], 9); checkEqual(connectivities[ 2], 4);
    checkEqual(connectivities[ 3], 1); checkEqual(connectivities[ 4], 5); checkEqual(connectivities[ 5], 9);
    checkEqual(connectivities[ 6], 4); checkEqual(connectivities[ 7], 9); checkEqual(connectivities[ 8], 8);
    checkEqual(connectivities[ 9], 5); checkEqual(connectivities[10], 8); checkEqual(connectivities[11], 9);

    section = 5;
    connectivities.resize(12);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "NORTH");
    checkEqual(this->start, 37);
    checkEqual(this->end, 40);
    checkEqual(this->elementType, TRI_3);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 12);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[ 0],  4); checkEqual(connectivities[ 1], 12); checkEqual(connectivities[ 2],  3);
    checkEqual(connectivities[ 3], 12); checkEqual(connectivities[ 4],  7); checkEqual(connectivities[ 5],  3);
    checkEqual(connectivities[ 6],  8); checkEqual(connectivities[ 7], 12); checkEqual(connectivities[ 8],  4);
    checkEqual(connectivities[ 9], 12); checkEqual(connectivities[10],  8); checkEqual(connectivities[11],  7);

    section = 7;
    connectivities.resize(12);
    checkEqual(cg_section_read(this->file, this->base, this->zone, section, this->name, &this->elementType, &this->start, &this->end, &this->nbndry, &this->parentFlag), false);
    checkEqual(this->name, "TOP");
    checkEqual(this->start, 45);
    checkEqual(this->end, 48);
    checkEqual(this->elementType, TRI_3);
    checkEqual(cg_ElementDataSize(this->file, this->base, this->zone, section, &this->size), false);
    checkEqual(this->size, 12);
    checkEqual(cg_elements_read(this->file, this->base, this->zone, section, &connectivities[0], nullptr), false);
    checkEqual(connectivities[ 0],  6); checkEqual(connectivities[ 1], 14); checkEqual(connectivities[ 2],  5);
    checkEqual(connectivities[ 3], 14); checkEqual(connectivities[ 4],  8); checkEqual(connectivities[ 5],  5);
    checkEqual(connectivities[ 6],  7); checkEqual(connectivities[ 7], 14); checkEqual(connectivities[ 8],  6);
    checkEqual(connectivities[ 9],  8); checkEqual(connectivities[10], 14); checkEqual(connectivities[11],  7);
}

TestSuiteEnd()
