#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader2D.hpp"

#define TOLERANCE 1e-12

struct Region1_ElementType1_2D_Cgns {
    Region1_ElementType1_2D_Cgns() {
        CgnsReader2D cgnsReader2D(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/2D-Region1-ElementType1/9v_4e.cgns");
        this->gridData = cgnsReader2D.gridData;
    }

    ~Region1_ElementType1_2D_Cgns() = default;

    boost::shared_ptr<GridData> gridData;
};

FixtureTestSuite(ReadCgns_Region1_ElementType1_2D, Region1_ElementType1_2D_Cgns)

TestCase(Coordinates) {
    auto coordinates = this->gridData->coordinates;

    checkEqual(coordinates.size(), 9u);
    checkClose(coordinates[0][0], 0.0, TOLERANCE); checkClose(coordinates[0][1], 0.0, TOLERANCE); checkClose(coordinates[0][2], 0.0, TOLERANCE);
    checkClose(coordinates[1][0], 1.0, TOLERANCE); checkClose(coordinates[1][1], 0.0, TOLERANCE); checkClose(coordinates[1][2], 0.0, TOLERANCE);
    checkClose(coordinates[2][0], 1.0, TOLERANCE); checkClose(coordinates[2][1], 1.0, TOLERANCE); checkClose(coordinates[2][2], 0.0, TOLERANCE);
    checkClose(coordinates[3][0], 0.0, TOLERANCE); checkClose(coordinates[3][1], 1.0, TOLERANCE); checkClose(coordinates[3][2], 0.0, TOLERANCE);
    checkClose(coordinates[4][0], 0.5, TOLERANCE); checkClose(coordinates[4][1], 0.0, TOLERANCE); checkClose(coordinates[4][2], 0.0, TOLERANCE);
    checkClose(coordinates[5][0], 1.0, TOLERANCE); checkClose(coordinates[5][1], 0.5, TOLERANCE); checkClose(coordinates[5][2], 0.0, TOLERANCE);
    checkClose(coordinates[6][0], 0.5, TOLERANCE); checkClose(coordinates[6][1], 1.0, TOLERANCE); checkClose(coordinates[6][2], 0.0, TOLERANCE);
    checkClose(coordinates[7][0], 0.0, TOLERANCE); checkClose(coordinates[7][1], 0.5, TOLERANCE); checkClose(coordinates[7][2], 0.0, TOLERANCE);
    checkClose(coordinates[8][0], 0.5, TOLERANCE); checkClose(coordinates[8][1], 0.5, TOLERANCE); checkClose(coordinates[8][2], 0.0, TOLERANCE);
}

TestCase(Elements) {
    auto quadrangles = this->gridData->quadrangleConnectivity;

    checkEqual(quadrangles.size(), 4u);
    checkEqual(quadrangles[0][0], 0); checkEqual(quadrangles[0][1], 4); checkEqual(quadrangles[0][2], 8); checkEqual(quadrangles[0][3], 7); checkEqual(quadrangles[0][4], 0);
    checkEqual(quadrangles[1][0], 7); checkEqual(quadrangles[1][1], 8); checkEqual(quadrangles[1][2], 6); checkEqual(quadrangles[1][3], 3); checkEqual(quadrangles[1][4], 1);
    checkEqual(quadrangles[2][0], 4); checkEqual(quadrangles[2][1], 1); checkEqual(quadrangles[2][2], 5); checkEqual(quadrangles[2][3], 8); checkEqual(quadrangles[2][4], 2);
    checkEqual(quadrangles[3][0], 8); checkEqual(quadrangles[3][1], 5); checkEqual(quadrangles[3][2], 2); checkEqual(quadrangles[3][3], 6); checkEqual(quadrangles[3][4], 3);
}

TestCase(Facets) {
    auto lines = this->gridData->lineConnectivity;

    checkEqual(lines.size(), 8u);
    checkEqual(lines[0][0], 3); checkEqual(lines[0][1], 7); checkEqual(lines[0][2],  4);
    checkEqual(lines[1][0], 7); checkEqual(lines[1][1], 0); checkEqual(lines[1][2],  5);
    checkEqual(lines[2][0], 1); checkEqual(lines[2][1], 5); checkEqual(lines[2][2],  6);
    checkEqual(lines[3][0], 5); checkEqual(lines[3][1], 2); checkEqual(lines[3][2],  7);
    checkEqual(lines[4][0], 0); checkEqual(lines[4][1], 4); checkEqual(lines[4][2],  8);
    checkEqual(lines[5][0], 4); checkEqual(lines[5][1], 1); checkEqual(lines[5][2],  9);
    checkEqual(lines[6][0], 2); checkEqual(lines[6][1], 6); checkEqual(lines[6][2], 10);
    checkEqual(lines[7][0], 6); checkEqual(lines[7][1], 3); checkEqual(lines[7][2], 11);
}

TestCase(Regions) {
    checkEqual(this->gridData->regions.size(), 1u);
}

TestCase(Geometry) {
    auto region = this->gridData->regions[0];

    check(region.name == std::string("GEOMETRY"));

    checkEqual(region.begin, 0);
    checkEqual(region.end, 4);
}

TestCase(Boundaries) {
    checkEqual(this->gridData->boundaries.size(), 4u);
}

TestCase(West) {
    BoundaryData boundary = this->gridData->boundaries[0];

    check(boundary.name == std::string("WEST"));

    checkEqual(boundary.begin, 4);
    checkEqual(boundary.end, 6);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 3u);
    checkEqual(vertices[0], 0);
    checkEqual(vertices[1], 3);
    checkEqual(vertices[2], 7);
}

TestCase(East) {
    BoundaryData boundary = this->gridData->boundaries[1];

    check(boundary.name == std::string("EAST"));

    checkEqual(boundary.begin, 6);
    checkEqual(boundary.end, 8);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 3u);
    checkEqual(vertices[0], 1);
    checkEqual(vertices[1], 2);
    checkEqual(vertices[2], 5);
}

TestCase(South) {
    BoundaryData boundary = this->gridData->boundaries[2];

    check(boundary.name == std::string("SOUTH"));

    checkEqual(boundary.begin, 8);
    checkEqual(boundary.end, 10);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 3u);
    checkEqual(vertices[0], 0);
    checkEqual(vertices[1], 1);
    checkEqual(vertices[2], 4);
}

TestCase(North) {
    BoundaryData boundary = this->gridData->boundaries[3];

    check(boundary.name == std::string("NORTH"));

    checkEqual(boundary.begin, 10);
    checkEqual(boundary.end, 12);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 3u);
    checkEqual(vertices[0], 2);
    checkEqual(vertices[1], 3);
    checkEqual(vertices[2], 6);
}

TestSuiteEnd()
