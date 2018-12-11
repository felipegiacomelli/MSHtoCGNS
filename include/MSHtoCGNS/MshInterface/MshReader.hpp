#ifndef MSH_READER_HPP
#define MSH_READER_HPP

#include <set>
#include <sstream>
#include <fstream>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/Grid/GridData.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"

class MshReader {
    public:
        MshReader(std::string filePath);

        virtual ~MshReader() = default;

        boost::shared_ptr<GridData> gridData;

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
        virtual void defineBoundaryVertices() = 0;

        std::string filePath;
        std::ifstream file;
        char buffer[1024];
        int numberOfPhysicalEntities;
        std::vector<std::vector<int>> connectivities;

        int sectionIndex = 2;
        int typeIndex = 1;
        int nodeIndex = 3;

        std::vector<int> entitiesTypes;
        std::vector<int> entitiesIndices;
        std::vector<std::string> entitiesNames;

        std::vector<std::array<int, 2>> physicalEntitiesRange;

        int shift = 0;
};

#endif
