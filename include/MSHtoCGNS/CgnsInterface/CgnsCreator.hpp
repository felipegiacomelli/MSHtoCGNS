#ifndef __CGNS_INTERFACE_CGNS_CREATOR_HPP__
#define __CGNS_INTERFACE_CGNS_CREATOR_HPP__

#include <algorithm>
#include <numeric>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

class CgnsCreator {
    public:
        CgnsCreator(boost::shared_ptr<GridData> gridData, std::string outputPath);

        std::string getFileName() const;

        virtual ~CgnsCreator();

        std::string baseName = "BASE";
        std::string zoneName = "ZONE";

    protected:
        virtual void checkDimension() = 0;
        virtual void setDimensions() = 0;
        void setupFile();
        virtual void initialize();
        void writeBase();
        void writeZone();
        virtual void writeCoordinates() = 0;
        void buildGlobalConnectivities();
        virtual void writeSections();
        virtual void writeRegions() = 0;
        virtual void writeBoundaries() = 0;
        void setElementType(int begin, int end, std::unordered_map<unsigned,int> sizeType);
        void writeSection(int begin, int end, std::string name);
        void writePolySection(int begin, int end, std::string name, const std::vector<int>& offsets);

        boost::shared_ptr<GridData> gridData;
        boost::filesystem::path output;

        std::string fileName;
        int fileIndex, baseIndex, zoneIndex, cellDimension, physicalDimension;
        int sizes[3];
        int coordinateIndex, sectionIndex, boundaryIndex;
        int elementType, elementStart = 1, elementEnd = 0;

        std::vector<std::vector<int>> global;
};

#endif
