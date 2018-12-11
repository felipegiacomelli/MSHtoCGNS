#ifndef MSH_READER_3D_HPP
#define MSH_READER_3D_HPP

#include "MSHtoCGNS/MshInterface/MshReader.hpp"

class MshReader3D : public MshReader {
    public:
        MshReader3D(std::string filePath);

        ~MshReader3D() = default;

    private:
        void addPhysicalEntities() override;
        void addElements() override;
        void addFacets() override;
        void defineBoundaryVertices() override;
};

#endif
