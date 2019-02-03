#ifndef __CGNS_INTERFACE_CGNS_READER_2D_HPP__
#define __CGNS_INTERFACE_CGNS_READER_2D_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"

class CgnsReader2D : public CgnsReader {
    public:
        CgnsReader2D(std::string filePath, bool readInConstructor = true);

    protected:
        void readCoordinates() override;
        bool skipSection(int elementType) override;
        void addEntity(int elementType, int elementStart, int elementEnd) override;
        void findBoundaryVertices() override;
        void findRegionVertices() override;
};

#endif
