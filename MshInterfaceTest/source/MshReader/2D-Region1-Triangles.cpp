#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/MshInterface/MshReaderFixture.hpp"

#define TOLERANCE 1e-12

struct Read_Region1_Triangles_2D_Fixture : public MshReaderFixture {
    Read_Region1_Triangles_2D_Fixture() : MshReaderFixture("MshInterface/2D-Region1-ElementType1/5v_4e.msh") {}
};

FixtureTestSuite(Read_Region1_Triangles_2D_Suite, Read_Region1_Triangles_2D_Fixture)

TestCase(Elements) {
    auto triangles = this->gridData->triangles;

    checkEqual(triangles.size(), 4u);
    checkEqual(triangles[0][0], 0); checkEqual(triangles[0][1], 1); checkEqual(triangles[0][2], 4); checkEqual(triangles[0][3], 0);
    checkEqual(triangles[1][0], 0); checkEqual(triangles[1][1], 4); checkEqual(triangles[1][2], 3); checkEqual(triangles[1][3], 1);
    checkEqual(triangles[2][0], 1); checkEqual(triangles[2][1], 2); checkEqual(triangles[2][2], 4); checkEqual(triangles[2][3], 2);
    checkEqual(triangles[3][0], 2); checkEqual(triangles[3][1], 3); checkEqual(triangles[3][2], 4); checkEqual(triangles[3][3], 3);
}

TestCase(Facets) {
    auto lines = this->gridData->lines;

    checkEqual(lines.size(), 4u);
    checkEqual(lines[0][0], 3); checkEqual(lines[0][1], 0); checkEqual(lines[0][2], 4);
    checkEqual(lines[1][0], 1); checkEqual(lines[1][1], 2); checkEqual(lines[1][2], 5);
    checkEqual(lines[2][0], 0); checkEqual(lines[2][1], 1); checkEqual(lines[2][2], 6);
    checkEqual(lines[3][0], 2); checkEqual(lines[3][1], 3); checkEqual(lines[3][2], 7);
}

TestCase(Regions) {
    checkEqual(this->gridData->regions.size(), 1u);
}

TestCase(Geometry) {
    auto region = this->gridData->regions[0];

    checkEqual(region.name, std::string("GEOMETRY"));

    checkEqual(region.begin, 0);
    checkEqual(region.end, 4);

    auto vertices = region.vertices;
    checkEqual(vertices.size(), 5u);
    checkEqual(vertices[0], 0);
    checkEqual(vertices[1], 1);
    checkEqual(vertices[2], 2);
    checkEqual(vertices[3], 3);
    checkEqual(vertices[4], 4);
}

TestCase(Boundaries) {
    checkEqual(this->gridData->boundaries.size(), 4u);
}

TestCase(West) {
    auto boundary = this->gridData->boundaries[0];

    checkEqual(boundary.name, std::string("WEST"));

    checkEqual(boundary.begin, 4);
    checkEqual(boundary.end, 5);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 2u);
    checkEqual(vertices[0], 0);
    checkEqual(vertices[1], 3);
}

TestCase(East) {
    auto boundary = this->gridData->boundaries[1];

    checkEqual(boundary.name, std::string("EAST"));

    checkEqual(boundary.begin, 5);
    checkEqual(boundary.end, 6);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 2u);
    checkEqual(vertices[0], 1);
    checkEqual(vertices[1], 2);
}

TestCase(South) {
    auto boundary = this->gridData->boundaries[2];

    checkEqual(boundary.name, std::string("SOUTH"));

    checkEqual(boundary.begin, 6);
    checkEqual(boundary.end, 7);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 2u);
    checkEqual(vertices[0], 0);
    checkEqual(vertices[1], 1);
}

TestCase(North) {
    auto boundary = this->gridData->boundaries[3];

    checkEqual(boundary.name, std::string("NORTH"));

    checkEqual(boundary.begin, 7);
    checkEqual(boundary.end, 8);

    auto vertices = boundary.vertices;
    checkEqual(vertices.size(), 2u);
    checkEqual(vertices[0], 2);
    checkEqual(vertices[1], 3);
}

TestSuiteEnd()
