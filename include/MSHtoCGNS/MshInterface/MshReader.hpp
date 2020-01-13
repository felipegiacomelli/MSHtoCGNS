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

        std::vector<int> sectionsDimension;

    protected:
        void checkFile();
        void readPhysicalNames();
        void readNodes();
        void readElements();
        void determinePhysicalEntitiesRange();
        void addSections();
        void addElements(std::vector<SectionData>& sections);
        void findVertices(std::vector<SectionData>& sections);

        std::string filePath;
        double version;
        std::ifstream file;
        char buffer[8192];
        int numberOfPhysicalEntities;
        std::vector<std::vector<int>> connectivities;

        int sectionIndex = 0;
        int typeIndex = 1;
        int nodeIndex = 2;

        std::vector<int> sectionsIndices;
        std::vector<std::string> sectionsNames;

        std::vector<std::array<long int, 2>> physicalEntitiesRange;

        int shift = 0;
};

#endif
