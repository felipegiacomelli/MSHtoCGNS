#ifndef MSH_READER_HPP
#define MSH_READER_HPP

#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <numeric>
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
        void readNodes();
        virtual void readPhysicalEntities() = 0;
        void readConnectivities();
        virtual void determineNumberOfFacets() = 0;
        void divideConnectivities();
        void assignElementsToRegions();
        void assignFacetsToBoundaries();
        virtual void addRegions() = 0;
        virtual void addBoundaries() = 0;
        virtual void defineBoundaryVertices() = 0;

        std::string filePath;
        std::ifstream file;
        char buffer[1024];
        int numberOfPhysicalEntities, numberOfBoundaries, numberOfRegions, numberOfFacets;
        std::vector<std::vector<int>> connectivities, elements, facets, regionElements, boundaryFacets;
};

#endif
