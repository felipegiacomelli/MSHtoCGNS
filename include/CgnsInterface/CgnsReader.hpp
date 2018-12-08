#ifndef CGNS_READER_HPP
#define CGNS_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <string>
#include <set>

class CgnsReader {
    public:
        CgnsReader(std::string filePath);

        std::vector<double> readField(std::string solutionName, std::string fieldName);
        std::vector<double> readField(int solutionIndex, std::string fieldName);
        int readNumberOfTimeSteps();
        std::vector<double> readTimeInstants();

        boost::shared_ptr<GridData> gridData;

        virtual ~CgnsReader();

    protected:
        void checkFile();
        void readBase();
        void readZone();
        void readNumberOfSections();
        void readNumberOfBoundaries();
        void createGridData();
        virtual void readCoordinates() = 0;
        virtual void readSections() = 0;
        void addRegion(std::string&& name, int elementStart, int end);
        void addBoundary(std::string&& name, int elementStart, int end);
        void readBoundaryConditions();
        int readSolutionIndex(std::string solutionName);

        std::string filePath;
        char buffer[800];
        int fileIndex, baseIndex, zoneIndex, cellDimension, physicalDimension;
        int sizes[3];
        int numberOfSections, numberOfBoundaries;
};

#endif
