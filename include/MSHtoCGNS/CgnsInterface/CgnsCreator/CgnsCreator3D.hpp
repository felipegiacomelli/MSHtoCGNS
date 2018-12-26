#ifndef __CGNS_INTERFACE_CGNS_CREATOR_3D_HPP__
#define __CGNS_INTERFACE_CGNS_CREATOR_3D_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"

class CgnsCreator3D : public CgnsCreator {
    public:
        CgnsCreator3D(boost::shared_ptr<GridData> gridData, std::string folderPath, bool createInConstructor = true);

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
