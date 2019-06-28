#ifndef __CGNS_INTERFACE_CGNS_READER_HPP__
#define __CGNS_INTERFACE_CGNS_READER_HPP__

#include <string>
#include <set>

#include "MSHtoCGNS/CgnsInterface/CgnsOpener.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

class CgnsReader : public CgnsOpener {
    public:
        CgnsReader(std::string filePath);

        std::vector<double> readField(std::string solutionName, std::string fieldName);
        std::vector<double> readField(int solutionIndex, std::string fieldName);
        int readNumberOfTimeSteps();
        std::vector<double> readTimeInstants();

        boost::shared_ptr<GridData> gridData;

        virtual ~CgnsReader() = default;

    protected:
        void readNumberOfSections();
        void createGridData();

        virtual void readCoordinates() = 0;

        void readSections();
        virtual bool skipSection() = 0;
        void addConnectivities(const std::vector<int>& connectivities);

        template<typename T>
        void addConnectivity(std::vector<T>& connectivities, const std::vector<int>& connectivity) {
            connectivities.emplace_back(T{});
            std::copy_n(std::cbegin(connectivity), connectivity.size(), std::begin(connectivities.back()));
        }

        template<typename T>
        void addConnectivity(std::vector<T>& connectivities, const std::vector<int>& connectivity, int numberOfElements, int numberOfVertices) {
            for (int e = 0; e < numberOfElements; ++e) {
                connectivities.emplace_back(T{});
                auto& element = connectivities.back();
                for (int k = 0; k < numberOfVertices; ++k)
                    element[k] = connectivity[e * numberOfVertices + k] - 1;
                element.back() = this->elementStart - 1 + e;
            }
        }

        virtual void addEntity(int elementType) = 0;
        void addRegion(std::string&& name, int begin, int end);
        void addBoundary(std::string&& name, int begin, int end);

        template<typename T, typename U>
        void findVertices(const std::vector<T>& connectivities, const std::vector<U>& entities, std::vector<std::set<int>>& vertices) {
            for (const auto& connectivity : connectivities) {
                for (unsigned e = 0u; e < entities.size(); ++e) {
                    if (connectivity.back() >= entities[e].begin && connectivity.back() < entities[e].end) {
                        vertices[e].insert(connectivity.cbegin(), connectivity.cend() - 1);
                        break;
                    }
                }
            }
        }
        virtual void findBoundaryVertices() = 0;
        virtual void findRegionVertices() = 0;

        int readSolutionIndex(std::string solutionName);

        int numberOfSections, numberOfBoundaries;
        int one = 1;
        int elementType, elementStart, elementEnd;
};

#endif
