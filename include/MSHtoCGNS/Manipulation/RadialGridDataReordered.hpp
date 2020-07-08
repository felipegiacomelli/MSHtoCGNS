#ifndef __MANIPULATION_RADIAL_GRID_DATA_REORDERED_HPP__
#define __MANIPULATION_RADIAL_GRID_DATA_REORDERED_HPP__

#include <set>
#include <numeric>

#include <boost/unordered_map.hpp>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/Utilities/Algorithm.hpp"
#include "MSHtoCGNS/GridData/GridData.hpp"

class RadialGridDataReordered {
    public:
        RadialGridDataReordered(boost::shared_ptr<GridData> gridData);

        ~RadialGridDataReordered() = default;

        boost::shared_ptr<GridData> reordered;

        double tolerance = 1.0e-4;

    private:
        void checkGridData();
        void defineQuantities();
        void createReordered();
        void reorderBoundaries();
        void copyData();
        void reorder();
        void buildFirstSection();
        void addVertex(int vertex, int section);
        void updateTriangle(std::vector<std::vector<int>>::iterator prism, std::vector<std::vector<int>>::iterator triangle);
        void updateQuadrangle(std::vector<std::vector<int>>::iterator hexahedron, std::vector<std::vector<int>>::iterator quadrangle);
        void copyHexahedron(std::vector<std::vector<int>>::iterator hexahedron);
        void copyPrism(std::vector<std::vector<int>>::iterator prism);
        void copyVertices();

        void rectifyConnectivities();

        void fixElementIndices();

        boost::shared_ptr<GridData> gridData;

        int numberOfSegments;
        int numberOfPrismsPerSegment;
        int numberOfHexahedronsPerSegment;
        int numberOfHexahedronsPerRadius;
        int numberOfVerticesPerSection;

        std::vector<std::vector<int>> hexahedrons;
        std::vector<std::vector<int>> prisms;
        std::vector<std::vector<int>> triangles;
        std::vector<std::vector<int>> quadrangles;

        std::vector<std::pair<int, int>> vertices;

        int vertexShift = 0;
        int elementShift = 0;
};

#endif
