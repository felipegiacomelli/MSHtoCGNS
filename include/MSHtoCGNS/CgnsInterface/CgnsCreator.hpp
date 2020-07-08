#ifndef __CGNS_INTERFACE_CGNS_CREATOR_HPP__
#define __CGNS_INTERFACE_CGNS_CREATOR_HPP__

#include <numeric>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/BoostInterface/String.hpp"
#include "MSHtoCGNS/Utilities/Algorithm.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/Utilities/Error.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

class CgnsCreator {
    public:
        CgnsCreator(boost::shared_ptr<GridData> gridData, std::string outputPath, bool createInConstructor = true);

        std::string getFileName() const;

        virtual ~CgnsCreator();

        std::string baseName = "BASE";
        std::string zoneName = "ZONE";

    protected:
        void setDimensions();
        void setupFile();
        virtual void create();
        void writeBase();
        void writeZone();
        void writeCoordinates();
        void buildGlobalConnectivities();
        virtual void writeSections(std::vector<SectionData> sections);
        void setElementType(int begin, int end);
        void writeSection(int begin, int end, std::string name);
        void writePolySection(int begin, int end, std::string name, const std::vector<int>& offsets);

        boost::shared_ptr<GridData> gridData;
        boost::filesystem::path output;

        std::string fileName;
        int fileIndex, baseIndex, zoneIndex, cellDimension, physicalDimension = 3;
        int sizes[3];
        int coordinateIndex, sectionIndex, boundaryIndex;
        int elementType, elementStart = 1, elementEnd = 0;

        std::vector<std::vector<int>> global;
};

#endif
