#ifndef CGNS_CREATOR_HPP
#define CGNS_CREATOR_HPP

#include <set>
#include <algorithm>
#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>

class CgnsCreator {
    public:
        CgnsCreator(boost::shared_ptr<GridData> gridData, std::string folderPath);

        std::string getFileName() const;

        virtual ~CgnsCreator();

    protected:
        virtual void checkDimension() = 0;
        virtual void setDimensions() = 0;
        void setupFile();
        virtual void initialize();
        void writeBase();
        void writeZone();
        virtual void writeCoordinates() = 0;
        virtual void buildGlobalConnectivities() = 0;
        virtual void writeSections();
        virtual void writeRegions() = 0;
        virtual void writeBoundaries() = 0;
        void writeBoundaryConditions();

        boost::shared_ptr<GridData> gridData;
        std::string folderPath, baseName, zoneName, fileName;
        int fileIndex, baseIndex, zoneIndex, cellDimension, physicalDimension;
        int sizes[3];
        int coordinateIndex, sectionIndex, boundaryIndex;
        int elementStart, elementEnd;

        std::vector<std::vector<int>> globalConnectivities;
};

#endif
