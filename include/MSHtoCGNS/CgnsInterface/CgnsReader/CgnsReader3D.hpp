#ifndef CGNS_READER_3D_HPP
#define CGNS_READER_3D_HPP

#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"

class CgnsReader3D : public CgnsReader {
    public:
        CgnsReader3D(std::string filePath, bool readInConstructor = true);

    protected:
        void readCoordinates() override;
        void readSections() override;
        void addWell(std::string&& name, int start, int end);
        void findBoundaryVertices() override;
        void findRegionVertices() override;
        void findWellVertices();
};

#endif
