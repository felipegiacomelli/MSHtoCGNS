#ifndef __CGNS_INTERFACE_CGNS_READER_HPP__
#define __CGNS_INTERFACE_CGNS_READER_HPP__

#include <set>

#include "MSHtoCGNS/BoostInterface/String.hpp"
#include "MSHtoCGNS/Utilities/Vector.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"
#include "MSHtoCGNS/CgnsInterface/CgnsOpener.hpp"

class CgnsReader : public CgnsOpener {
    public:
        CgnsReader(std::string filePath, bool readInConstructor = true);

        std::vector<double> readField(std::string solutionName, std::string fieldName);
        std::vector<double> readField(int solutionIndex, std::string fieldName);
        int readNumberOfTimeSteps();
        std::vector<double> readTimeInstants();

        boost::shared_ptr<GridData> gridData;

        virtual ~CgnsReader() = default;

    protected:
        void createGridData();
        virtual void read();
        void readNumberOfSections();
        void readCoordinates();
        void readSections();
        virtual bool skipSection(std::string, int);
        void addConnectivities(const std::vector<int>& connectivities, int elementType);
        template<typename T> void addConnectivity(std::vector<T>& connectivities, const std::vector<int>& connectivity);
        template<typename T> void addConnectivity(std::vector<T>& connectivities, const std::vector<int>& connectivity, int numberOfElements, int numberOfVertices);
        void addSection(int elementType);
        void addSection(std::vector<SectionData>& sections, int dimension);

        void findVertices(std::vector<SectionData>& sections);

        int readSolutionIndex(std::string solutionName);

        int numberOfSections;
        int one = 1;
        int elementStart, elementEnd;
};

#endif
