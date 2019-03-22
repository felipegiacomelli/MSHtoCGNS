#ifndef __MSH_INTERFACE_MSH_READER_3D_HPP__
#define __MSH_INTERFACE_MSH_READER_3D_HPP__

#include "MSHtoCGNS/MshInterface/MshReader.hpp"

class MshReader3D : public MshReader {
    public:
        MshReader3D(std::string filePath);

        ~MshReader3D() = default;

    private:
        void addPhysicalEntities() override;
        void addWell(std::string name, int begin, int end);
        void addElements() override;
        void addFacets() override;
        void addLines();
        void findBoundaryVertices() override;
        void findRegionVertices() override;
        void findWellVertices();
};

#endif
