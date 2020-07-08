#ifndef __MSH_INTERFACE_MSH_READER_HPP__
#define __MSH_INTERFACE_MSH_READER_HPP__

#include <set>
#include <sstream>
#include <fstream>

#include <boost/unordered_map.hpp>

#include "MSHtoCGNS/BoostInterface/String.hpp"
#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"

class MshReader {
    public:
        MshReader(std::string filePath);

        virtual ~MshReader();

        boost::shared_ptr<GridData> gridData;

    protected:
        void checkFile();
        void readPhysicalNames();
        void readEntities();
        void readNodes();
        void readElements();
        void determinePhysicalEntitiesRange();
        void addSections();
        void addElements(std::vector<SectionData>& sections);
        void findVertices(std::vector<SectionData>& sections);

        void findLine(std::string content);

        std::string filePath;
        std::ifstream file;
        std::string line;

        std::string version;
        bool isLegacy;

        std::vector<int> sectionsIndices;
        std::vector<int> sectionsDimension;
        std::vector<std::string> sectionsNames;
        std::vector<std::vector<int>> localToGlobal;

        std::vector<std::vector<int>> connectivities;

        std::vector<std::array<long int, 2>> physicalEntitiesRange;

        int sectionIndex = 0;
        int typeIndex = 1;
        int nodeIndex = 2;

        int shift = 0;
};

#endif
