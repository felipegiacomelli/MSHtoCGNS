#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"

#define TOLERANCE 1e-12

TestCase(ReadResults) {
    CgnsReader cgnsReader(std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/2D-Region2-ElementType1-Solution/9v_6e.cgns");

    int numberOfVertices = cgnsReader.gridData->coordinates.size();
    checkEqual(numberOfVertices, 9);

    checkEqual(cgnsReader.readNumberOfTimeSteps(), 2);

    int solutionIndex = 1;
    std::vector<double> field = cgnsReader.readField(solutionIndex, "temperature");
    for (int j = 0; j < numberOfVertices; ++j)
        checkClose(field[j], double(j), TOLERANCE);

    field = cgnsReader.readField(solutionIndex, "pressure");
    for (int j = 0; j < numberOfVertices; ++j)
        checkClose(field[j], double(j+1), TOLERANCE);

    solutionIndex = 2;
    field = cgnsReader.readField(solutionIndex, "temperature");
    for (int j = 0; j < numberOfVertices; ++j)
        checkClose(field[j], double(j+2), TOLERANCE);

    field = cgnsReader.readField(solutionIndex, "pressure");
    for (int j = 0; j < numberOfVertices; ++j)
        checkClose(field[j], double(j+3), TOLERANCE);

    std::vector<double> timeInstants = cgnsReader.readTimeInstants();
    checkClose(timeInstants[0], 0.0, TOLERANCE);
    checkClose(timeInstants[1], 0.1, TOLERANCE);
}
