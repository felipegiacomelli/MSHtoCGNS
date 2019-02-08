#ifndef __MSH_INTERFACE_MSH_READER_HPP__
#define __MSH_INTERFACE_MSH_READER_HPP__

#include <set>
#include <sstream>
#include <fstream>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"

class MshReader {
    public:
        MshReader(std::string filePath);

        virtual ~MshReader() = default;

        boost::shared_ptr<GridData> gridData;

        std::vector<int> entitiesTypes;

    protected:
        void checkFile();
        void readPhysicalNames();
        void readNodes();
        void readElements();
        void determinePhysicalEntitiesRange();
        virtual void addPhysicalEntities() = 0;
        void addRegion(std::string name, int begin, int end);
        void addBoundary(std::string name, int begin, int end);
        virtual void addElements() = 0;
        virtual void addFacets() = 0;
        virtual void findBoundaryVertices() = 0;
        virtual void findRegionVertices() = 0;

        std::string filePath;
        std::ifstream file;
        char buffer[1024];
        int numberOfPhysicalEntities;
        std::vector<std::vector<int>> connectivities;

        int typeIndex = 0;
        int sectionIndex = 1;
        int nodeIndex = 2;

        std::vector<int> entitiesIndices;
        std::vector<std::string> entitiesNames;

        std::vector<std::array<int, 2>> physicalEntitiesRange;

        int shift = 0;
};

#endif
