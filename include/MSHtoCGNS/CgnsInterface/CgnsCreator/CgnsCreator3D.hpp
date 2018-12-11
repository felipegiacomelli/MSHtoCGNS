#ifndef CGNS_CREATOR_3D_HPP
#define CGNS_CREATOR_3D_HPP

#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"

class CgnsCreator3D : public CgnsCreator {
    public:
        CgnsCreator3D(boost::shared_ptr<GridData> gridData, std::string folderPath);

    protected:
        void checkDimension() override;
        void setDimensions() override;
        void writeCoordinates() override;
        void writeSections() override;
        void writeRegions() override;
        void writeBoundaries() override;
        void writeWells();
};

#endif
