#include "MSHtoCGNS/BoostInterface/Test.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsWriter.hpp"
#include <cgnslib.h>

#define TOLERANCE 1e-12

struct CgnsWriterFixture {
    CgnsWriterFixture() {
        this->inputFile = std::string(TEST_INPUT_DIRECTORY) + "CgnsInterface/2D-Region1-ElementType1/9v_4e.cgns";
        this->outputFile = "./Results.cgns";
        boost::filesystem::copy_file(this->inputFile, this->outputFile, boost::filesystem::copy_option::overwrite_if_exists);

        this->numberOfVertices = 9;
        this->timePace = 0.1;
        this->timeInstant = 0.0;
        this->temperature = std::vector<double>(this->numberOfVertices);
        this->pressure = std::vector<double>(this->numberOfVertices);

        for (int j = 0; j < this->numberOfVertices; ++j) {
            this->temperature[j] = double(j);
            this->pressure[j] = double(j+1);
        }
    }

    void advanceTime() {
        this->timeInstant += timePace;
        for (int j = 0; j < this->numberOfVertices; ++j) {
            this->temperature[j]++;
            this->pressure[j]++;
        }
    }

    ~CgnsWriterFixture() {
        boost::filesystem::remove_all(this->outputFile);
    }

    std::string inputFile, outputFile;
    int numberOfVertices;
    double timePace;
    double timeInstant;
    std::vector<double> temperature;
    std::vector<double> pressure;

    int fileIndex, baseIndex = 1, zoneIndex = 1;
    char buffer[500];
    GridLocation_t location;
    DataType_t datatype;
    int range_min = 1, range_max = 9;
    double field[9];
};

FixtureTestSuite(CgnsWriterSuite, CgnsWriterFixture)

TestCase(CgnsWriterTest) {
    CgnsWriter cgnsWriter(this->outputFile, "Vertex");
    cgnsWriter.writeTransientSolution(this->timeInstant);
    cgnsWriter.writeTransientField(this->temperature, "temperature");
    cgnsWriter.writeTransientField(this->pressure, "pressure");

    this->advanceTime();

    cgnsWriter.writeTransientSolution(this->timeInstant);
    cgnsWriter.writeTransientField(this->temperature, "temperature");
    cgnsWriter.writeTransientField(this->pressure, "pressure");

    cgnsWriter.finalizeTransient();

    cg_open(this->outputFile.c_str(), CG_MODE_READ, &this->fileIndex);
    int numberOfSolutions;
    cg_nsols(this->fileIndex, this->baseIndex, this->zoneIndex, &numberOfSolutions);
    checkEqual(numberOfSolutions, 2);

    int solutionNumber = 1;
    cg_sol_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, this->buffer, &this->location);
    checkEqual(this->buffer, "TimeStep1");
    check(location == Vertex);

    int numberOfFields;
    cg_nfields(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, &numberOfFields);
    checkEqual(numberOfFields, 2);

    cg_field_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, 1, &this->datatype, this->buffer);
    checkEqual(this->buffer, "temperature");
    cg_field_read(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, this->buffer, this->datatype, &this->range_min, &this->range_max, this->field);
    for (int j = 0; j < this->numberOfVertices; ++j)
        checkClose(field[j], double(j), TOLERANCE);

    cg_field_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, 2, &this->datatype, this->buffer);
    checkEqual(this->buffer, "pressure");
    cg_field_read(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, this->buffer, this->datatype, &this->range_min, &this->range_max, this->field);
    for (int j = 0; j < this->numberOfVertices; ++j)
        checkClose(field[j], double(j+1), TOLERANCE);

    solutionNumber = 2;
    cg_sol_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, this->buffer, &this->location);
    checkEqual(this->buffer, "TimeStep2");
    check(location == Vertex);

    cg_nfields(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, &numberOfFields);
    checkEqual(numberOfFields, 2);

    cg_field_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, 1, &this->datatype, this->buffer);
    checkEqual(this->buffer, "temperature");
    cg_field_read(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, this->buffer, this->datatype, &this->range_min, &this->range_max, this->field);
    for (int j = 0; j < this->numberOfVertices; ++j)
        checkClose(field[j], double(j+1), TOLERANCE);

    cg_field_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, 2, &this->datatype, this->buffer);
    checkEqual(this->buffer, "pressure");
    cg_field_read(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, this->buffer, this->datatype, &this->range_min, &this->range_max, this->field);
    for (int j = 0; j < this->numberOfVertices; ++j)
        checkClose(field[j], double(j+2), TOLERANCE);
}

TestSuiteEnd()
