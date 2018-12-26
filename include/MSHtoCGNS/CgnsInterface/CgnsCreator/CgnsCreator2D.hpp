#ifndef __CGNS_INTERFACE_CGNS_CREATOR_2D_HPP__
#define __CGNS_INTERFACE_CGNS_CREATOR_2D_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"

class CgnsCreator2D : public CgnsCreator {
    public:
        CgnsCreator2D(boost::shared_ptr<GridData> gridData, std::string folderPath, bool createInConstructor = true);

    protected:
        void checkDimension() override;
        void setDimensions() override;
        void writeCoordinates() override;
        void writeRegions() override;
        void writeBoundaries() override;
};

#endif
