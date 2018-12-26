#ifndef __MSH_INTERFACE_MSH_READER_2D_HPP__
#define __MSH_INTERFACE_MSH_READER_2D_HPP__

#include "MSHtoCGNS/MshInterface/MshReader.hpp"

class MshReader2D : public MshReader {
    public:
        MshReader2D(std::string filePath);

        ~MshReader2D() = default;

    private:
        void addPhysicalEntities() override;
        void addElements() override;
        void addFacets() override;
        void findBoundaryVertices() override;
        void findRegionVertices() override;
};

#endif
