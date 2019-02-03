#include "MSHtoCGNS/FileMend/CgnsReader/SpecialCgnsReader3D.hpp"
#include <cgnslib.h>

SpecialCgnsReader3D::SpecialCgnsReader3D(std::string filePath) : CgnsReader3D(filePath, false) {
    this->readCoordinates();
    this->readSections();
    this->findBoundaryVertices();
    this->findRegionVertices();
}

bool SpecialCgnsReader3D::skipSection(int elementType) {
    std::string sectionName(this->buffer);
    if (sectionName.substr(sectionName.length() - 3) == "_1D" || sectionName.substr(sectionName.length() - 3) == "_0D")
        return true;
    else if (elementType == BAR_2)
        return true;
    else
        return false;
}
