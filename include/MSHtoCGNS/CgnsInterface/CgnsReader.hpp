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

        void readSections();
        virtual bool skipSection(int elementType) = 0;
        virtual void addEntity(int elementType, int elementStart, int elementEnd) = 0;
        void addRegion(std::string&& name, int begin, int end);
        void addBoundary(std::string&& name, int begin, int end);
        void addConnectivities(int elementType, int elementStart, int elementEnd, const std::vector<int> connectivities);

        template<typename T, typename U>
        void findVertices(const std::vector<T>& connectivities, const std::vector<U>& entities, std::vector<std::set<int>>& vertices) {
            for (const auto& connectivity : connectivities)
                    for (unsigned e = 0u; e < entities.size(); ++e)
                        if (connectivity.back() >= entities[e].begin && connectivity.back() < entities[e].end) {
                            vertices[e].insert(connectivity.cbegin(), connectivity.cend() - 1);
                            break;
                        }
        }
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
        int one = 1;
};

#endif
