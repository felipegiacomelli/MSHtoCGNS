#ifndef __CGNS_INTERFACE_CGNS_READER_HPP__
#define __CGNS_INTERFACE_CGNS_READER_HPP__

#include <string>
#include <set>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"

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
        void addRegion(std::string&& name, int begin, int end);
        void addBoundary(std::string&& name, int begin, int end);
        virtual void findBoundaryVertices() = 0;
        virtual void findRegionVertices() = 0;
        int readSolutionIndex(std::string solutionName);

        boost::filesystem::path filePath;
        int fileType;
        float fileVersion;
        char buffer[1024];
        int fileIndex, baseIndex, zoneIndex, cellDimension, physicalDimension;
        int sizes[3];
        int numberOfSections, numberOfBoundaries;
};

#endif
